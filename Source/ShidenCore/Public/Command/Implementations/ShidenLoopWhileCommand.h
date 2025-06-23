// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenario.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenLoopWhileCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenLoopWhileCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FLoopWhileCommandArgs
	{
		EShidenVariableKind VariableKind;
		FString VariableName;
		FString Operator;
		FString RightHandValue;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FLoopWhileCommandArgs& Args, FString& ErrorMessage);

	static bool TryEvaluateCondition(const FLoopWhileCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage);

	static bool TryFindEndLoopWhileIndex(const UShidenScenario* Scenario, int32 StartIndex, int32& ResultIndex, FString& ErrorMessage);

	FLoopWhileCommandArgs Args;
};
