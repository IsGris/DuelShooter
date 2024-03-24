// All rights by IsGris


#include "DuelShooterPlayerController.h"
#include "DuelShooterPlayer.h"
#include "Components/CapsuleComponent.h"

void ADuelShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;
	
	PawnOwner = Cast<ADuelShooterPlayer>(GetPawn());
}
