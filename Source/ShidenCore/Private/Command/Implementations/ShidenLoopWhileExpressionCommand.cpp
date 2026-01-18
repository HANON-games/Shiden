// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenLoopWhileExpressionCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Expression/ShidenExpressionBlueprintLibrary.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenLoopWhileExpressionCommand::TryParseCommand(const FString& ProcessName, FLoopWhileExpressionArgs& Args, FString& ErrorMessage)
{
	// HACK: Get the original command to access the arguments
	FShidenCommand* OriginalCommand = nullptr;
	if (!ShidenCommandHelpers::TryGetCurrentOriginalCommand(ProcessName, OriginalCommand, ErrorMessage))
	{
		return false;
	}

	Args.Expression = OriginalCommand->GetArg(TEXT("Expression"));

	if (Args.Expression.IsEmpty())
	{
		ErrorMessage = TEXT("Expression is empty");
		return false;
	}

	return true;
}

void UShidenLoopWhileExpressionCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                      UShidenWidget* ShidenWidget,
                                                                      const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                      const float DeltaTime,
                                                                      UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                                      FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(ProcessName, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryEvaluateExpressionCondition(Args, ProcessName, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

bool UShidenLoopWhileExpressionCommand::TryEvaluateExpressionCondition(const FLoopWhileExpressionArgs& Args, const FString& ProcessName,
                                                                       FString& ErrorMessage)
{
	bool bResult = false;
	if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionBoolean(ProcessName, Args.Expression, bResult, ErrorMessage))
	{
		return false;
	}

	if (bResult)
	{
		return true;
	}

	// Condition is false, find EndLoopWhile
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
