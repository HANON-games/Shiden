// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenWaitUntilCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenWaitUntilCommand::TryParseCommand(const FShidenCommand& Command, FWaitUntilCommandArgs& Args, FString& ErrorMessage)
{
	Args.VariableKindStr = Command.GetArg("VariableKind");
	Args.VariableName = Command.GetArg("VariableName");
	Args.Operator = Command.GetArg("Operator");
	Args.RightHandValue = Command.GetArg("RightHandValue");

	bool bSuccess;
	UShidenVariableBlueprintLibrary::ConvertToVariableKind(Args.VariableKindStr, Args.VariableKind, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *Args.VariableKindStr);
		return false;
	}

	return true;
}

void UShidenWaitUntilCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                            UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
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
	bool bSuccess;
	
	UShidenVariableBlueprintLibrary::FindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType,
	                                                      bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value,
	                                                      bSuccess, ErrorMessage);
	if (!bSuccess)
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
