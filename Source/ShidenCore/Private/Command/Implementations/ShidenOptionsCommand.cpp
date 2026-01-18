// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenOptionsCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "Variable/ShidenVariableKind.h"
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenOptionsCommand::TryParseCommand(const FShidenCommand& Command, FOptionsCommandArgs& Args, FString& ErrorMessage)
{
	Args.LocalizedOptions.Empty();
	for (int32 i = 0; i < MaxLanguageCount; i++)
	{
		const FString Prefix = i == 0 ? TEXT("") : FString::Printf(TEXT("Language %d "), i + 1);
		Args.LocalizedOptions.Add(FOptions{
			{
				Command.GetArg(Prefix + TEXT("Option 1")),
				Command.GetArg(Prefix + TEXT("Option 2")),
				Command.GetArg(Prefix + TEXT("Option 3")),
				Command.GetArg(Prefix + TEXT("Option 4")),
				Command.GetArg(Prefix + TEXT("Option 5"))
			}
		});
	}

	const FString DestinationVariableKindStr = Command.GetArg(TEXT("DestinationVariableKind"));
	Args.DestinationVariableName = Command.GetArg(TEXT("DestinationVariableName"));
	Args.bHideTextLayer = Command.GetArgAsBool(TEXT("HideTextLayer"));

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(DestinationVariableKindStr, Args.DestinationVariableKind))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *DestinationVariableKindStr);
		return false;
	}

	return true;
}

void UShidenOptionsCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
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

void UShidenOptionsCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
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
	if (!TryUpdateVariable(ProcessName, Args.DestinationVariableName, Args.DestinationVariableKind, SelectedOption, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	if (!ShidenWidget->TrySetVisibilityByName(TEXT("OptionLayer"), ESlateVisibility::Collapsed, true))
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

void UShidenOptionsCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                          const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                          const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (bIsCurrentCommand)
	{
		if (!TrySetupOptions(Args, ShidenWidget, false, ErrorMessage))
		{
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}

	Status = TryUpdateVariable(TEXT("Default"), Args.DestinationVariableName, Args.DestinationVariableKind, 0, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

bool UShidenOptionsCommand::TrySetupOptions(const FOptionsCommandArgs& Args, UShidenWidget* ShidenWidget, const bool bRegisterProperty,
                                            FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	const int32 LanguageIndex = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;

	if (!Args.LocalizedOptions.IsValidIndex(LanguageIndex))
	{
		ErrorMessage = FString::Printf(TEXT("Invalid LanguageIndex: %d. Must be between 0 and %d."), LanguageIndex, Args.LocalizedOptions.Num() - 1);
		return false;
	}

	ShidenWidget->SetOptions(Args.LocalizedOptions[LanguageIndex].Options);

	if (!ShidenWidget->TrySetVisibilityByName(TEXT("OptionLayer"), ESlateVisibility::SelfHitTestInvisible, bRegisterProperty))
	{
		ErrorMessage = TEXT("Failed to set OptionLayer visibility.");
		return false;
	}

	if (Args.bHideTextLayer)
	{
		if (!ShidenWidget->TrySetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::Collapsed, bRegisterProperty))
		{
			ErrorMessage = TEXT("Failed to set TextLayer visibility.");
			return false;
		}
	}

	return true;
}

bool UShidenOptionsCommand::TryUpdateVariable(const FString& ProcessName, const FString& VariableName, const EShidenVariableKind VariableKind, const int32 Value,
                                              FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	switch (VariableKind)
	{
	case EShidenVariableKind::UserVariable:
		return ShidenSubsystem->UserVariable.TryUpdate(VariableName, Value);
	case EShidenVariableKind::SystemVariable:
		return ShidenSubsystem->SystemVariable.TryUpdate(VariableName, Value);
	case EShidenVariableKind::LocalVariable:
		{
			return UShidenVariableBlueprintLibrary::TryUpdateLocalInteger(ProcessName, VariableName, Value, ErrorMessage);
		}
	case EShidenVariableKind::PredefinedSystemVariable:
		{
			return UShidenVariableBlueprintLibrary::TryUpdatePredefinedSystemVariableByString(nullptr, VariableName, FString::FromInt(Value));
		}
	default:
		ErrorMessage = FString::Printf(TEXT("Invalid variable kind for %s."), *VariableName);
		return false;
	}
}
