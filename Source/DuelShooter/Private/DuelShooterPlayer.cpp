// All rights by IsGris


#include "DuelShooterPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Misc/DateTime.h"
#include "ShootComponent.h"
#include "Camera/CameraComponent.h"
#include "DuelShooterPlayerController.h"

DEFINE_LOG_CATEGORY( DuelShooterPlayerLog );

ADuelShooterPlayer::ADuelShooterPlayer()
{
    ShootComponent = CreateDefaultSubobject<UShootComponent>(TEXT("ShootComponent"));
    CameraComponent = CreateDefaultSubobject<UCameraComponent>( TEXT( "CameraComponent" ) );
    CameraComponent->SetupAttachment( RootComponent );
}

void ADuelShooterPlayer::BeginPlay()
{
    Super::BeginPlay();

    PrimaryActorTick.bCanEverTick = false;

    PlayerController = Cast<ADuelShooterPlayerController>(GetController());
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
        if ( ShootSwapAction )
        {
            PlayerEnhancedInputComponent->BindAction(ShootSwapAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::SwapShootingState );
        } else
            UE_LOG( DuelShooterPlayerLog, Warning, TEXT( "ShootSwap input action is not defined" ) );

        if (MoveAction)
        {
            PlayerEnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::MakeMove);
        }
        else
            UE_LOG(DuelShooterPlayerLog, Warning, TEXT("MoveAction input action is not defined"));

        if (LookAction)
        {
            PlayerEnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::MakeLook);
        }
        else
            UE_LOG(DuelShooterPlayerLog, Warning, TEXT("LookAction input action is not defined"));

        if (GoBackAction)
        {
            PlayerEnhancedInputComponent->BindAction(GoBackAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::GoBack);
        }
        else
            UE_LOG(DuelShooterPlayerLog, Warning, TEXT("GoBackAction input action is not defined"));

        if (CrouchAction)
        {
            PlayerEnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::MakeCrouch );
        }
        else
            UE_LOG(DuelShooterPlayerLog, Warning, TEXT("CrouchAction input action is not defined"));

        if (UnCrouchAction)
        {
            PlayerEnhancedInputComponent->BindAction(UnCrouchAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::MakeUnCrouch );
        }
        else
            UE_LOG(DuelShooterPlayerLog, Warning, TEXT("UnCrouchAction input action is not defined"));

        if (JumpAction)
        {
            PlayerEnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ADuelShooterPlayer::Jump);
        }
        else
            UE_LOG(DuelShooterPlayerLog, Warning, TEXT("JumpAction input action is not defined"));

        UE_LOG( DuelShooterPlayerLog, Log, TEXT( "Input actions successfully binded" ) );
    } else
        UE_LOG( DuelShooterPlayerLog, Error, TEXT( "Cannot bind input actions: Failed to get UEnhancedInputComponent" ) );
}

void ADuelShooterPlayer::SwapShootingState()
{
    UE_LOG( DuelShooterPlayerLog, Log, TEXT( "Swap shooting state" ) );
    if ( ShootComponent->IsShooting() )
		ShootComponent->EndShooting();
	else
		ShootComponent->StartShooting();
}

void ADuelShooterPlayer::MakeLook(const FInputActionValue& ActionValue)
{
    PlayerController->AddYawInput(ActionValue.Get<FVector2D>().X);
    PlayerController->AddPitchInput(ActionValue.Get<FVector2D>().Y);
}

void ADuelShooterPlayer::MakeMove(const FInputActionValue& ActionValue)
{
    AddMovementInput(GetActorForwardVector(), ActionValue.Get<FVector2D>().X);
    AddMovementInput(GetActorRightVector(), ActionValue.Get<FVector2D>().Y);
}

void ADuelShooterPlayer::MakeCrouch()
{
    Crouch();
}

void ADuelShooterPlayer::MakeUnCrouch()
{
    UnCrouch();
}

void ADuelShooterPlayer::GoBack()
{
    // TODO: Do something like escape menu
}


