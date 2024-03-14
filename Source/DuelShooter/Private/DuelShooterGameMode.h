// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DuelShooterGameMode.generated.h"

class UGunDataAsset;
struct FGunInfo;

DECLARE_LOG_CATEGORY_EXTERN( DuelShooterGameModeLog, Log, All );

/**
 * Game mode for duel shooter
 */
UCLASS()
class DUELSHOOTER_API ADuelShooterGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;

protected:
	// Stores data about all weapons in the game
	UPROPERTY( EditDefaultsOnly, Category = "Guns" )
	TSoftObjectPtr<UGunDataAsset> GunsDatas;

	// ~ Getters
public:
	/*
	* Finds gun with given id
	* @param GunId Id of the gun we looking for
	* @return GunInfo finded by id
	*/
	FGunInfo* GetGunData( const int GunId ) const;
	/*
	* Getter for gun data asset
	* @param GunId Id of the gun we looking for
	* @return GunInfo finded by id
	*/
	UFUNCTION( BlueprintCallable, Category = "Get" )
	UGunDataAsset* GetGunDataAsset() const;
	// ~ Getters
};

