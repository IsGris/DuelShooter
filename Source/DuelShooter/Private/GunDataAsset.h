// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GunDataAsset.generated.h"

// Time between shots in seconds to reset spread to default value 
#define TIME_BETWEENSHOTS_TO_RESET_SPREAD 1
// How far forward can a bullet travel
#define BULLET_DISTANCE 10000

/**
 * Stores weapon data that can change
 */
USTRUCT( BlueprintType )
struct FGunConsumables
{
	GENERATED_BODY()

	void InitGun( const FGunInfo& GunInfo );

protected:
	const FGunInfo* Gun;

public:
	// How many bullets remaining in magazine of weapon
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int BulletsInMagazineRemains = 0;
	// How many bullets remaining in weapon not including magazine
	UPROPERTY( EditDefaultsOnly, BlueprintReadWrite, Category = "Gun" )
	int BulletsNotInMagazineRemains = 0;
	// How many bullets gun fired continuously
	UPROPERTY( BlueprintReadWrite, Category = "Spread" )
	int CurrentBulletShotedContinouslyCount = 0;
	// Gets rotation that needs to be added to the player to apply spread
	FRotator GetRotationToAppendForSpread();
};

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
	TArray<TPair<int, int>> Spread;
private:
	// A curve that displays Spread in camera when shooting
	// X-axis means how many bullets gun shooted starting from the beginning to the end of the curve
	// Y-axis means the deflection of the sight to the right or left
	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
	FRuntimeFloatCurve SpreadX;
	// A curve that displays Spread in camera when shooting
	// X-axis means how many bullets gun shooted starting from the beginning to the end of the curve
	// Y-axis means the deflection of the sight to the up or down
	UPROPERTY( EditDefaultsOnly, Category = "Gun" )
	FRuntimeFloatCurve SpreadY;
	// How big is the step between shots in the X axis to take into account all the spread available in the curve
	UPROPERTY()
	int IntervalsBetweenShotsInCurveX;
	// How big is the step between shots in the Y axis to take into account all the spread available in the curve
	UPROPERTY()
	int IntervalsBetweenShotsInCurveY;
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
