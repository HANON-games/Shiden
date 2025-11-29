// Copyright (c) 2025 HANON. All Rights Reserved.

#include "System/ShidenSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Config/ShidenProjectConfig.h"

bool UShidenSubsystem::TickScreenFade(const float DeltaTime)
{
	bool bAnyFading = false;

	for (TPair<FString, FShidenScreenFadeLayer>& Pair : ScreenFadeLayers)
	{
		FShidenScreenFadeLayer& Layer = Pair.Value;

		// Skip if fade is already complete
		if (Layer.Params.EasingAlpha >= 1.0f)
		{
			continue;
		}

		if (!Layer.Widget)
		{
			continue;
		}

		Layer.Params.EasingAlpha += DeltaTime / Layer.Params.FadeDuration;
		
		const float Alpha = FMath::Clamp(Layer.Params.EasingAlpha, 0.0f, 1.0f);

		const float ResultR = UKismetMathLibrary::Ease(Layer.Params.StartColor.R, Layer.Params.TargetColor.R, Alpha, Layer.Params.FadeFunction,
		                                               Layer.Params.BlendExp, Layer.Params.Steps);
		const float ResultG = UKismetMathLibrary::Ease(Layer.Params.StartColor.G, Layer.Params.TargetColor.G, Alpha, Layer.Params.FadeFunction,
		                                               Layer.Params.BlendExp, Layer.Params.Steps);
		const float ResultB = UKismetMathLibrary::Ease(Layer.Params.StartColor.B, Layer.Params.TargetColor.B, Alpha, Layer.Params.FadeFunction,
		                                               Layer.Params.BlendExp, Layer.Params.Steps);
		const float ResultA = UKismetMathLibrary::Ease(Layer.Params.StartColor.A, Layer.Params.TargetColor.A, Alpha, Layer.Params.FadeFunction,
		                                               Layer.Params.BlendExp, Layer.Params.Steps);

		Layer.Widget->SetBrushColor(FLinearColor(ResultR, ResultG, ResultB, ResultA));

		// Check if fade is complete
		if (Alpha >= 1.0f)
		{
			// If fading in, hide the widget
			if (!Layer.Params.bIsFadeOut)
			{
				Layer.Widget->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			bAnyFading = true;
		}
	}

	// Stop ticker when all fades are done
	if (!bAnyFading)
	{
		if (TickerHandle.IsValid())
		{
			FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
			TickerHandle.Reset();
		}
		return false;
	}

	return true; // Continue ticking
}

bool UShidenSubsystem::IsScreenFadeCompleted(const FString& LayerName) const
{
	if (const FShidenScreenFadeLayer* Layer = ScreenFadeLayers.Find(LayerName))
	{
		return Layer->Params.EasingAlpha >= 1.0f;
	}
	return true;
}

void UShidenSubsystem::ResetScreenFadeLayers()
{
	// Stop the ticker if it's running
	if (TickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickerHandle);
		TickerHandle.Reset();
	}

	// Remove all widgets from their parents before clearing
	for (TPair<FString, FShidenScreenFadeLayer>& Pair : ScreenFadeLayers)
	{
		if (Pair.Value.Widget)
		{
			Pair.Value.Widget->SetVisibility(ESlateVisibility::Collapsed);

			const TSharedPtr<SWidget> CachedWidget = Pair.Value.Widget->GetCachedWidget();
			if (CachedWidget.IsValid())
			{
				if (GEngine && GEngine->GameViewport)
				{
					GEngine->GameViewport->RemoveViewportWidgetContent(CachedWidget.ToSharedRef());
				}
			}

			Pair.Value.Widget->RemoveFromParent();
		}
	}

	ScreenFadeLayers.Empty();
}

void UShidenSubsystem::SetDefaultPredefinedSystemVariables()
{
	PredefinedSystemVariable = FShidenPredefinedSystemVariable(GetDefault<UShidenProjectConfig>()->PredefinedSystemVariable);
}

#if WITH_EDITOR
void UShidenSubsystem::BeginPlay(bool)
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
	UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
	LocalVariable = FShidenLocalVariable();
	ScenarioProperties = TMap<FString, FShidenScenarioProperties>();
	ScenarioProgressStack = TMap<FString, FShidenScenarioProgressStack>();
	BacklogItems = TArray<FShidenBacklogItem>();
	ScenarioReadLines = TMap<FGuid, FShidenReadLines>();
	SetDefaultPredefinedSystemVariables();
	ResetScreenFadeLayers();
}
#endif

UShidenSubsystem::UShidenSubsystem()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	SystemVariable = FShidenVariable(ShidenProjectConfig->SystemVariableDefinitions);
	UserVariable = FShidenVariable(ShidenProjectConfig->UserVariableDefinitions);
	LocalVariable = FShidenLocalVariable();
	ScenarioProperties = TMap<FString, FShidenScenarioProperties>();
	ScenarioProgressStack = TMap<FString, FShidenScenarioProgressStack>();
	BacklogItems = TArray<FShidenBacklogItem>();
	ScenarioReadLines = TMap<FGuid, FShidenReadLines>();
	SetDefaultPredefinedSystemVariables();
	ResetScreenFadeLayers();
#if WITH_EDITOR
	FEditorDelegates::PreBeginPIE.AddUObject(this, &UShidenSubsystem::BeginPlay);
#endif
}
