// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenAssignVariableCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenAssignVariableCommand::TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FAssignVariableCommandArgs& Args,
                                                   FString& ErrorMessage)
{
	const FString VariableKindStr = Command.GetArg("VariableKind");
	Args.VariableName = Command.GetArg("VariableName");
	Args.Value = Command.GetArg("Value");

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(VariableKindStr, Args.VariableKind))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *VariableKindStr);
		return false;
	}

	FShidenVariableDefinition Definition;
	if (!UShidenVariableBlueprintLibrary::TryFindVariableDefinition(ProcessName, Args.VariableKind, Args.VariableName, Definition, ErrorMessage))
	{
		return false;
	}

	if (Definition.bIsReadOnly)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is read only."), *Args.VariableName);
		return false;
	}

	Args.VariableType = Definition.Type;
	return true;
}


void UShidenAssignVariableCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                 UShidenWidget* ShidenWidget,
                                                                 const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                 const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                 FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(ProcessName, Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = TrySetVariable(ShidenWidget, Args, ProcessName, ErrorMessage)
		? EShidenProcessStatus::Next
		: EShidenProcessStatus::Error;
}

void UShidenAssignVariableCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                 const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                 bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(TEXT("Default"), Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TrySetVariable(ShidenWidget, Args, TEXT("Default"), ErrorMessage)
		? EShidenPreviewStatus::Complete
		: EShidenPreviewStatus::Error;
}

bool UShidenAssignVariableCommand::TrySetVariable(const UObject* WorldContextObject, const FAssignVariableCommandArgs& Args,
                                                  const FString& ProcessName, FString& ErrorMessage)
{
	FVector2d Vector2Value;
	Vector2Value.InitFromString(*Args.Value);
	FVector Vector3Value;
	Vector3Value.InitFromString(*Args.Value);

	return UShidenVariableBlueprintLibrary::TryUpdateVariable(WorldContextObject, ProcessName, Args.VariableKind,
	                                                Args.VariableType, Args.VariableName, Args.Value.ToBool(),
	                                                Args.Value, FCString::Atoi(*Args.Value),
	                                                FCString::Atof(*Args.Value), Vector2Value, Vector3Value, ErrorMessage);
}
