// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "ShidenForceFeedbackCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenForceFeedbackCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FForceFeedbackCommandArgs
	{
		FString FeedbackEffectPath;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FForceFeedbackCommandArgs& Args);

	static bool TryPlayForceFeedback(const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                 const FForceFeedbackCommandArgs& Args, FString& ErrorMessage);

	FForceFeedbackCommandArgs Args;
};
