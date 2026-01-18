// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenIfCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Command/Implementations/ShidenIfExpressionCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

void UShidenIfCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
                                                     UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                     FString& NextScenarioName, FString& ErrorMessage)
{
	if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryExecuteCommand(ProcessName, Args, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenIfCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                     bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryExecuteCommand(TEXT("Default"), Args, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenIfCommand::TryExecuteCommand(const FString& ProcessName, const ShidenConditionalCommandHelpers::FVariableConditionArgs& Args, FString& ErrorMessage)
{
	bool bResult;
	if (!ShidenConditionalCommandHelpers::TryEvaluateVariableCondition(ProcessName, Args, bResult, ErrorMessage))
	{
		return false;
	}

	if (bResult)
	{
		return true;
	}

	int32 NextIndex;
	UShidenScenarioBlueprintLibrary::ToNext(ProcessName, NextIndex);

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

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
	if (!ShidenConditionalCommandHelpers::TrySkipToNextIfBranch(ProcessName, ScenarioProgress.CurrentIndex, CommandName, ResultIndex, ErrorMessage))
	{
		return false;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, ResultIndex);

	if (CommandName == TEXT("ElseIf"))
	{
		if (!Scenario->Commands.IsValidIndex(ResultIndex))
		{
			ErrorMessage = FString::Printf(TEXT("ResultIndex %d is out of bounds for Commands array (size: %d)."), ResultIndex, Scenario->Commands.Num());
			return false;
		}

		FShidenCommand ConstructedCommand;
		UShidenScenarioBlueprintLibrary::ConstructCommand(ProcessName, Scenario->Commands[ResultIndex], ConstructedCommand);

		ShidenConditionalCommandHelpers::FVariableConditionArgs NewArgs;
		if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(ConstructedCommand, NewArgs, ErrorMessage))
		{
			return false;
		}

		return TryExecuteCommand(ProcessName, NewArgs, ErrorMessage);
	}

	if (CommandName == TEXT("ElseIfExpression"))
	{
		return TryExecuteElseIfExpression(ProcessName, ResultIndex, Scenario, ErrorMessage);
	}

	// Else, EndIf are handled by their respective commands or naturally proceed
	return true;
}

bool UShidenIfCommand::TryExecuteElseIfExpression(const FString& ProcessName, const int32 ResultIndex, UShidenScenario* Scenario, FString& ErrorMessage)
{
	if (!Scenario->Commands.IsValidIndex(ResultIndex))
	{
		ErrorMessage = FString::Printf(TEXT("ResultIndex %d is out of bounds for Commands array (size: %d)."), ResultIndex, Scenario->Commands.Num());
		return false;
	}

	ShidenConditionalCommandHelpers::FExpressionConditionArgs ExpressionArgs;
	if (!ShidenConditionalCommandHelpers::TryParseExpressionCondition(Scenario->Commands[ResultIndex], ExpressionArgs, ErrorMessage))
	{
		return false;
	}

	bool bResult;
	if (!ShidenConditionalCommandHelpers::TryEvaluateExpressionCondition(ProcessName, ExpressionArgs, bResult, ErrorMessage))
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

	if (CommandName == TEXT("ElseIf"))
	{
		if (!Scenario->Commands.IsValidIndex(NextResultIndex))
		{
			ErrorMessage = FString::Printf(TEXT("NextResultIndex %d is out of bounds for Commands array (size: %d)."), NextResultIndex, Scenario->Commands.Num());
			return false;
		}

		FShidenCommand ConstructedCommand;
		UShidenScenarioBlueprintLibrary::ConstructCommand(ProcessName, Scenario->Commands[NextResultIndex], ConstructedCommand);

		ShidenConditionalCommandHelpers::FVariableConditionArgs NewArgs;
		if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(ConstructedCommand, NewArgs, ErrorMessage))
		{
			return false;
		}

		return TryExecuteCommand(ProcessName, NewArgs, ErrorMessage);
	}

	if (CommandName == TEXT("ElseIfExpression"))
	{
		return TryExecuteElseIfExpression(ProcessName, NextResultIndex, Scenario, ErrorMessage);
	}

	// Else, EndIf are handled by their respective commands or naturally proceed
	return true;
}
