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

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(VariableKindStr, Args.VariableKind))
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
	FVector2D Vector2Value;
	FVector Vector3Value;
	EShidenVariableType VariableType;
	if (!UShidenVariableBlueprintLibrary::TryFindVariable(ProcessName, Args.VariableKind, Args.VariableName, VariableType,
	                                                      bBooleanValue, StringValue, IntegerValue, FloatValue, Vector2Value, Vector3Value,
	                                                      ErrorMessage))
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
				return UShidenVariableBlueprintLibrary::TryUpdateLocalString(ProcessName, Args.VariableName, ResultValue, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, ResultValue);
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
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
				return UShidenVariableBlueprintLibrary::TryUpdateLocalInteger(ProcessName, Args.VariableName, ResultValue, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, FString::FromInt(ResultValue));
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
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
				return UShidenVariableBlueprintLibrary::TryUpdateLocalFloat(ProcessName, Args.VariableName, ResultValue, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, FString::SanitizeFloat(ResultValue));
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::Vector2:
		{
			FVector2D ValueVector2 = FVector2D::ZeroVector;
			if (!ValueVector2.InitFromString(Args.Value))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *Args.Value);
				return false;
			}
			FVector2D ResultValue = FVector2D::ZeroVector;
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
				return UShidenVariableBlueprintLibrary::TryUpdateLocalVector2(ProcessName, Args.VariableName, ResultValue, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, ResultValue.ToString());
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::Vector3:
		{
			FVector ValueVector3 = FVector::ZeroVector;
			if (!ValueVector3.InitFromString(Args.Value))
			{
				ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector."), *Args.Value);
				return false;
			}
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
				return UShidenVariableBlueprintLibrary::TryUpdateLocalVector3(ProcessName, Args.VariableName, ResultValue, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, ResultValue.ToString());
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	default:
		ErrorMessage = TEXT("Unknown variable type.");
		return false;
	}
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

bool UShidenCalculationCommand::CalculateVector2(const FString& Operator, const FVector2D& A, const FVector2D& B, FVector2D& Result,
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
		Result.X = FMath::IsNearlyZero(B.X) ? 0.0f : A.X / B.X;
		Result.Y = FMath::IsNearlyZero(B.Y) ? 0.0f : A.Y / B.Y;
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
		Result.X = FMath::IsNearlyZero(B.X) ? 0.0f : A.X / B.X;
		Result.Y = FMath::IsNearlyZero(B.Y) ? 0.0f : A.Y / B.Y;
		Result.Z = FMath::IsNearlyZero(B.Z) ? 0.0f : A.Z / B.Z;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Operator %s is not supported in calculation."), *Operator);
	return false;
}
