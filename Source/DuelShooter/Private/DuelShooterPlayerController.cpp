// All rights by IsGris


#include "DuelShooterPlayerController.h"

void ADuelShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = false;
}
