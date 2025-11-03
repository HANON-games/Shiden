// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenIfCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenIfCommand::TryParseCommand(const FShidenCommand& Command, FIfCommandArgs& Args, FString& ErrorMessage)
{
	const FString VariableKindStr = Command.GetArg(TEXT("VariableKind"));
	Args.VariableName = Command.GetArg(TEXT("VariableName"));
	Args.Operator = Command.GetArg(TEXT("Operator"));
	Args.RightHandValue = Command.GetArg(TEXT("RightHandValue"));

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(VariableKindStr, Args.VariableKind))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *VariableKindStr);
		return false;
	}

	return true;
}

void UShidenIfCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
                                                     UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                     FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryExecuteCommand(Args, ProcessName, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenIfCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                     bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryExecuteCommand(Args, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenIfCommand::TryEvaluateCondition(const FIfCommandArgs& Args, const FString& ProcessName, bool& bResult, FString& ErrorMessage)
{
	EShidenVariableType VariableType;
	bool bBooleanValue;
	FString StringValue;
	int32 IntegerValue;
	float FloatValue;
	FVector2D Vector2Value;
	FVector Vector3Value;

	if (!UShidenVariableBlueprintLibrary::TryFindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType, bBooleanValue,
												  StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, ErrorMessage))
	{
		return false;
	}

	switch (VariableType)
	{
	case EShidenVariableType::Boolean:
		{
			const bool bRightHandValue = Args.RightHandValue.ToBool();
			return UShidenVariableBlueprintLibrary::TryEvaluateBoolean(Args.Operator, bBooleanValue, bRightHandValue, bResult, ErrorMessage);
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			return UShidenVariableBlueprintLibrary::TryEvaluateString(Args.Operator, StringValue, Args.RightHandValue, bResult, ErrorMessage);
		}
	case EShidenVariableType::Integer:
		{
			const int32 RightHandValue = FCString::Atoi(*Args.RightHandValue);
			return UShidenVariableBlueprintLibrary::TryEvaluateInteger(Args.Operator, IntegerValue, RightHandValue, bResult, ErrorMessage);
		}
	case EShidenVariableType::Float:
		{
			const float RightHandValue = FCString::Atof(*Args.RightHandValue);
			return UShidenVariableBlueprintLibrary::TryEvaluateFloat(Args.Operator, FloatValue, RightHandValue, bResult, ErrorMessage);
		}
	case EShidenVariableType::Vector2:
		{
			FVector2D RightHandValue;
			if (!RightHandValue.InitFromString(Args.RightHandValue))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Args.RightHandValue);
				return false;
			}
			return UShidenVariableBlueprintLibrary::TryEvaluateVector2(Args.Operator, Vector2Value, RightHandValue, bResult, ErrorMessage);
		}
	case EShidenVariableType::Vector3:
		{
			FVector RightHandValue;
			if (!RightHandValue.InitFromString(Args.RightHandValue))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector."), *Args.RightHandValue);
				return false;
			}
			return UShidenVariableBlueprintLibrary::TryEvaluateVector3(Args.Operator, Vector3Value, RightHandValue, bResult, ErrorMessage);
		}
	}

	return false;
}

bool UShidenIfCommand::TryExecuteCommand(const FIfCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage)
{
	bool bResult;
	if (!TryEvaluateCondition(Args, ProcessName, bResult, ErrorMessage))
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
	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].IsEmpty())
	{
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		return false;
	}

	const FShidenScenarioProgress ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();

	UShidenScenario* Scenario = nullptr;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
	{
		ErrorMessage = TEXT("GetScenario failed.");
		return false;
	}

	FString CommandName;
	int32 ResultIndex;
	if (!TryFindNextElseIfOrElseOrEndIf(ScenarioProgress.CurrentIndex, Scenario, CommandName, ResultIndex, ErrorMessage))
	{
		return false;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, ResultIndex);

	if (CommandName == TEXT("ElseIf"))
	{
		FShidenCommand ConstructedCommand;
		UShidenScenarioBlueprintLibrary::ConstructCommand(ProcessName, Scenario->Commands[ResultIndex], ConstructedCommand);

		FIfCommandArgs NewArgs;
		if (!TryParseCommand(ConstructedCommand, NewArgs, ErrorMessage))
		{
			return false;
		}

		return TryExecuteCommand(NewArgs, ProcessName, ErrorMessage);
	}

	return true;
}

bool UShidenIfCommand::TryFindNextElseIfOrElseOrEndIf(const int32 StartIndex, const UShidenScenario* Scenario, FString& CommandName, int32& ResultIndex,
	                                                  FString& ErrorMessage)
{
	int32 Depth = 0;
	for (int32 Index = StartIndex; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& Command = Scenario->Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}

		if (Command.CommandName == TEXT("If"))
		{
			Depth++;
		}
		else if (Command.CommandName == TEXT("ElseIf") || Command.CommandName == TEXT("Else"))
		{
			if (Depth == 0)
			{
				CommandName = Command.CommandName;
				ResultIndex = Index;
				return true;
			}
		}
		else if (Command.CommandName == TEXT("EndIf"))
		{
			if (Depth == 0)
			{
				CommandName = Command.CommandName;
				ResultIndex = Index;
				return true;
			}
			Depth--;
		}
	}

	ErrorMessage = TEXT("Failed to find ElseIf or Else or EndIf command.");
	return false;
}
