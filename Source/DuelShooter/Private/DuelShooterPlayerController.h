// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DuelShooterPlayerController.generated.h"

/**
 * PlayerController for DuelShooter Player
 */
UCLASS()
class DUELSHOOTER_API ADuelShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
};
