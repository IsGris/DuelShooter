// All rights by IsGris


#include "ShootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DuelShooterGameMode.h"
#include "GunDataAsset.h"
#include "BulletDamage.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(DuelShooterShootComponentLog);

void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryComponentTick.bCanEverTick = false;
	
	Owner = GetOwner();
	GunConsumables = NewObject<UGunConsumables>();

	// Init gun
	if ( GunId > -1 )
		if ( auto GameMode = Cast<ADuelShooterGameMode>( UGameplayStatics::GetGameMode( GetWorld() ) ) )
			if ( auto GunData = GameMode->GetGunData( GunId ) )
			{
				Gun = GunData;
				GunConsumables->InitGun( *GunData, Owner, this );
			}
}

void UShootComponent::SetGun( const int& NewGunId )
{
	GunId = NewGunId;

	// Init gun
	if ( GunId > -1 )
		if ( auto GameMode = Cast<ADuelShooterGameMode>( UGameplayStatics::GetGameMode( GetWorld() ) ) )
			if ( auto GunData = GameMode->GetGunData( GunId ) )
			{
				Gun = GunData;
				GunConsumables->InitGun(*GunData, Owner, this );
			}
}

void UShootComponent::SetGunByInfo( const FGunInfo& NewGunInfo )
{
	GunId = -1;
	Gun = &NewGunInfo;
	GunConsumables->InitGun( NewGunInfo, Owner, this );
}

void UShootComponent::StartShooting()
{
	if (!Gun->bCanShootByHoldDownButton)
		Shoot();
	else
	{
		if (auto World = GetWorld())
			if (auto* TimerManager = &World->GetTimerManager())
				(*TimerManager).SetTimer(ShootingTimer, this, &UShootComponent::Shoot, Gun->ShotsDelay, true, 0);
	}
}

void UShootComponent::EndShooting()
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			(*TimerManager).ClearTimer(ShootingTimer);
}

void UShootComponent::Reload()
{
	GunConsumables->StartReload();
}

void UShootComponent::Shoot()
{
	if ( !GunConsumables->CanShoot() )
		return;
	
	auto OwnerRotation = Owner->GetActorRotation();
	OwnerRotation = { OwnerRotation.Yaw, OwnerRotation.Pitch,0 };
	FHitResult BulletHitResult;
	FRotator NewRotationBullet = OwnerRotation + GunConsumables->SpreadRotator;
	if (auto World = GetWorld())
		if (World->LineTraceSingleByChannel(BulletHitResult, Owner->GetActorLocation(), (UKismetMathLibrary::GetForwardVector(FRotator{NewRotationBullet.Yaw, NewRotationBullet.Pitch, 0})) * BULLET_DISTANCE, ECollisionChannel::ECC_Visibility))
		{
			if (BulletHitResult.BoneName.ToString().Contains("head"))
				UGameplayStatics::ApplyPointDamage(BulletHitResult.GetActor(), Gun->Damage * DAMAGE_MULTIPLIER_IN_HEAD, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass());
			else if (BulletHitResult.BoneName.ToString().Contains("body"))
				UGameplayStatics::ApplyPointDamage(BulletHitResult.GetActor(), Gun->Damage * DAMAGE_MULTIPLIER_IN_BODY, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass());
			else if (BulletHitResult.BoneName.ToString().Contains("hand"))
				UGameplayStatics::ApplyPointDamage(BulletHitResult.GetActor(), Gun->Damage * DAMAGE_MULTIPLIER_IN_HAND, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass());
			else if (BulletHitResult.BoneName.ToString().Contains("leg"))
				UGameplayStatics::ApplyPointDamage(BulletHitResult.GetActor(), Gun->Damage * DAMAGE_MULTIPLIER_IN_LEG, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass());
			else 
				UGameplayStatics::ApplyPointDamage(BulletHitResult.GetActor(), Gun->Damage, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass());
		}

	DrawDebugSphere(GetWorld(), BulletHitResult.Location, 4, 3, FColor::Yellow, false, 2);

	OnShoot.Broadcast(GunConsumables->GetCharacterRotationToAppendForSpread(), GunConsumables->GetGunRotationToAppendForSpread());
	auto NewRot = Owner->GetActorRotation();
}

bool UShootComponent::IsShooting() const
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			return TimerManager->IsTimerActive( ShootingTimer );
	
	return false;
}