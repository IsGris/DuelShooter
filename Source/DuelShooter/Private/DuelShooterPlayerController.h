// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
};
