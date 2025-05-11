// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenOptionsCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "Variable/ShidenVariableKind.h"
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenOptionsCommand::TryParseCommand(const FShidenCommand& Command, FOptionsCommandArgs& Args, FString& ErrorMessage)
{
	const FString Choice1 = Command.GetArg(TEXT("Choice 1"));
	const FString Choice2 = Command.GetArg(TEXT("Choice 2"));
	const FString Choice3 = Command.GetArg(TEXT("Choice 3"));
	const FString Choice4 = Command.GetArg(TEXT("Choice 4"));
	const FString Choice5 = Command.GetArg(TEXT("Choice 5"));
	Args.Options = {Choice1, Choice2, Choice3, Choice4, Choice5};
	const FString DestinationVariableKindStr = Command.GetArg(TEXT("DestinationVariableKind"));
	Args.DestinationVariableName = Command.GetArg(TEXT("DestinationVariableName"));
	Args.bHideTextLayer = Command.GetArgAsBool(TEXT("HideTextLayer"));

	bool bSuccess;
	UShidenVariableBlueprintLibrary::ConvertToVariableKind(DestinationVariableKindStr, Args.DestinationVariableKind, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *DestinationVariableKindStr);
		return false;
	}

	return true;
}

void UShidenOptionsCommand::PreProcessCommand_Implementation(const FString& ProcessName,
                                                             const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                             const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
                                                             EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	if (!TrySetupOptions(Args, ShidenWidget, true, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	ShidenWidget->SetInputModeOptionSelection();
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenOptionsCommand::ProcessCommand_Implementation(const FString& ProcessName,
                                                          const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                          const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
                                                          UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                          FString& NextScenarioName, FString& ErrorMessage)
{
	if (!ShidenWidget->IsOptionSelected())
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	const int32 SelectedOption = ShidenWidget->GetSelectedOption();
	if (!TryUpdateVariable(Args.DestinationVariableName, Args.DestinationVariableKind, SelectedOption, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	bool bSuccess;
	ShidenWidget->SetVisibilityByName(TEXT("OptionLayer"), ESlateVisibility::Collapsed, true, bSuccess);
	if (!bSuccess)
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("Failed to set OptionLayer visibility.");
		return;
	}

	ShidenWidget->SetInputModeGameAndUI();

	const TMap<FString, FString> AdditionalProperties = {{TEXT("SelectedOption"), FString::FromInt(SelectedOption)}};
	UShidenBlueprintLibrary::AddBacklogItem(Command, AdditionalProperties);

	Status = EShidenProcessStatus::Next;
}

void UShidenOptionsCommand::PreviewCommand_Implementation(const FShidenCommand& Command,
                                                          UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                          bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!TrySetupOptions(Args, ShidenWidget, false, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = TryUpdateVariable(Args.DestinationVariableName, Args.DestinationVariableKind, 0, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenOptionsCommand::TrySetupOptions(const FOptionsCommandArgs& Args, UShidenWidget* ShidenWidget, const bool bRegisterProperty,
                                            FString& ErrorMessage)
{
	ShidenWidget->SetOptions(Args.Options);

	bool bSuccess;
	ShidenWidget->SetVisibilityByName(TEXT("OptionLayer"), ESlateVisibility::SelfHitTestInvisible, bRegisterProperty, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set OptionLayer visibility.");
		return false;
	}

	if (Args.bHideTextLayer)
	{
		ShidenWidget->SetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::Collapsed, bRegisterProperty, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to set TextLayer visibility.");
			return false;
		}
	}

	return true;
}

bool UShidenOptionsCommand::TryUpdateVariable(const FString& VariableName, const EShidenVariableKind VariableKind, const int32 Value, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	switch (VariableKind)
	{
	case EShidenVariableKind::UserVariable:
		return ShidenSubsystem->UserVariable.TryUpdate(VariableName, Value);
	case EShidenVariableKind::SystemVariable:
		return ShidenSubsystem->SystemVariable.TryUpdate(VariableName, Value);
	case EShidenVariableKind::LocalVariable:
		{
			bool bSuccess;
			UShidenVariableBlueprintLibrary::UpdateLocalInteger(TEXT("Default"), VariableName, Value, bSuccess, ErrorMessage);
			return bSuccess;
		}
	case EShidenVariableKind::PredefinedSystemVariable:
		{
			bool bSuccess;
			UShidenVariableBlueprintLibrary::UpdatePredefinedSystemVariableByString(nullptr, VariableName, FString::FromInt(Value), bSuccess);
			return bSuccess;
		}
	default:
		ErrorMessage = FString::Printf(TEXT("Invalid variable kind for %s."), *VariableName);
		return false;
	}
}
