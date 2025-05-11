// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenCalculationCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenCalculationCommand::TryParseCommand(const FShidenCommand& Command, FCalculationCommandArgs& Args, FString& ErrorMessage)
{
	const FString VariableKindStr = Command.GetArg(TEXT("VariableKind"));
	Args.VariableName = Command.GetArg(TEXT("VariableName"));
	Args.Operator = Command.GetArg(TEXT("Operator"));
	Args.Value = Command.GetArg(TEXT("Value"));

	bool bSuccess;
	UShidenVariableBlueprintLibrary::ConvertToVariableKind(VariableKindStr, Args.VariableKind, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *VariableKindStr);
		return false;
	}

	return true;
}


void UShidenCalculationCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
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

	Status = TryCalculateAndUpdateVariable(Args, CallerObject, ProcessName, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenCalculationCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                              EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryCalculateAndUpdateVariable(Args, ShidenWidget, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenCalculationCommand::TryCalculateAndUpdateVariable(const FCalculationCommandArgs& Args, const UObject* WorldContextObject,
                                                              const FString& ProcessName, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	bool bBooleanValue;
	FString StringValue;
	int32 IntegerValue;
	float FloatValue;
	FVector2d Vector2Value;
	FVector Vector3Value;
	EShidenVariableType VariableType;
	bool bSuccess;
	UShidenVariableBlueprintLibrary::FindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType,
	                                              bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value, bSuccess,
	                                              ErrorMessage);
	if (!bSuccess)
	{
		return false;
	}

	switch (VariableType)
	{
	case EShidenVariableType::Boolean:
		{
			ErrorMessage = TEXT("Boolean type is not supported in calculation.");
			return false;
		}
	case EShidenVariableType::AssetPath:
	case EShidenVariableType::String:
		{
			const FString ResultValue = StringValue + Args.Value;
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::LocalVariable:
				UShidenVariableBlueprintLibrary::UpdateLocalString(ProcessName, Args.VariableName, ResultValue, bSuccess, ErrorMessage);
				return true;
			case EShidenVariableKind::PredefinedSystemVariable:
				UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, ResultValue, bSuccess);
				return true;
			}
			return false;
		}
	case EShidenVariableType::Integer:
		{
			const int32 ValueInt = FCString::Atoi(*Args.Value);
			int32 ResultValue = 0;
			if (!CalculateInteger(Args.Operator, IntegerValue, ValueInt, ResultValue, ErrorMessage))
			{
				return false;
			}

			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::LocalVariable:
				UShidenVariableBlueprintLibrary::UpdateLocalInteger(ProcessName, Args.VariableName, ResultValue, bSuccess, ErrorMessage);
				return true;
			case EShidenVariableKind::PredefinedSystemVariable:
				UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName,
				                                                                        FString::FromInt(ResultValue), bSuccess);
				return true;
			}
			return false;
		}
	case EShidenVariableType::Float:
		{
			const float ValueFloat = FCString::Atof(*Args.Value);
			float ResultValue = 0.0f;
			if (!CalculateFloat(Args.Operator, FloatValue, ValueFloat, ResultValue, ErrorMessage))
			{
				return false;
			}

			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::LocalVariable:
				UShidenVariableBlueprintLibrary::UpdateLocalFloat(ProcessName, Args.VariableName, ResultValue, bSuccess, ErrorMessage);
				return true;
			case EShidenVariableKind::PredefinedSystemVariable:
				UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName,
				                                                                        FString::SanitizeFloat(ResultValue), bSuccess);
				return true;
			}
			return false;
		}
	case EShidenVariableType::Vector2:
		{
			FVector2d ValueVector2 = FVector2d::ZeroVector;
			ValueVector2.InitFromString(Args.Value);
			FVector2d ResultValue = FVector2d::ZeroVector;
			if (!CalculateVector2(Args.Operator, Vector2Value, ValueVector2, ResultValue, ErrorMessage))
			{
				return false;
			}

			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::LocalVariable:
				UShidenVariableBlueprintLibrary::UpdateLocalVector2(ProcessName, Args.VariableName, ResultValue, bSuccess, ErrorMessage);
				return true;
			case EShidenVariableKind::PredefinedSystemVariable:
				UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, ResultValue.ToString(),
				                                                                        bSuccess);
				return true;
			}
			return false;
		}
	case EShidenVariableType::Vector3:
		{
			FVector ValueVector3 = FVector::ZeroVector;
			ValueVector3.InitFromString(Args.Value);
			FVector ResultValue = FVector::ZeroVector;
			if (!CalculateVector3(Args.Operator, Vector3Value, ValueVector3, ResultValue, ErrorMessage))
			{
				return false;
			}

			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, ResultValue);
			case EShidenVariableKind::LocalVariable:
				UShidenVariableBlueprintLibrary::UpdateLocalVector3(ProcessName, Args.VariableName, ResultValue, bSuccess, ErrorMessage);
				return true;
			case EShidenVariableKind::PredefinedSystemVariable:
				UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, ResultValue.ToString(),
				                                                                        bSuccess);
				return true;
			}
			return false;
		}
	}

	ErrorMessage = TEXT("Unknown variable type.");
	return false;
}

bool UShidenCalculationCommand::CalculateFloat(const FString& Operator, const float& A, const float& B, float& Result, FString& ErrorMessage)
{
	if (Operator == TEXT("+="))
	{
		Result = A + B;
		return true;
	}

	if (Operator == TEXT("-="))
	{
		Result = A - B;
		return true;
	}

	if (Operator == TEXT("*="))
	{
		Result = A * B;
		return true;
	}

	if (Operator == TEXT("/="))
	{
		Result = FMath::IsNearlyZero(B) ? 0.0f : A / B;
		return true;
	}

	if (Operator == TEXT("%="))
	{
		Result = FMath::IsNearlyZero(B) ? 0.0f : FMath::Fmod(A, B);
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Operator %s is not supported in calculation."), *Operator);
	return false;
}

bool UShidenCalculationCommand::CalculateInteger(const FString& Operator, const int32& A, const int32& B, int32& Result, FString& ErrorMessage)
{
	if (Operator == TEXT("+="))
	{
		Result = A + B;
		return true;
	}

	if (Operator == TEXT("-="))
	{
		Result = A - B;
		return true;
	}

	if (Operator == TEXT("*="))
	{
		Result = A * B;
		return true;
	}

	if (Operator == TEXT("/="))
	{
		Result = B == 0 ? 0 : A / B;
		return true;
	}

	if (Operator == TEXT("%="))
	{
		Result = B == 0 ? 0 : A % B;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Operator %s is not supported in calculation."), *Operator);
	return false;
}

bool UShidenCalculationCommand::CalculateVector2(const FString& Operator, const FVector2d& A, const FVector2d& B, FVector2d& Result,
                                                 FString& ErrorMessage)
{
	if (Operator == TEXT("+="))
	{
		Result = A + B;
		return true;
	}

	if (Operator == TEXT("-="))
	{
		Result = A - B;
		return true;
	}

	if (Operator == TEXT("*="))
	{
		Result = A * B;
		return true;
	}

	if (Operator == TEXT("/="))
	{
		Result = B.X == 0 || B.Y == 0 ? FVector2d::ZeroVector : A / B;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Operator %s is not supported in calculation."), *Operator);
	return false;
}

bool UShidenCalculationCommand::CalculateVector3(const FString& Operator, const FVector& A, const FVector& B, FVector& Result, FString& ErrorMessage)
{
	if (Operator == TEXT("+="))
	{
		Result = A + B;
		return true;
	}

	if (Operator == TEXT("-="))
	{
		Result = A - B;
		return true;
	}

	if (Operator == TEXT("*="))
	{
		Result = A * B;
		return true;
	}

	if (Operator == TEXT("/="))
	{
		Result = B.X == 0 || B.Y == 0 || B.Z == 0 ? FVector::ZeroVector : A / B;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Operator %s is not supported in calculation."), *Operator);
	return false;
}
