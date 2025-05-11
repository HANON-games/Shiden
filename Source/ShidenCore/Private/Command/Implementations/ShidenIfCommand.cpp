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

	bool bSuccess;
	UShidenVariableBlueprintLibrary::ConvertToVariableKind(VariableKindStr, Args.VariableKind, bSuccess);
	if (!bSuccess)
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
	FVector2d Vector2Value;
	FVector Vector3Value;

	bool bSuccess;
	UShidenVariableBlueprintLibrary::FindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType, bBooleanValue,
	                                              StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess, ErrorMessage);

	if (!bSuccess)
	{
		return false;
	}

	switch (VariableType)
	{
	case EShidenVariableType::Boolean:
		{
			const bool bRightHandValue = Args.RightHandValue.ToBool();
			UShidenVariableBlueprintLibrary::EvaluateBoolean(Args.Operator, bBooleanValue, bRightHandValue, bResult, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::String:
	case EShidenVariableType::AssetPath:
		{
			UShidenVariableBlueprintLibrary::EvaluateString(Args.Operator, StringValue, Args.RightHandValue, bResult, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Integer:
		{
			const int32 RightHandValue = FCString::Atoi(*Args.RightHandValue);
			UShidenVariableBlueprintLibrary::EvaluateInteger(Args.Operator, IntegerValue, RightHandValue, bResult, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			const float RightHandValue = FCString::Atof(*Args.RightHandValue);
			UShidenVariableBlueprintLibrary::EvaluateFloat(Args.Operator, FloatValue, RightHandValue, bResult, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector2:
		{
			FVector2d RightHandValue;
			RightHandValue.InitFromString(Args.RightHandValue);
			UShidenVariableBlueprintLibrary::EvaluateVector2(Args.Operator, Vector2Value, RightHandValue, bResult, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Vector3:
		{
			FVector RightHandValue;
			RightHandValue.InitFromString(Args.RightHandValue);
			UShidenVariableBlueprintLibrary::EvaluateVector3(Args.Operator, Vector3Value, RightHandValue, bResult, bSuccess, ErrorMessage);
			break;
		}
	}

	return bSuccess;
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
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		return false;
	}

	const FShidenScenarioProgress ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();

	bool bSuccess;
	UShidenScenario* Scenario = nullptr;
	UShidenScenarioBlueprintLibrary::GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("GetScenarioFromCache failed.");
		return false;
	}

	FString CommandName;
	int32 ResultIndex;
	FindNextElseIfOrElseOrEndIf(ScenarioProgress.CurrentIndex, Scenario, CommandName, ResultIndex, bSuccess, ErrorMessage);
	if (!bSuccess)
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

void UShidenIfCommand::FindNextElseIfOrElseOrEndIf(const int32 StartIndex, const UShidenScenario* Scenario, FString& CommandName,
                                                   int32& ResultIndex, bool& bSuccess, FString& ErrorMessage)
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
				bSuccess = true;
				return;
			}
		}
		else if (Command.CommandName == TEXT("EndIf"))
		{
			if (Depth == 0)
			{
				CommandName = Command.CommandName;
				ResultIndex = Index;
				bSuccess = true;
				return;
			}
			Depth--;
		}
	}

	bSuccess = false;
	ErrorMessage = TEXT("Failed to find ElseIf or Else or EndIf command.");
}
