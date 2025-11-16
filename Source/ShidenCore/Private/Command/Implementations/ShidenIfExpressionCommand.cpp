// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenIfExpressionCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Command/Implementations/ShidenIfCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

void UShidenIfExpressionCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
                                                               UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                               FString& NextScenarioName, FString& ErrorMessage)
{
	// HACK: get expression from original command
	FShidenCommand* OriginalCommand = nullptr;
	if (!ShidenCommandHelpers::TryGetCurrentOriginalCommand(ProcessName, OriginalCommand, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	if (!ShidenConditionalCommandHelpers::TryParseExpressionCondition(*OriginalCommand, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryExecuteCommand(Args, ProcessName, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenIfExpressionCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                               EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	// HACK: get expression from original command
	FShidenCommand* OriginalCommand = nullptr;
	if (!ShidenCommandHelpers::TryGetCurrentOriginalCommand(TEXT("Default"), OriginalCommand, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!ShidenConditionalCommandHelpers::TryParseExpressionCondition(*OriginalCommand, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryExecuteCommand(Args, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenIfExpressionCommand::TryExecuteCommand(const ShidenConditionalCommandHelpers::FExpressionConditionArgs& Args, const FString& ProcessName,
                                                   FString& ErrorMessage)
{
	bool bResult;
	if (!ShidenConditionalCommandHelpers::TryEvaluateExpressionCondition(ProcessName, Args, bResult, ErrorMessage))
	{
		return false;
	}

	if (bResult)
	{
		// Condition is true, proceed to next command
		return true;
	}

	// Condition is false, find ElseIfExpression/Else/EndIf
	int32 NextIndex;
	UShidenScenarioBlueprintLibrary::ToNext(ProcessName, NextIndex);

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		return false;
	}

	const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();

	UShidenScenario* Scenario = nullptr;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
	{
		ErrorMessage = TEXT("GetScenario failed.");
		return false;
	}

	FString CommandName;
	int32 ResultIndex;
	if (!ShidenConditionalCommandHelpers::TrySkipToNextIfBranch(ProcessName, ScenarioProgress.CurrentIndex, CommandName, ResultIndex,
	                                                                    ErrorMessage))
	{
		return false;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, ResultIndex);

	if (CommandName == TEXT("ElseIfExpression"))
	{
		ShidenConditionalCommandHelpers::FExpressionConditionArgs NewArgs;
		if (!ShidenConditionalCommandHelpers::TryParseExpressionCondition(Scenario->Commands[ResultIndex], NewArgs, ErrorMessage))
		{
			return false;
		}

		return TryExecuteCommand(NewArgs, ProcessName, ErrorMessage);
	}

	if (CommandName == TEXT("ElseIf"))
	{
		return TryExecuteElseIf(ProcessName, ResultIndex, Scenario, ErrorMessage);
	}

	// Else, EndIf are handled by their respective commands or naturally proceed
	return true;
}

bool UShidenIfExpressionCommand::TryExecuteElseIf(const FString& ProcessName, const int32 ResultIndex, UShidenScenario* Scenario, FString& ErrorMessage)
{
	FShidenCommand ConstructedCommand;
	UShidenScenarioBlueprintLibrary::ConstructCommand(ProcessName, Scenario->Commands[ResultIndex], ConstructedCommand);

	ShidenConditionalCommandHelpers::FVariableConditionArgs VariableArgs;
	if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(ConstructedCommand, VariableArgs, ErrorMessage))
	{
		return false;
	}

	bool bResult;
	if (!ShidenConditionalCommandHelpers::TryEvaluateVariableCondition(ProcessName, VariableArgs, bResult, ErrorMessage))
	{
		return false;
	}

	if (bResult)
	{
		return true;
	}

	// Condition is false, find next conditional
	int32 NextIndex;
	UShidenScenarioBlueprintLibrary::ToNext(ProcessName, NextIndex);

	FString CommandName;
	int32 NextResultIndex;
	if (!ShidenConditionalCommandHelpers::TrySkipToNextIfBranch(ProcessName, NextIndex, CommandName, NextResultIndex, ErrorMessage))
	{
		return false;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, NextResultIndex);

	if (CommandName == TEXT("ElseIfExpression"))
	{
		ShidenConditionalCommandHelpers::FExpressionConditionArgs NewArgs;
		if (!ShidenConditionalCommandHelpers::TryParseExpressionCondition(Scenario->Commands[NextResultIndex], NewArgs, ErrorMessage))
		{
			return false;
		}

		return TryExecuteCommand(NewArgs, ProcessName, ErrorMessage);
	}

	if (CommandName == TEXT("ElseIf"))
	{
		return TryExecuteElseIf(ProcessName, NextResultIndex, Scenario, ErrorMessage);
	}

	// Else, EndIf are handled by their respective commands or naturally proceed
	return true;
}
