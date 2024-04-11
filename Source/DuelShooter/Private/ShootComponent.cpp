// All rights by IsGris


#include "ShootComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DuelShooterGameMode.h"
#include "GunDataAsset.h"
#include "Misc/Timespan.h"
#include "BulletDamage.h"
#include "Kismet/KismetMathLibrary.h"

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
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), *(UKismetMathLibrary::GetForwardVector(Owner->GetActorRotation() + GunConsumables->SpreadRotator).ToString()));

	FHitResult BulletHitResult;
	if (auto World = GetWorld())
		if (World->LineTraceSingleByChannel(BulletHitResult, Owner->GetActorLocation(), UKismetMathLibrary::GetForwardVector(Owner->GetActorRotation() + GunConsumables->SpreadRotator) * BULLET_DISTANCE, ECollisionChannel::ECC_Visibility))
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
	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), UKismetMathLibrary::GetForwardVector(Owner->GetActorRotation() + GunConsumables->SpreadRotator) * BULLET_DISTANCE, FColor::Red, false, 2, 0, 2);

	OnShoot.Broadcast();
}

bool UShootComponent::IsShooting() const
{
	if ( auto World = GetWorld() )
		if ( auto* TimerManager = &World->GetTimerManager() )
			return TimerManager->IsTimerActive( ShootingTimer );
	
	return false;
}
