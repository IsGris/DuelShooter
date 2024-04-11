// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Containers/Ticker.h"

#include "GunDataAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FReloadSignature);
DECLARE_MULTICAST_DELEGATE_FourParams( FOnSpreadAddedSignature, FRotator/*SpreadAddedToSight*/, FRotator/*SpreadAddedToCharacter*/, FRotator/*SightSpreadFromCharacter*/, FRotator/*CharacterRotation*/);
DECLARE_LOG_CATEGORY_EXTERN(GunDatasLog, Log, All);

// Time between shots in seconds to reset spread to default value 
#define TIME_BETWEEN_SHOTS_TO_RESET_SPREAD 2.0f
// How far forward can a bullet travel
#define BULLET_DISTANCE 10000
#define SIGHT_SPREAD_RESET_SPEED 0.5f
#define CHARACTER_ROTATION_SPREAD_MULTIPLIER 0.3f
#define DAMAGE_MULTIPLIER_IN_HEAD 1.0f
#define DAMAGE_MULTIPLIER_IN_BODY 0.3f
#define DAMAGE_MULTIPLIER_IN_HAND 0.25f
#define DAMAGE_MULTIPLIER_IN_LEG 0.18f

/**
 * Stores one weapon data
 */
USTRUCT( BlueprintType )
struct FGunInfo
{
	GENERATED_BODY()

	void Init();

public:
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Cosmetic" )
	FString GunName;
	// If true, then the person will fire all the cartridges from the magazine while holding down the fire button, 
	// Otherwise he will have to press the fire button again each time to shoot
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Cosmetic" )
	bool bCanShootByHoldDownButton = true;
	// Time between shots in seconds
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	float ShotsDelay = 1;
	// Time needed to reload gun
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	float ReloadTime = 1;
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int Damage = 1;
	// How many ammo can store one magazine
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int MagazineAmmoCount = 0;
	// How many ammo this gun has in start
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int AmmoOnStart = 0;
	// Indicates the deflection for each bullet that is fired from this weapon
	// Number of elements = number of bullets in the magazine
	// The index indicates which cartridge we fired from the magazine
	// TPair in every element indicates the deviation along the X axis and Y axis
	TArray<TPair<float, float>> Spread;
private:
	// A curve that displays Spread in camera when shooting
	// X-axis means how many bullets gun shooted starting from 1 and ending to MagazineAmmoCount
	// Y-axis means the deflection of the sight to the right or left
	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
	FRuntimeFloatCurve SpreadX;
	// A curve that displays Spread in camera when shooting
	// X-axis means how many bullets gun shooted starting from 1 and ending to MagazineAmmoCount
	// Y-axis means the deflection of the sight to the up or down
	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
	FRuntimeFloatCurve SpreadY;
};

/**
 * Stores weapon data that can change
 */
UCLASS(BlueprintType)
class UGunConsumables : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void InitGun(const FGunInfo& GunInfo, AActor* OwnerActor, UShootComponent* OwnerShootComponent);

protected:
	const FGunInfo* Gun;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UShootComponent> ShootComponent;
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> Owner;
	// Does gun reloading
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Gun")
	bool bIsReloading = false;
	UFUNCTION(BlueprintCallable)
	bool CanReload() const;
	UFUNCTION()
	void EndReload();
	UPROPERTY(BlueprintReadWrite, Category = "Gun")
	FDateTime LastShootTime;
	UFUNCTION()
	void AppendSpread();
	UFUNCTION()
	void StartResetSpread();
	UFUNCTION()
	bool ResetSpreadTick(float DeltaTime);
	UFUNCTION()
	void EndResetSpread();
	FTickerDelegate ResetSpreadTickerDelegate;
	FTSTicker::FDelegateHandle ResetSpreadTickerDelegateHandle;
	UPROPERTY()
	FDateTime StartResetSpreadTime;
	UPROPERTY()
	FRotator RotationBeforeResetSpread = FRotator::ZeroRotator;
	UPROPERTY()
	FTimerHandle ResetSpreadAfterShootTimerHandle;

public:
	UPROPERTY(BlueprintAssignable)
	FReloadSignature OnReloadStart;
	UPROPERTY( BlueprintAssignable )
	FReloadSignature OnReloadEnd;
	// Event for spread added
	// * SpreadAddedToSight - added to sight rotation
	// * SpreadAddedToCharacter - added to player rotation
	// * SightSpreadFromCharacter - rotator that will be added to the player's rotator to get the crosshair location
	// * CharacterRotation - current player rotator
	FOnSpreadAddedSignature OnSpreadAdded;
	void StartReload();
	UFUNCTION(BlueprintCallable)
	bool IsReloading() const;
	UFUNCTION(BlueprintCallable)
	bool CanShoot() const;
	UFUNCTION()
	void MakeShot();
	// How many bullets remaining in magazine of weapon
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int BulletsInMagazineRemains = 0;
	// How many bullets remaining in weapon not including magazine
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int BulletsNotInMagazineRemains = 0;
	// How many bullets gun fired continuously
	UPROPERTY( BlueprintReadWrite, Category = "Spread" )
	int CurrentBulletShotedContinouslyCount = 0;
	/**
	* Кotation that needs to be added to the gun sight to apply spread
	* @param SpreadIndex what cartridge do we need the spread from
	*/
	UFUNCTION()
	FRotator GetGunRotationToAppendForSpread( const int& SpreadIndex = -1 );
	/**
	* Rotation that will be added to the view angle for player to apply spread
	* @param SpreadIndex what cartridge do we need the spread from
	*/
	UFUNCTION()
	FRotator GetCharacterRotationToAppendForSpread( const int& SpreadIndex = -1 );
	// Rotator that will be added to initial player rotation to add spread
	UPROPERTY( BlueprintReadWrite, Category = "Spread" )
	FRotator SpreadRotator = FRotator::ZeroRotator;
};

/**
 * Stores all weapon data
 */
UCLASS( BlueprintType )
class UGunDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY( BlueprintReadOnly )
	bool bIsInited = false;

public:
	void Init();

	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite )
	TArray<FGunInfo> Guns;
};
