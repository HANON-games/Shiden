// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenLoopWhileExpressionCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenLoopWhileExpressionCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FLoopWhileExpressionArgs
	{
		FString Expression;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static bool TryParseCommand(const FString& ProcessName, FLoopWhileExpressionArgs& Args, FString& ErrorMessage);

	static bool TryEvaluateExpressionCondition(const FLoopWhileExpressionArgs& Args, const FString& ProcessName, FString& ErrorMessage);

	FLoopWhileExpressionArgs Args;
};
