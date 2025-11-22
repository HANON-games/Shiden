// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenAssignExpressionCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Expression/ShidenExpressionBlueprintLibrary.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenAssignExpressionCommand::TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FAssignExpressionCommandArgs& Args,
                                                     FString& ErrorMessage)
{
	// HACK: get expression from original command
	FShidenCommand* OriginalCommand = nullptr;
	if (!ShidenCommandHelpers::TryGetCurrentOriginalCommand(ProcessName, OriginalCommand, ErrorMessage))
	{
		return false;
	}

	const FString VariableKindStr = Command.GetArg(TEXT("VariableKind"));
	Args.VariableName = Command.GetArg(TEXT("VariableName"));
	Args.Expression = OriginalCommand->GetArg(TEXT("Expression"));

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(VariableKindStr, Args.VariableKind))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *VariableKindStr);
		return false;
	}

	if (Args.VariableName.IsEmpty())
	{
		ErrorMessage = TEXT("VariableName is empty");
		return false;
	}

	if (Args.Expression.IsEmpty())
	{
		ErrorMessage = TEXT("Expression is empty");
		return false;
	}

	return true;
}

void UShidenAssignExpressionCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                   UShidenWidget* ShidenWidget,
                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                   const float DeltaTime,
                                                                   UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                                   FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(ProcessName, Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TryEvaluateAndAssign(CallerObject, ProcessName, Args, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenAssignExpressionCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                   bool bIsCurrentCommand,
                                                                   EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(TEXT("Default"), Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryEvaluateAndAssign(ShidenWidget, TEXT("Default"), Args, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenAssignExpressionCommand::TryEvaluateAndAssign(const UObject* WorldContextObject, const FString& ProcessName,
                                                          const FAssignExpressionCommandArgs& Args, FString& ErrorMessage)
{
	FShidenVariableDefinition VariableDefinition;
	if (!UShidenVariableBlueprintLibrary::TryFindVariableDefinition(ProcessName, Args.VariableKind, Args.VariableName, VariableDefinition,
	                                                                ErrorMessage))
	{
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	switch (VariableDefinition.Type)
	{
	case EShidenVariableType::Boolean:
		{
			bool bResult;
			if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionBoolean(ProcessName, Args.Expression, bResult, ErrorMessage))
			{
				return false;
			}
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, bResult);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, bResult);
			case EShidenVariableKind::LocalVariable:
				return UShidenVariableBlueprintLibrary::TryUpdateLocalBoolean(ProcessName, Args.VariableName, bResult, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(
					WorldContextObject, Args.VariableName, bResult ? TEXT("true") : TEXT("false"));
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::AssetPath:
	case EShidenVariableType::String:
		{
			FString Result;
			if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionString(ProcessName, Args.Expression, Result, ErrorMessage))
			{
				return false;
			}
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::LocalVariable:
				return UShidenVariableBlueprintLibrary::TryUpdateLocalString(ProcessName, Args.VariableName, Result, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, Result);
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::Integer:
		{
			int32 Result;
			if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionInteger(ProcessName, Args.Expression, Result, ErrorMessage))
			{
				return false;
			}
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::LocalVariable:
				return UShidenVariableBlueprintLibrary::TryUpdateLocalInteger(ProcessName, Args.VariableName, Result, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, FString::FromInt(Result));
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::Float:
		{
			float Result;
			if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionFloat(ProcessName, Args.Expression, Result, ErrorMessage))
			{
				return false;
			}
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::LocalVariable:
				return UShidenVariableBlueprintLibrary::TryUpdateLocalFloat(ProcessName, Args.VariableName, Result, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, FString::SanitizeFloat(Result));
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::Vector2:
		{
			FVector2D Result;
			if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionVector2(ProcessName, Args.Expression, Result, ErrorMessage))
			{
				return false;
			}
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::LocalVariable:
				return UShidenVariableBlueprintLibrary::TryUpdateLocalVector2(ProcessName, Args.VariableName, Result, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, Result.ToString());
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	case EShidenVariableType::Vector3:
		{
			FVector Result;
			if (!UShidenExpressionBlueprintLibrary::TryEvaluateExpressionVector3(ProcessName, Args.Expression, Result, ErrorMessage))
			{
				return false;
			}
			switch (Args.VariableKind)
			{
			case EShidenVariableKind::UserVariable:
				return ShidenSubsystem->UserVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::SystemVariable:
				return ShidenSubsystem->SystemVariable.TryUpdate(Args.VariableName, Result);
			case EShidenVariableKind::LocalVariable:
				return UShidenVariableBlueprintLibrary::TryUpdateLocalVector3(ProcessName, Args.VariableName, Result, ErrorMessage);
			case EShidenVariableKind::PredefinedSystemVariable:
				return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(WorldContextObject, Args.VariableName, Result.ToString());
			default:
				ErrorMessage = FString::Printf(TEXT("Unknown variable kind: %d"), static_cast<int32>(Args.VariableKind));
				return false;
			}
		}
	default:
		ErrorMessage = TEXT("Unsupported expression value type.");
		return false;
	}
}
