// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DuelShooterPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UShootComponent;

DECLARE_LOG_CATEGORY_EXTERN( DuelShooterPlayerLog, Log, All );

UCLASS()
class DUELSHOOTER_API ADuelShooterPlayer : public APawn
{
	GENERATED_BODY()

	ADuelShooterPlayer();
	virtual void BeginPlay() override;

	// ~ Components
protected:
	UPROPERTY( BlueprintReadWrite, Category = "Components" )
	UShootComponent* ShootComponent;
	// ~ Components
	
	// ~ Input
	// ~ ~ Inits
protected:
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ~ ~ Inits
	// ~ ~ Functions
protected:
	void SwapShootingState();
	// ~ ~ Functions
	// ~ ~ Variables
protected:
	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	UInputMappingContext* PlayerInputs;
	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	UInputAction* ShootSwap;
	// ~ ~ Variables
	// ~ Input

};
