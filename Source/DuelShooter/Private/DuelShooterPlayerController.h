// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputSettings.h"
#include "DuelShooterPlayerController.generated.h"

#define JUMP_FORCE 300

class ADuelShooterPlayer;

/**
 * PlayerController for DuelShooter Player
 */
UCLASS()
class DUELSHOOTER_API ADuelShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;


protected:
	TObjectPtr<ADuelShooterPlayer> PawnOwner;

protected:
	bool IsCrouching = false;
	// ~ Begin Look movement
protected:
	// Look input speed(rotator units/sec)
	UPROPERTY()
	FVector LookVelocity;
public:
	// Whether to maintain the look speed(rotator units/sec)
	UPROPERTY(BlueprintReadWrite, Category = "Game|Player|Velocity")
	bool bSaveLookVelocity = true;

	// Getter for look velocity(rotator units/sec)
	UFUNCTION(BlueprintCallable, Category = "Game|Player|Velocity")
	FVector GetLookVelocity() const;

	/**
	 * Add Pitch (look up) input. This value is multiplied by InputPitchScale.
	 * @param Val Amount to add to Pitch. This value is multiplied by InputPitchScale.
	 */
	virtual void AddPitchInput(float Val) override;

	/**
	 * Add Yaw (turn) input. This value is multiplied by InputYawScale.
	 * @param Val Amount to add to Yaw. This value is multiplied by InputYawScale.
	 */
	virtual void AddYawInput(float Val) override;

	/**
	 * Add Roll input. This value is multiplied by InputRollScale.
	 * @param Val Amount to add to Roll. This value is multiplied by InputRollScale.
	 */
	virtual void AddRollInput(float Val) override;
	// ~ End Look movement
};
