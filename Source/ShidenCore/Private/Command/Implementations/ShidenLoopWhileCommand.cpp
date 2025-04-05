// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenLoopWhileCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenLoopWhileCommand::TryParseCommand(const FShidenCommand& Command, FLoopWhileCommandArgs& Args, FString& ErrorMessage)
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

void UShidenLoopWhileCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
															const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
															UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
															FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryEvaluateCondition(Args, ProcessName, ErrorMessage)
				 ? EShidenProcessStatus::Next
				 : EShidenProcessStatus::Error;
}

bool UShidenLoopWhileCommand::TryEvaluateCondition(const FLoopWhileCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage)
{
	bool bSuccess;
	EShidenVariableType VariableType;
	bool bBooleanValue;
	FString StringValue;
	int32 IntegerValue;
	float FloatValue;
	FVector2d Vector2Value;
	FVector Vector3Value;
	UShidenVariableBlueprintLibrary::FindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType, bBooleanValue,
	                                             StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess, ErrorMessage);
	if (!bSuccess)
	{
		return false;
	}

	bool bResult = false;
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

	if (!bSuccess)
	{
		return false;
	}

	if (!bResult)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
		check(ShidenSubsystem);

		if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
			|| ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
		{
			return true;
		}

		int32 NextIndex;
		UShidenScenarioBlueprintLibrary::ToNext(ProcessName, NextIndex);

		const FShidenScenarioProgress ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();

		UShidenScenario* Scenario = nullptr;
		UShidenScenarioBlueprintLibrary::GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("GetScenarioFromCache failed.");
			return false;
		}

		int32 ResultIndex;
		if (!TryFindEndLoopWhileIndex(Scenario, ScenarioProgress.CurrentIndex + 1, ResultIndex, ErrorMessage))
		{
			return false;
		}
		UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, ResultIndex);
	}

	return true;
}

bool UShidenLoopWhileCommand::TryFindEndLoopWhileIndex(const UShidenScenario* Scenario, const int32 StartIndex, int32& ResultIndex, FString& ErrorMessage)
{
	for (int32 Index = StartIndex; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& Command = Scenario->Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}

		if (Command.CommandName == TEXT("EndLoopWhile"))
		{
			ResultIndex = Index;
			return true;
		}

		if (Command.CommandName == TEXT("LoopWhile"))
		{
			if (!TryFindEndLoopWhileIndex(Scenario, Index + 1, ResultIndex, ErrorMessage))
			{
				return false;
			}

			return TryFindEndLoopWhileIndex(Scenario, ResultIndex + 1, ResultIndex, ErrorMessage);
		}
	}

	ErrorMessage = TEXT("EndLoopWhile is not found.");
	return false;
}
