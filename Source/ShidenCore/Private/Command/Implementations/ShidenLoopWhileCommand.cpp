// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenLoopWhileCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

void UShidenLoopWhileCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
                                                            UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                            FString& NextScenarioName, FString& ErrorMessage)
{
	if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryEvaluateCondition(Args, ProcessName, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

bool UShidenLoopWhileCommand::TryEvaluateCondition(const ShidenConditionalCommandHelpers::FVariableConditionArgs& Args, const FString& ProcessName, FString& ErrorMessage)
{
	bool bResult = false;
	if (!ShidenConditionalCommandHelpers::TryEvaluateVariableCondition(ProcessName, Args, bResult, ErrorMessage))
	{
		return false;
	}

	if (bResult)
	{
		return true;
	}

	// Condition is false, find EndLoopWhile and jump to it
	int32 NextIndex;
	UShidenScenarioBlueprintLibrary::ToNext(ProcessName, NextIndex);

	int32 ResultIndex;
	if (!ShidenConditionalCommandHelpers::TryFindEndLoopWhileIndex(ProcessName, NextIndex, ResultIndex, ErrorMessage))
	{
		return false;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, ResultIndex);
	return true;
}
