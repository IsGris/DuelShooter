// All rights by IsGris


#include "DuelShooterPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Misc/DateTime.h"
#include "ShootComponent.h"

DEFINE_LOG_CATEGORY( DuelShooterPlayerLog );

ADuelShooterPlayer::ADuelShooterPlayer()
{
   ShootComponent = CreateDefaultSubobject<UShootComponent>( TEXT( "ShootComponent" ) );
}

void ADuelShooterPlayer::BeginPlay()
{
    Super::BeginPlay();

    PrimaryActorTick.bCanEverTick = false;
}

void ADuelShooterPlayer::PawnClientRestart()
{
	Super::PawnClientRestart();

    if ( APlayerController* PC = Cast<APlayerController>( GetController() ) )
    {
        // Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
        if ( UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>( PC->GetLocalPlayer() ) )
        {
            // PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
            Subsystem->ClearAllMappings();

            // Add each mapping context, along with their priority values. Higher values outprioritize lower values.
            Subsystem->AddMappingContext( PlayerInputs, this->InputPriority );

            UE_LOG( DuelShooterPlayerLog, Log, TEXT( "Input mapping contexts added" ) );
        } else
            UE_LOG( DuelShooterPlayerLog, Error, TEXT( "Cannot add input mapping contexts: Failed to get UEnhancedInputLocalPlayerSubsystem" ) );
    } else
        UE_LOG( DuelShooterPlayerLog, Error, TEXT( "Cannot add input mapping contexts: Failed to get player controller" ) );
}

void ADuelShooterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

    if ( UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>( PlayerInputComponent ) )
    {
        // There are ways to bind a UInputAction* to a handler function and multiple types of ETriggerEvent that may be of interest.

        // This calls the handler function on the tick when MyInputAction starts, such as when pressing an action button.
        if ( ShootSwap )
        {
            PlayerEnhancedInputComponent->BindAction( ShootSwap, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::SwapShootingState );
        } else
            UE_LOG( DuelShooterPlayerLog, Warning, TEXT( "ShootSwap input action is not defined" ) );

        UE_LOG( DuelShooterPlayerLog, Log, TEXT( "Input actions successfully binded" ) );
    } else
        UE_LOG( DuelShooterPlayerLog, Error, TEXT( "Cannot bind input actions: Failed to get UEnhancedInputComponent" ) );
}

void ADuelShooterPlayer::SwapShootingState()
{
    if ( ShootComponent->IsShooting() )
		ShootComponent->EndShooting();
	else
		ShootComponent->StartShooting();
}


