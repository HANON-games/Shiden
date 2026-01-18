// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenAssignExpressionCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenAssignExpressionCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FAssignExpressionCommandArgs
	{
		EShidenVariableKind VariableKind;
		FString VariableName;
		FString Expression;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
	                                           UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FAssignExpressionCommandArgs& Args, FString& ErrorMessage);

	static bool TryEvaluateAndAssign(const UObject* WorldContextObject, const FString& ProcessName, const FAssignExpressionCommandArgs& Args,
	                                 FString& ErrorMessage);

	FAssignExpressionCommandArgs Args;
};
