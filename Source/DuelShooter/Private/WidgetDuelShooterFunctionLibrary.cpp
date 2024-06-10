// All rights by IsGris


#include "WidgetDuelShooterFunctionLibrary.h"

#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY(DuelShooterWidgetsLog);

UUserWidget* UWidgetDuelShooterFunctionLibrary::InitWidgetInstance(APlayerController* Owner, const TSubclassOf<UUserWidget> Widget, FName WidgetName, const bool& IsVisible)
{
	if (Widget)
	{
		UUserWidget* result = CreateWidget(Owner, Widget, WidgetName);
		if (result)
		{
			result->AddToViewport();
			if (!IsVisible)
				result->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(DuelShooterWidgetsLog, Verbose, TEXT("Widget instance %s successfully inited"), Widget.Get()->GetClass()->GetFName());
			return result;
		}
		else
		{
			UE_LOG(DuelShooterWidgetsLog, Warning, TEXT("Cannot create widget instance of widget %s given in function InitWidgetInstance"),
				*(Widget->GetClass()->GetFName().ToString()));
			return nullptr;
		}
	}
	else
	{
		UE_LOG(DuelShooterWidgetsLog, Warning, TEXT("Widget given in function InitWidgetInstance is not defined"));
		return nullptr;
	}
}

FMargin UWidgetDuelShooterFunctionLibrary::WidgetAbsolutePositionToMargin(const int AbsolutePositionX, const int AbsolutePositionY, const UCanvasPanelSlot* WidgetSlot)
{
	const int ScreenResolutionX = 1920;
	const int ScreenResolutionY = 1080;

	FMargin Result;
	auto Anchors = WidgetSlot->GetAnchors();
	auto Size = WidgetSlot->GetSize();
	auto Offsets = WidgetSlot->GetOffsets();

	bool IsHorizontalOffsetsAvialible = Anchors.Minimum.X != Anchors.Maximum.X;
	bool IsVerticalOffsetsAvialible = Anchors.Minimum.Y != Anchors.Maximum.Y;

	float DistanceFromLeftAnchorWall, DistanceFromRightAnchorWall, DistanceFromTopAnchorWall, DistanceFromBottomAnchorWall;

	float AbsolutePositionXFromRightWall = ScreenResolutionX - AbsolutePositionX;
	float AbsolutePositionYFromBottomWall = ScreenResolutionY - AbsolutePositionY;

	DistanceFromLeftAnchorWall = ScreenResolutionX * Anchors.Minimum.X;
	if (IsHorizontalOffsetsAvialible)
	{
		DistanceFromRightAnchorWall = ScreenResolutionX * (1 - Anchors.Maximum.X);
		Size.X = ScreenResolutionX * (Anchors.Maximum.X - Anchors.Minimum.X) - (Offsets.Left + Offsets.Right);
	}

	DistanceFromTopAnchorWall = ScreenResolutionY * Anchors.Minimum.Y;
	if (IsVerticalOffsetsAvialible)
	{
		DistanceFromBottomAnchorWall = ScreenResolutionY * (1 - Anchors.Maximum.Y);
		Size.Y = ScreenResolutionY * (Anchors.Maximum.Y - Anchors.Minimum.Y) - (Offsets.Top + Offsets.Bottom);
	}

	
	Result.Right = (IsHorizontalOffsetsAvialible ? AbsolutePositionXFromRightWall - Size.X - DistanceFromRightAnchorWall : Size.X);
	Result.Left = AbsolutePositionX - DistanceFromLeftAnchorWall;
	Result.Bottom = (IsVerticalOffsetsAvialible ? AbsolutePositionYFromBottomWall - Size.Y - DistanceFromBottomAnchorWall : Size.Y);
	Result.Top = AbsolutePositionY - DistanceFromTopAnchorWall;
	return Result;
}

FVector2D UWidgetDuelShooterFunctionLibrary::GetAbsoluteWidgetPosition(const UCanvasPanelSlot* WidgetSlot)
{
	auto Offsets = WidgetSlot->GetOffsets();
	auto Anchors = WidgetSlot->GetAnchors();
	return FVector2D(1920 * Anchors.Minimum.X + Offsets.Left, 1080 * Anchors.Minimum.Y + Offsets.Top);
}

bool UWidgetDuelShooterFunctionLibrary::AddOffsetToWidgetByRotator(APlayerController* Owner, FRotator OffsetToAdd, UWidget* Widget)
{
	if (!Owner || !Widget)
		return false;

	// Convert rotator for forward vector function
	OffsetToAdd = { OffsetToAdd.Yaw, OffsetToAdd.Pitch, 0 };

	FVector2D AppendOffset;
	auto bSuccess = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(Owner, Owner->GetPawn()->GetActorLocation() + UKismetMathLibrary::GetForwardVector(Owner->GetPawn()->GetActorRotation() + OffsetToAdd), AppendOffset, false);
	FVector2D PlayerStartOffset;
	bSuccess &= UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(Owner, Owner->GetPawn()->GetActorLocation() + UKismetMathLibrary::GetForwardVector(Owner->GetPawn()->GetActorRotation()), PlayerStartOffset, false);
	if (!bSuccess)
		return false;
	AppendOffset -= PlayerStartOffset;
	return UWidgetDuelShooterFunctionLibrary::AddOffsetToWidgetByPixels(AppendOffset, Widget);
}

bool UWidgetDuelShooterFunctionLibrary::AddOffsetToWidgetByPixels(FVector2D OffsetToAdd, UWidget* Widget)
{
	if (!Widget)
		return false;
	return UWidgetDuelShooterFunctionLibrary::MoveWidgetByPixels(UWidgetDuelShooterFunctionLibrary::GetAbsoluteWidgetPosition(UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget)) + OffsetToAdd, Widget);
}

FVector2D UWidgetDuelShooterFunctionLibrary::GetWidgetSize(UWidget* Widget)
{
	if (!Widget)
		return { 0, 0 };
	FVector2D Size;
	const int ScreenResolutionX = 1920;
	const int ScreenResolutionY = 1080;
	auto WidgetSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget);
	if (!WidgetSlot)
		return { 0, 0 };
	auto Anchors = WidgetSlot->GetAnchors();
	auto Offsets = WidgetSlot->GetOffsets();
	bool IsHorizontalOffsetsAvialible = Anchors.Minimum.X != Anchors.Maximum.X;
	bool IsVerticalOffsetsAvialible = Anchors.Minimum.Y != Anchors.Maximum.Y;
	Size = WidgetSlot->GetSize();
	if (IsHorizontalOffsetsAvialible)
	{
		Size.X = ScreenResolutionX * (Anchors.Maximum.X - Anchors.Minimum.X) - (Offsets.Left + Offsets.Right);
	}
	if (IsVerticalOffsetsAvialible)
	{
		Size.Y = ScreenResolutionY * (Anchors.Maximum.Y - Anchors.Minimum.Y) - (Offsets.Top + Offsets.Bottom);
	}
	return Size;
}

bool UWidgetDuelShooterFunctionLibrary::MoveWidgetByPixels(const FVector2D& NewAbsoluteLocation, UWidget* Widget)
{
	if (!Widget)
		return false;
	auto CanvasPanelSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(Widget);
	if (!CanvasPanelSlot)
		return false;
	CanvasPanelSlot->SetOffsets(UWidgetDuelShooterFunctionLibrary::WidgetAbsolutePositionToMargin(NewAbsoluteLocation.X, NewAbsoluteLocation.Y, CanvasPanelSlot));
	return true;
}