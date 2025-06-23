// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenWaitUntilCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenWaitUntilCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FWaitUntilCommandArgs
	{
		FString VariableKindStr;
		FString VariableName;
		FString Operator;
		FString RightHandValue;
		EShidenVariableKind VariableKind;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FWaitUntilCommandArgs& Args, FString& ErrorMessage);

	static bool TryEvaluateCondition(const FWaitUntilCommandArgs& Args, const FString& ProcessName, bool& bResult, FString& ErrorMessage);

	FWaitUntilCommandArgs Args;
};
