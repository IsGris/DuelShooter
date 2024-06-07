// All rights by IsGris

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WidgetDuelShooterFunctionLibrary.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(DuelShooterWidgetsLog, Log, All);

class UCanvasPanelSlot;
class UWidget;

/**
 * 
 */
UCLASS()
class DUELSHOOTER_API UWidgetDuelShooterFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	* Init widget from type
	* @param Owner Who own this widget
	* @param Widget Type of widget
	* @param WidgetName Name of widget
	* @param IsVisible Does widget visible on create
	* @return If widget created return widget otherwise return nullptr
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Initialize", meta = (KeyWords = "Create"))
	static UUserWidget* InitWidgetInstance(APlayerController* Owner, const TSubclassOf<UUserWidget> Widget, FName WidgetName = NAME_None, const bool& IsVisible = true);
	/*
	* Converts absolute position of widget(starting from left up corner as 0) to margin of certain widget
	* @param WidgetSlot Canvas panel slot of widget margin we looking for
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position", meta = (KeyWords = "Convert"))
	static FMargin WidgetAbsolutePositionToMargin(const int AbsolutePositionX, const int AbsolutePositionY, const UCanvasPanelSlot* WidgetSlot);

	/*
	* Calculates the widget's position from the top left edge of the screen
	* @param WidgetSlot Canvas panel slot of widget
	* @return Widget position where X is the deviation from the left edge of the screen and Y is the deviation from the top edge of the screen
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position")
	static FVector2D GetAbsoluteWidgetPosition(const UCanvasPanelSlot* WidgetSlot);

	/*
	* Adds rotation to the widget using its owner
	* @param Owner Owner of widget that we will use for calculate new widget position
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position")
	static bool AddOffsetToWidgetByRotator(APlayerController* Owner, FRotator OffsetToAdd, UWidget* Widget);

	/*
	* Adds pixels to widget position
	* @param OffsetToAdd How many pixels add to widget position
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position")
	static bool AddOffsetToWidgetByPixels(FVector2D OffsetToAdd, UWidget* Widget);

	/*
	* Get widget size in pixels
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position")
	static FVector2D GetWidgetSize(UWidget* Widget);

	/*
	* Move the widget's position from the top left edge of the screen
	* @param Widget Canvas panel slot of widget
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position")
	static bool MoveWidgetByPixels(const FVector2D& NewAbsoluteLocation, UWidget* Widget);

	/*
	* Move the widget's position to rotator
	* @param Widget Canvas panel slot of widget
	*/
	UFUNCTION(BlueprintPure, Category = "Widgets|Position")
	static bool MoveWidgetByRotator(APlayerController* Owner, FRotator NewWidgetLocation, UWidget* Widget);
};
