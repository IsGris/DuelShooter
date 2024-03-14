// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GunDataAsset.h"
#include "ShootComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FOnShootSignature );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UShootComponent : public UActorComponent
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

public:
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
	// Shoot 1 ammo
	UFUNCTION( BlueprintCallable )
	virtual void Shoot();
	// Makes shot without checking for ammo and other things
	virtual void MakeShot();
	// Reset spread of gun
	virtual void ResetSpread();
	// Adds spread to the sight
	virtual void AppendSpread();
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
	FGunConsumables GunConsumables;
	UPROPERTY( BlueprintReadWrite, Category = "Gun" )
	FDateTime LastShootTime;
	UPROPERTY()
	FTimerHandle ShootingTimer;
};
