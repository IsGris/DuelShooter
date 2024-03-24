// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DuelShooterPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class UShootComponent;
class UCameraComponent;
class ADuelShooterPlayerController;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN( DuelShooterPlayerLog, Log, All );

UCLASS()
class DUELSHOOTER_API ADuelShooterPlayer : public ACharacter
{
	GENERATED_BODY()

	friend ADuelShooterPlayerController;

	ADuelShooterPlayer();
	virtual void BeginPlay() override;

	// ~ Components
protected:
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "Components" )
	UShootComponent* ShootComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;
	// ~ Components
	
	// ~ Input
	// ~ ~ Inits
protected:
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// ~ ~ Inits
	// ~ ~ Functions
	// Functions for handle actions
protected:
	void SwapShootingState();
	void MakeLook(const FInputActionValue& ActionValue);
	void MakeMove(const FInputActionValue& ActionValue);
	void MakeCrouch();
	void MakeUnCrouch();
	void GoBack();
	// ~ ~ Functions
	// ~ ~ Variables
protected:
	TObjectPtr<ADuelShooterPlayerController> PlayerController;
	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	UInputMappingContext* PlayerInputs;
	UPROPERTY( EditDefaultsOnly, Category = "Input" )
	UInputAction* ShootSwapAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* UnCrouchAction;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;
	// Action for go back in interface
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* GoBackAction;
	// ~ ~ Variables
	// ~ Input

};
