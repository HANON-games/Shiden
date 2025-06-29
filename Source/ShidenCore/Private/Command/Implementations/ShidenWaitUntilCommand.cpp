// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenWaitUntilCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenWaitUntilCommand::TryParseCommand(const FShidenCommand& Command, FWaitUntilCommandArgs& Args, FString& ErrorMessage)
{
	Args.VariableKindStr = Command.GetArg("VariableKind");
	Args.VariableName = Command.GetArg("VariableName");
	Args.Operator = Command.GetArg("Operator");
	Args.RightHandValue = Command.GetArg("RightHandValue");

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(Args.VariableKindStr, Args.VariableKind))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *Args.VariableKindStr);
		return false;
	}

	return true;
}

void UShidenWaitUntilCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                            UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                            FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	bool bResult;
	if (!TryEvaluateCondition(Args, ProcessName, bResult, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = bResult ? EShidenProcessStatus::Next : EShidenProcessStatus::DelayUntilNextTick;
}

bool UShidenWaitUntilCommand::TryEvaluateCondition(const FWaitUntilCommandArgs& Args, const FString& ProcessName, bool& bResult, FString& ErrorMessage)
{
	EShidenVariableType VariableType = EShidenVariableType::Boolean;
	bool bBooleanValue;
	FString StringValue;
	int32 IntegerValue;
	float FloatValue;
	FVector2d Vector2Value;
	FVector Vector3Value;

	if (!UShidenVariableBlueprintLibrary::TryFindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType,
												  bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value,
												  ErrorMessage))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find variable %s."), *Args.VariableName);
		return false;
	}

	bResult = false;
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
			FVector2d RightHandValue;
			RightHandValue.InitFromString(Args.RightHandValue);
			return UShidenVariableBlueprintLibrary::TryEvaluateVector2(Args.Operator, Vector2Value, RightHandValue, bResult, ErrorMessage);
		}
	case EShidenVariableType::Vector3:
		{
			FVector RightHandValue;
			RightHandValue.InitFromString(Args.RightHandValue);
			return UShidenVariableBlueprintLibrary::TryEvaluateVector3(Args.Operator, Vector3Value, RightHandValue, bResult, ErrorMessage);
		}
	}
	return false;
}
