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

	// Init gun
	if ( GunId > -1 )
		if ( auto GameMode = Cast<ADuelShooterGameMode>( UGameplayStatics::GetGameMode( GetWorld() ) ) )
			if ( auto GunData = GameMode->GetGunData( GunId ) )
			{
				Gun = GunData;
				GunConsumables.InitGun( *GunData );
			}

	Owner = GetOwner();
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
				GunConsumables.InitGun( *GunData );
			}
}

void UShootComponent::SetGunByInfo( const FGunInfo& NewGunInfo )
{
	GunId = -1;
	Gun = &NewGunInfo;
	GunConsumables.InitGun( NewGunInfo );
}

void UShootComponent::StartShooting()
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			( *TimerManager ).SetTimer( ShootingTimer, this, &UShootComponent::Shoot, Gun->ShotsDelay, true );
}

void UShootComponent::EndShooting()
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			(*TimerManager).ClearTimer(ShootingTimer);
}

void UShootComponent::Shoot()
{
	// Check for ammo
	if ( !( GunConsumables.BulletsInMagazineRemains > 0 ) )
		return;

	if ( LastShootTime + FTimespan::FromSeconds( TIME_BETWEENSHOTS_TO_RESET_SPREAD ) < FDateTime::Now() ) // Does we need to reset spread of gun before shooting
	{
		ResetSpread();
		MakeShot();
		AppendSpread();
	}
	else
	{
		MakeShot();
		AppendSpread();
	}
}

void UShootComponent::MakeShot()
{
	GunConsumables.BulletsInMagazineRemains -= 1;
	GunConsumables.CurrentBulletShotedContinouslyCount += 1;

	FHitResult BulletHitResult;
	if ( auto World = GetWorld() )
		if ( World->LineTraceSingleByChannel( BulletHitResult, Owner->GetActorLocation(), Owner->GetActorForwardVector() * BULLET_DISTANCE, ECollisionChannel::ECC_Visibility ) )
			UGameplayStatics::ApplyPointDamage( BulletHitResult.GetActor(), Gun->Damage, BulletHitResult.TraceStart, BulletHitResult, Owner->GetInstigatorController(), Owner, UBulletDamage::StaticClass() );
	DrawDebugLine( GetWorld(), Owner->GetActorLocation(), Owner->GetActorForwardVector() * BULLET_DISTANCE, FColor::Red, false, 2, 0, 2 );

	LastShootTime = FDateTime::Now();
}

void UShootComponent::ResetSpread()
{
	GunConsumables.CurrentBulletShotedContinouslyCount = 0;
}

void UShootComponent::AppendSpread()
{
	Owner->SetActorRotation( Owner->GetActorRotation() + GunConsumables.GetRotationToAppendForSpread() );
	GunConsumables.CurrentBulletShotedContinouslyCount += 1;
}

bool UShootComponent::IsShooting() const
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			return TimerManager->IsTimerActive( ShootingTimer );
	
	return false;
}
