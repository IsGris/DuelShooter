// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunDataAsset.h"
#include "ShootComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnShootSignature, FRotator, PlayerAddedRotation, FRotator, GunAddedRotation);

DECLARE_LOG_CATEGORY_EXTERN(DuelShooterShootComponentLog, Log, All);

class UWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UShootComponent : public UActorComponent
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
	// Event for shoot
	UPROPERTY( BlueprintAssignable, Category = "Shoot" )
	FOnShootSignature OnShoot;
	UFUNCTION( Exec, BlueprintCallable )
	void SetGun( const int& NewGunId );
	UFUNCTION( BlueprintCallable )
	void SetGunByInfo( const FGunInfo& NewGunInfo );
	// Start shooting from gun
	UFUNCTION( BlueprintCallable )
	virtual void StartShooting();
	// End shooting from gun
	UFUNCTION( BlueprintCallable )
	virtual void EndShooting();
	// Reload gun
	UFUNCTION(BlueprintCallable)
	void Reload();
	// Shoot 1 ammo
	UFUNCTION( BlueprintCallable )
	virtual void Shoot();
	// Checks does gun now shooting
	UFUNCTION( BlueprintCallable )
	bool IsShooting() const;
protected:
	UPROPERTY()
	TObjectPtr<AActor> Owner;
	// Id of current gun if -1 then no gun
	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
	int GunId;
	// Current gun
	const FGunInfo* Gun;
	UPROPERTY( BlueprintReadWrite, Category = "Gun" )
	UGunConsumables* GunConsumables;
	UPROPERTY()
	FTimerHandle ShootingTimer;
};
