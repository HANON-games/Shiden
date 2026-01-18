// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Command/ShidenCommandHelpers.h"
#include "ShidenIfExpressionCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenIfExpressionCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryExecuteCommand(const ShidenConditionalCommandHelpers::FExpressionConditionArgs& Args, const FString& ProcessName, FString& ErrorMessage);

	static bool TryExecuteElseIf(const FString& ProcessName, int32 ResultIndex, UShidenScenario* Scenario, FString& ErrorMessage);

	ShidenConditionalCommandHelpers::FExpressionConditionArgs Args;
};
