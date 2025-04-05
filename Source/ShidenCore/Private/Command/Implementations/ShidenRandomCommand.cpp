// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenRandomCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenRandomCommand::TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FRandomCommandArgs& Args,
                                           FString& ErrorMessage)
{
	const FString VariableKindStr = Command.GetArg("VariableKind");
	Args.DestinationVariableName = Command.GetArg("DestinationVariableName");
	Args.MinValue = Command.GetArg("Min");
	Args.MaxValue = Command.GetArg("Max");

	bool bSuccess;
	UShidenVariableBlueprintLibrary::ConvertToVariableKind(VariableKindStr, Args.VariableKind, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *VariableKindStr);
		return false;
	}

	FShidenVariableDefinition Definition;
	UShidenVariableBlueprintLibrary::FindVariableDefinition(ProcessName, Args.VariableKind, Args.DestinationVariableName, Definition, bSuccess,
	                                                      ErrorMessage);
	if (!bSuccess)
	{
		return false;
	}

	if (Definition.bIsReadOnly)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is read only."), *Args.DestinationVariableName);
		return false;
	}

	if (Definition.Type != EShidenVariableType::Integer && Definition.Type != EShidenVariableType::Float)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is not Integer or Float."), *Args.DestinationVariableName);
		return false;
	}

	Args.VariableType = Definition.Type;

	return true;
}

void UShidenRandomCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                         UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                         const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                         FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(ProcessName, Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TrySetRandomValue(Args, ProcessName, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenRandomCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                         EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(TEXT("Default"), Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TrySetRandomValue(Args, TEXT("Default"), ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenRandomCommand::TrySetRandomValue(const FRandomCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage)
{
	bool bSuccess;
	switch (Args.VariableType)
	{
	case EShidenVariableType::Integer:
		{
			const int32 Min = FCString::Atoi(*Args.MinValue);
			const int32 Max = FCString::Atoi(*Args.MaxValue);
			if (Min > Max)
			{
				ErrorMessage = FString::Printf(TEXT("Min value %d is greater than Max value %d."), Min, Max);
				return false;
			}
			const int32 RandomValue = FMath::RandRange(Min, Max);
			UShidenVariableBlueprintLibrary::UpdateVariable(ProcessName, Args.VariableKind, Args.VariableType,
														   Args.DestinationVariableName, false, TEXT(""), RandomValue, 0.0f,
														   FVector2D::ZeroVector, FVector::ZeroVector, bSuccess, ErrorMessage);
			break;
		}
	case EShidenVariableType::Float:
		{
			const float Min = FCString::Atof(*Args.MinValue);
			const float Max = FCString::Atof(*Args.MaxValue);
			if (Min > Max)
			{
				ErrorMessage = FString::Printf(TEXT("Min value %d is greater than Max value %d."), Min, Max);
				return false;
			}
			const float RandomValue = FMath::RandRange(Min, Max);
			UShidenVariableBlueprintLibrary::UpdateVariable(ProcessName, Args.VariableKind, Args.VariableType,
														   Args.DestinationVariableName, false, TEXT(""), 0, RandomValue,
														   FVector2D::ZeroVector, FVector::ZeroVector, bSuccess, ErrorMessage);
			break;
		}
	default:
		ErrorMessage = FString::Printf(TEXT("Variable %s is not Integer or Float."), *Args.DestinationVariableName);
		return false;
	}

	return bSuccess;
}
