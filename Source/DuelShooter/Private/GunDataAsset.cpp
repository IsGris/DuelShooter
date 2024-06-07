// All rights by IsGris


#include "GunDataAsset.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "ShootComponent.h"
#include "Containers/Ticker.h"

DEFINE_LOG_CATEGORY(GunDatasLog);

void UGunConsumables::InitGun(const FGunInfo& GunInfo, AActor* OwnerActor, UShootComponent* OwnerShootComponent )
{
	Gun = &GunInfo;
	Owner = OwnerActor;
	ShootComponent = OwnerShootComponent;
	OwnerShootComponent->OnShoot.AddUniqueDynamic(this, &UGunConsumables::MakeShot);
	BulletsInMagazineRemains = Gun->MagazineAmmoCount;
	BulletsNotInMagazineRemains = FGenericPlatformMath::Max(0, Gun->AmmoOnStart - Gun->MagazineAmmoCount);
	CurrentBulletShotedContinouslyCount = 0;
	ResetSpreadTickerDelegate = FTickerDelegate::CreateUObject(this, &UGunConsumables::ResetSpreadTick);
}

bool UGunConsumables::CanReload() const
{
	return BulletsInMagazineRemains < Gun->MagazineAmmoCount && BulletsNotInMagazineRemains > 0 && !bIsReloading;
}

void UGunConsumables::EndReload()
{
	if (!bIsReloading)
		return;

	bIsReloading = false;
	int HowManyBulletsTakeFromMagazine = FGenericPlatformMath::Min( Gun->MagazineAmmoCount - BulletsInMagazineRemains, BulletsNotInMagazineRemains );
	BulletsNotInMagazineRemains -= HowManyBulletsTakeFromMagazine;
	BulletsInMagazineRemains += HowManyBulletsTakeFromMagazine;
	OnReloadEnd.Broadcast();
}

void UGunConsumables::StartReload()
{
	if (!CanReload())
		return;

	bIsReloading = true;
	StartResetSpread();

	if (auto World = Owner->GetWorld())
		if (auto* TimerManager = &World->GetTimerManager()) 
		{
			FTimerHandle UnusedHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this] {
				EndReload();
				});
			TimerManager->SetTimer(UnusedHandle, TimerDelegate, Gun->ReloadTime, false);
		}

	OnReloadStart.Broadcast();
}

bool UGunConsumables::IsReloading() const
{
	return bIsReloading;
}

void UGunConsumables::AppendSpread()
{
	auto RotationToAppend = GetCharacterRotationToAppendForSpread();
	if (auto OwnerAsPawn = Cast<APawn>(Owner)) {

		if (OwnerAsPawn->bUseControllerRotationYaw)
			OwnerAsPawn->AddControllerYawInput(RotationToAppend.Pitch);
		else
			Owner->AddActorLocalRotation(FRotator(0, RotationToAppend.Yaw, 0));

		if (OwnerAsPawn->bUseControllerRotationPitch)
			OwnerAsPawn->AddControllerPitchInput(RotationToAppend.Yaw);
		else
			Owner->AddActorLocalRotation(FRotator(0, 0, RotationToAppend.Pitch));

	}
	else
		Owner->AddActorLocalRotation(RotationToAppend);
	SpreadRotator += GetGunRotationToAppendForSpread();
	OnSightSpreadChanged.Broadcast(SpreadRotator);
	CurrentBulletShotedContinouslyCount += 1;
}

void UGunConsumables::StartResetSpread()
{
	FTSTicker::GetCoreTicker().RemoveTicker(ResetSpreadTickerDelegateHandle);
	//ResetSpreadTickerDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(ResetSpreadTickerDelegate);
	StartResetSpreadTime = FDateTime::Now();
}

bool UGunConsumables::ResetSpreadTick(float DeltaTime)
{
	FRotator RotationToDecreaseFromSight = FRotator(SIGHT_SPREAD_RESET_SPEED * DeltaTime, SIGHT_SPREAD_RESET_SPEED * DeltaTime, 0);
	if (SpreadRotator.Pitch < 0)
		RotationToDecreaseFromSight.Pitch *= -1;
	if (SpreadRotator.Yaw < 0)
		RotationToDecreaseFromSight.Yaw *= -1;
	if (FMath::Abs(RotationToDecreaseFromSight.Pitch) >= FMath::Abs(SpreadRotator.Pitch) || SpreadRotator.Pitch == 0)
		RotationToDecreaseFromSight.Pitch = SpreadRotator.Pitch;
	if (FMath::Abs(RotationToDecreaseFromSight.Yaw) >= FMath::Abs(SpreadRotator.Yaw) || SpreadRotator.Yaw == 0)
		RotationToDecreaseFromSight.Yaw = SpreadRotator.Yaw;
	SpreadRotator -= RotationToDecreaseFromSight;
	OnSightSpreadChanged.Broadcast(SpreadRotator);
	if (SpreadRotator.IsZero())
		EndResetSpread();

	return true;
}

void UGunConsumables::EndResetSpread()
{
	FTSTicker::GetCoreTicker().RemoveTicker(ResetSpreadTickerDelegateHandle);
	CurrentBulletShotedContinouslyCount = 0;
	SpreadRotator = FRotator::ZeroRotator;
}

bool UGunConsumables::CanShoot() const
{
	return BulletsInMagazineRemains > 0 && LastShootTime + FTimespan::FromSeconds(Gun->ShotsDelay) <= FDateTime::Now() && !bIsReloading;
}

void UGunConsumables::MakeShot( FRotator PlayerAddedRotation, FRotator GunAddedRotation)
{
	BulletsInMagazineRemains -= 1;

	if (auto World = Owner->GetWorld())
		if (auto* TimerManager = &World->GetTimerManager()) {
			if ((*TimerManager).IsTimerActive(ResetSpreadAfterShootTimerHandle)) 
				(*TimerManager).ClearTimer(ResetSpreadAfterShootTimerHandle);
			FTSTicker::GetCoreTicker().RemoveTicker(ResetSpreadTickerDelegateHandle);

			(*TimerManager).SetTimer(ResetSpreadAfterShootTimerHandle, this, &UGunConsumables::StartResetSpread, 0.00001f, false, TIME_BETWEEN_SHOTS_TO_RESET_SPREAD);
		}

	AppendSpread();
	LastShootTime = FDateTime::Now();
}

FRotator UGunConsumables::GetGunRotationToAppendForSpread(const int& SpreadIndex)
{
	if (!Gun->Spread.IsEmpty())
		return FRotator(
			Gun->Spread[
				SpreadIndex == -1 ?
					FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount, Gun->Spread.Num() - 1) :
					SpreadIndex
				].Key,
			Gun->Spread[
				SpreadIndex == -1 ?
					FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount, Gun->Spread.Num() - 1) :
					SpreadIndex
				].Value,
					0
					) * GUN_ROTATION_SPREAD_MULTIPLIER;
	else
		return FRotator();
}

FRotator UGunConsumables::GetCharacterRotationToAppendForSpread(const int& SpreadIndex)
{
	if ( !Gun->Spread.IsEmpty() )
		return FRotator(
			Gun->Spread[
				SpreadIndex == -1 ?
					FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount, Gun->Spread.Num() - 1) :
					SpreadIndex
				].Key,
			Gun->Spread[
				SpreadIndex == -1 ?
					FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount, Gun->Spread.Num() - 1) :
					SpreadIndex
				].Value,
					0
			) * CHARACTER_ROTATION_SPREAD_MULTIPLIER;
	else
		return FRotator();
}

void FGunInfo::Init()
{
	if ( MagazineAmmoCount > 0 )
	{
		Spread.Empty();
		Spread.Reserve( MagazineAmmoCount );
		for ( int i{1}; i <= MagazineAmmoCount; i++ )
		{
			Spread.Add( { SpreadX.GetRichCurveConst()->Eval( i ), SpreadY.GetRichCurveConst()->Eval( i ) } );
		}
	}
}

void UGunDataAsset::Init()
{
	bIsInited = true;
	for ( auto& GunInfo : Guns )
		GunInfo.Init();
}
