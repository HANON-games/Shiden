// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenAsyncActionFadeScreen.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShidenAsyncActionFadeScreen)

// UShidenAsyncActionFadeOutScreen

UShidenAsyncActionFadeOutScreen* UShidenAsyncActionFadeOutScreen::FadeOutScreen(UObject* WorldContextObject, const FString& LayerName, const float FadeDuration,
                                                                                const EEasingFunc::Type FadeFunction, const FLinearColor TargetColor, const int32 Steps,
                                                                                const float BlendExp, const int32 ZOrder, const bool bSaveScenarioProperty)
{
	const TObjectPtr<UShidenAsyncActionFadeOutScreen> Action = NewObject<UShidenAsyncActionFadeOutScreen>();
	Action->WorldContextObject = WorldContextObject;
	Action->LayerName = LayerName;
	Action->FadeDuration = FadeDuration;
	Action->FadeFunction = FadeFunction;
	Action->TargetColor = TargetColor;
	Action->Steps = Steps;
	Action->BlendExp = BlendExp;
	Action->ZOrder = ZOrder;
	Action->bSaveScenarioProperty = bSaveScenarioProperty;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionFadeOutScreen::Activate()
{
	// Start the fade
	if (!UShidenBlueprintLibrary::TryStartScreenFade(WorldContextObject, LayerName, FadeDuration, FadeFunction, TargetColor, true, Steps, BlendExp, ZOrder, bSaveScenarioProperty))
	{
		SetReadyToDestroy();
		return;
	}

	// If fade is instant (Duration == 0), complete immediately
	if (UShidenBlueprintLibrary::IsScreenFadeCompleted(LayerName))
	{
		OnCompleted.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// Set up a timer to check for completion
	if (const TObjectPtr<UWorld> World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr)
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UShidenAsyncActionFadeOutScreen::CheckCompletion, 0.016f, true);
	}
	else
	{
		// No world available, complete immediately
		OnCompleted.Broadcast();
		SetReadyToDestroy();
	}
}

void UShidenAsyncActionFadeOutScreen::CheckCompletion()
{
	if (UShidenBlueprintLibrary::IsScreenFadeCompleted(LayerName))
	{
		// Stop the timer
		if (const TObjectPtr<UWorld> World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}

		OnCompleted.Broadcast();
		SetReadyToDestroy();
	}
}

// UShidenAsyncActionFadeInScreen

UShidenAsyncActionFadeInScreen* UShidenAsyncActionFadeInScreen::FadeInScreen(UObject* WorldContextObject, const FString& LayerName, const float FadeDuration,
                                                                             const EEasingFunc::Type FadeFunction, const FLinearColor TargetColor, const int32 Steps,
                                                                             const float BlendExp, const int32 ZOrder, const bool bSaveScenarioProperty)
{
	const TObjectPtr<UShidenAsyncActionFadeInScreen> Action = NewObject<UShidenAsyncActionFadeInScreen>();
	Action->WorldContextObject = WorldContextObject;
	Action->LayerName = LayerName;
	Action->FadeDuration = FadeDuration;
	Action->FadeFunction = FadeFunction;
	Action->TargetColor = TargetColor;
	Action->Steps = Steps;
	Action->BlendExp = BlendExp;
	Action->ZOrder = ZOrder;
	Action->bSaveScenarioProperty = bSaveScenarioProperty;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionFadeInScreen::Activate()
{
	// Start the fade
	if (!UShidenBlueprintLibrary::TryStartScreenFade(WorldContextObject, LayerName, FadeDuration, FadeFunction, TargetColor, false, Steps, BlendExp, ZOrder, bSaveScenarioProperty))
	{
		SetReadyToDestroy();
		return;
	}

	// If fade is instant (Duration == 0), complete immediately
	if (UShidenBlueprintLibrary::IsScreenFadeCompleted(LayerName))
	{
		OnCompleted.Broadcast();
		SetReadyToDestroy();
		return;
	}

	// Set up a timer to check for completion
	if (const TObjectPtr<UWorld> World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr)
	{
		World->GetTimerManager().SetTimer(TimerHandle, this, &UShidenAsyncActionFadeInScreen::CheckCompletion, 0.016f, true);
	}
	else
	{
		// No world available, complete immediately
		OnCompleted.Broadcast();
		SetReadyToDestroy();
	}
}

void UShidenAsyncActionFadeInScreen::CheckCompletion()
{
	if (UShidenBlueprintLibrary::IsScreenFadeCompleted(LayerName))
	{
		// Stop the timer
		if (const TObjectPtr<UWorld> World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr)
		{
			World->GetTimerManager().ClearTimer(TimerHandle);
		}

		OnCompleted.Broadcast();
		SetReadyToDestroy();
	}
}
