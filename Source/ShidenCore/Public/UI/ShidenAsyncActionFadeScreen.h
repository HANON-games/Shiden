// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "ShidenAsyncActionFadeScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeScreenCompleted);

/**
 * Async action for fading the screen out to a specified color.
 * Provides OnStarted (fires immediately) and OnCompleted (fires when fade finishes) pins.
 */
UCLASS()
class SHIDENCORE_API UShidenAsyncActionFadeOutScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Fades the screen out to a specified color.
	 * This function uses a global fade widget and does not require ShidenWidget.
	 *
	 * @param WorldContextObject World context object
	 * @param LayerName The name of the fade layer
	 * @param FadeDuration The duration of the fade effect in seconds
	 * @param FadeFunction The easing function type for the fade animation
	 * @param TargetColor The color to fade to
	 * @param Steps The number of animation steps for precision
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param ZOrder The Z-order for the fade widget
	 * @param bSaveScenarioProperty Whether to save the scenario property
	 * @return The async action object
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Fade", WorldContext = "WorldContextObject", LayerName = "Default", bSaveScenarioProperty = "true"))
	static UShidenAsyncActionFadeOutScreen* FadeOutScreen(UObject* WorldContextObject, const FString& LayerName, float FadeDuration,
	                                                      EEasingFunc::Type FadeFunction, FLinearColor TargetColor, int32 Steps = 2,
	                                                      float BlendExp = 2.0f, int32 ZOrder = 250, bool bSaveScenarioProperty = true);

	virtual void Activate() override;

	/** Fires when the fade animation completes */
	UPROPERTY(BlueprintAssignable)
	FOnFadeScreenCompleted OnCompleted;

protected:
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;

	FString LayerName;
	float FadeDuration;
	EEasingFunc::Type FadeFunction;
	FLinearColor TargetColor;
	int32 Steps;
	float BlendExp;
	int32 ZOrder;
	bool bSaveScenarioProperty;

	FTimerHandle TimerHandle;

	void CheckCompletion();
};

/**
 * Async action for fading the screen in from the current fade color.
 * Provides OnStarted (fires immediately) and OnCompleted (fires when fade finishes) pins.
 */
UCLASS()
class SHIDENCORE_API UShidenAsyncActionFadeInScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Fades the screen in from the current fade color.
	 * This function uses a global fade widget and does not require ShidenWidget.
	 *
	 * @param WorldContextObject World context object
	 * @param LayerName The name of the fade layer
	 * @param FadeDuration The duration of the fade effect in seconds
	 * @param FadeFunction The easing function type for the fade animation
	 * @param TargetColor The color to fade from, used when creating new widget
	 * @param Steps The number of animation steps for precision
	 * @param BlendExp The blend exponent for certain easing functions
	 * @param ZOrder The Z-order for the fade widget
	 * @param bSaveScenarioProperty Whether to save the scenario property
	 * @return The async action object
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Fade", WorldContext = "WorldContextObject", LayerName = "Default", bSaveScenarioProperty = "true"))
	static UShidenAsyncActionFadeInScreen* FadeInScreen(UObject* WorldContextObject, const FString& LayerName, float FadeDuration,
	                                                    EEasingFunc::Type FadeFunction, FLinearColor TargetColor, int32 Steps = 2,
	                                                    float BlendExp = 2.0f, int32 ZOrder = 250, bool bSaveScenarioProperty = true);

	virtual void Activate() override;

	/** Fires when the fade animation completes */
	UPROPERTY(BlueprintAssignable)
	FOnFadeScreenCompleted OnCompleted;

protected:
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject;

	FString LayerName;
	float FadeDuration;
	EEasingFunc::Type FadeFunction;
	FLinearColor TargetColor;
	int32 Steps;
	float BlendExp;
	int32 ZOrder;
	bool bSaveScenarioProperty;

	FTimerHandle TimerHandle;

	void CheckCompletion();
};
