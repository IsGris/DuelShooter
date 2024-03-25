// All rights by IsGris


#include "ShootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DuelShooterGameMode.h"
#include "GunDataAsset.h"
#include "Misc/Timespan.h"
#include "BulletDamage.h"

void UShootComponent::BeginPlay()
{
	Super::BeginPlay();
	
	PrimaryComponentTick.bCanEverTick = false;
	
	Owner = GetOwner();

	// Init gun
	if ( GunId > -1 )
		if ( auto GameMode = Cast<ADuelShooterGameMode>( UGameplayStatics::GetGameMode( GetWorld() ) ) )
			if ( auto GunData = GameMode->GetGunData( GunId ) )
			{
				Gun = GunData;
				GunConsumables.InitGun( *GunData, *Owner );
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
				GunConsumables.InitGun( *GunData, *Owner );
			}
}

void UShootComponent::SetGunByInfo( const FGunInfo& NewGunInfo )
{
	GunId = -1;
	Gun = &NewGunInfo;
	GunConsumables.InitGun( NewGunInfo, *Owner );
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
	GunConsumables.StartReload();
}

void UShootComponent::Shoot()
{
	if ( !GunConsumables.CanShoot() )
		return;
	
	GunConsumables.MakeShot();

	FHitResult BulletHitResult;
	if (auto World = GetWorld())
		if (World->LineTraceSingleByChannel(BulletHitResult, Owner->GetActorLocation(), Owner->GetActorForwardVector() * BULLET_DISTANCE, ECollisionChannel::ECC_Visibility))
			UGameplayStatics::ApplyPointDamage(BulletHitResult.GetActor(), Gun->Damage, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass());
	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorForwardVector() * BULLET_DISTANCE, FColor::Red, false, 2, 0, 2);

	AppendSpreadToScreen();

	OnShoot.Broadcast();
}

void UShootComponent::ResetSpread()
{
	GunConsumables.ResetSpread();
}

void UShootComponent::AppendSpreadToScreen()
{
	Owner->SetActorRotation( Owner->GetActorRotation() + GunConsumables.GetRotationToAppendForSpread() );
}

bool UShootComponent::IsShooting() const
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			return TimerManager->IsTimerActive( ShootingTimer );
	
	return false;
}
