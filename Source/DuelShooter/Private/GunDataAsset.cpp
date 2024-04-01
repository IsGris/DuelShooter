// All rights by IsGris


#include "GunDataAsset.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(GunDatasLog);

void FGunConsumables::InitGun( const FGunInfo& GunInfo, const AActor& OwnerActor)
{
	Gun = &GunInfo;
	Owner = &OwnerActor;
	BulletsInMagazineRemains = GunInfo.MagazineAmmoCount;
	BulletsNotInMagazineRemains = FGenericPlatformMath::Max(0, GunInfo.AmmoOnStart - GunInfo.MagazineAmmoCount);
	CurrentBulletShotedContinouslyCount = 0;
}

bool FGunConsumables::CanReload() const
{
	return BulletsInMagazineRemains < Gun->MagazineAmmoCount && BulletsNotInMagazineRemains > 0 && !bIsReloading;
}

void FGunConsumables::EndReload()
{
	if (!bIsReloading)
		return;

	bIsReloading = false;
	ResetSpread();
	int HowManyBulletsTakeFromMagazine = FGenericPlatformMath::Min( Gun->MagazineAmmoCount - BulletsInMagazineRemains, BulletsNotInMagazineRemains );
	BulletsNotInMagazineRemains -= HowManyBulletsTakeFromMagazine;
	BulletsInMagazineRemains += HowManyBulletsTakeFromMagazine;
	OnReloadEnd.Broadcast();
}

void FGunConsumables::StartReload()
{
	if (!CanReload())
		return;

	bIsReloading = true;

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

bool FGunConsumables::IsReloading() const
{
	return bIsReloading;
}

void FGunConsumables::ResetSpread()
{
	CurrentBulletShotedContinouslyCount = 0;
}

void FGunConsumables::AppendSpread()
{
	CurrentBulletShotedContinouslyCount += 1;
}

bool FGunConsumables::CanShoot() const
{
	return BulletsInMagazineRemains > 0 && LastShootTime + FTimespan::FromSeconds(Gun->ShotsDelay) <= FDateTime::Now() && !bIsReloading;
}

void FGunConsumables::MakeShot()
{
	BulletsInMagazineRemains -= 1;

	// Does we need to reset spread of gun
	if (LastShootTime + FTimespan::FromSeconds(Gun->ShotsDelay) + FTimespan::FromSeconds(TIME_BETWEEN_SHOTS_TO_RESET_SPREAD) < FDateTime::Now())
		ResetSpread();

	AppendSpread();
	LastShootTime = FDateTime::Now();
}

FRotator FGunConsumables::GetGunRotationToAppendForSpread() 
{
	if (!Gun->Spread.IsEmpty())
		return FRotator(
			-Gun->Spread[FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount - 1, Gun->Spread.Num() - 1)].Value,
			-Gun->Spread[FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount - 1, Gun->Spread.Num() - 1)].Key,
			0);
	else
		return FRotator();
}

FRotator FGunConsumables::GetCharacterRotationToAppendForSpread()
{
	if ( !Gun->Spread.IsEmpty() )
		return FRotator(
			-Gun->Spread[FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount - 1, Gun->Spread.Num() - 1)].Value * CHARACTER_ROTATION_SPREAD_MULTIPLIER,
			-Gun->Spread[FGenericPlatformMath::Min(CurrentBulletShotedContinouslyCount - 1, Gun->Spread.Num() - 1)].Key * CHARACTER_ROTATION_SPREAD_MULTIPLIER,
			0 );
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
