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

FVector ADuelShooterPlayerController::GetLookVelocity() const
{
	if (bSaveLookVelocity)
		return LookVelocity;
	else
		return FVector();
}

void ADuelShooterPlayerController::AddPitchInput(float Val)
{
	Super::AddPitchInput(Val);
	if (bSaveLookVelocity)
	{
		const auto ValueToAdd = !IsLookInputIgnored() ? Val * (GetDefault<UInputSettings>()->bEnableLegacyInputScales ? InputPitchScale_DEPRECATED : 1.0f) : 0.0f;
		const auto LookVelocityRef = &LookVelocity;
		LookVelocity.X += ValueToAdd;
		FTimerHandle TempTimerHandle;
		FTimerDelegate TempTimerCallback;
		TempTimerCallback.BindLambda([=]()
									 {
										 (*LookVelocityRef).X -= ValueToAdd;
									 });
		GetWorldTimerManager().SetTimer(TempTimerHandle, TempTimerCallback, 0.00001f, false, 0.00001f);
	}
}

void ADuelShooterPlayerController::AddYawInput(float Val)
{
	Super::AddYawInput(Val);
	if (bSaveLookVelocity)
	{
		const auto ValueToAdd = !IsLookInputIgnored() ? Val * (GetDefault<UInputSettings>()->bEnableLegacyInputScales ? InputYawScale_DEPRECATED : 1.0f) : 0.0f;
		const auto LookVelocityRef = &LookVelocity;
		LookVelocity.Y = ValueToAdd;
		FTimerHandle TempTimerHandle;
		FTimerDelegate TempTimerCallback;
		TempTimerCallback.BindLambda([=]()
									 {
										 (*LookVelocityRef).Y -= ValueToAdd;
									 });
		GetWorldTimerManager().SetTimer(TempTimerHandle, TempTimerCallback, 0.00001f, false, 0.00001f);
	}
}

void ADuelShooterPlayerController::AddRollInput(float Val)
{
	Super::AddRollInput(Val);
	if (bSaveLookVelocity)
	{
		const auto ValueToAdd = !IsLookInputIgnored() ? Val * (GetDefault<UInputSettings>()->bEnableLegacyInputScales ? InputRollScale_DEPRECATED : 1.0f) : 0.0f;
		const auto LookVelocityRef = &LookVelocity;
		LookVelocity.Z += ValueToAdd;
		FTimerHandle TempTimerHandle;
		FTimerDelegate TempTimerCallback;
		TempTimerCallback.BindLambda([=]()
									 {
										 (*LookVelocityRef).Z -= ValueToAdd;
									 });
		GetWorldTimerManager().SetTimer(TempTimerHandle, TempTimerCallback, 0.00001f, false, 0.00001f);
	}
}
