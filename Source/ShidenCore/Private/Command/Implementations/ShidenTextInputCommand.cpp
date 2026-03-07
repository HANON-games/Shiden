// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenTextInputCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "System/ShidenBlueprintLibrary.h"

bool UShidenTextInputCommand::TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FTextInputCommandArgs& Args,
                                              FString& ErrorMessage)
{
	const TOptional<FString> MaxLengthOpt = Command.GetOptionalArg(TEXT("MaxLength"));
	const TOptional<FString> MaxLineCountOpt = Command.GetOptionalArg(TEXT("MaxLineCount"));
	const TOptional<FString> AllowedCharacterRegexOpt = Command.GetOptionalArg(TEXT("AllowedCharacterRegex"));
	Args.AllowedCharacterRegex = AllowedCharacterRegexOpt.Get(TEXT(""));
	Args.DefaultText = Command.GetArg(TEXT("DefaultText"));
	Args.HintText = Command.GetArg(TEXT("HintText"));
	const FString DestinationVariableKindStr = Command.GetArg(TEXT("DestinationVariableKind"));
	Args.DestinationVariableName = Command.GetArg(TEXT("DestinationVariableName"));
	Args.bHideTextLayer = Command.GetArgAsBool(TEXT("HideTextLayer"));

	Args.MaxLength = !MaxLengthOpt.IsSet() || MaxLengthOpt.GetValue().IsEmpty() ? -1 : FCString::Atoi(*MaxLengthOpt.GetValue());
	Args.MaxLineCount = !MaxLineCountOpt.IsSet() || MaxLineCountOpt.GetValue().IsEmpty() ? -1 : FCString::Atoi(*MaxLineCountOpt.GetValue());

	if (!UShidenVariableBlueprintLibrary::TryConvertToVariableKind(DestinationVariableKindStr, Args.DestinationVariableKind))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *DestinationVariableKindStr);
		return false;
	}

	FShidenVariableDefinition Definition;
	if (!UShidenVariableBlueprintLibrary::TryFindVariableDefinition(ProcessName, Args.DestinationVariableKind, Args.DestinationVariableName, Definition, ErrorMessage))
	{
		return false;
	}

	if (Definition.bIsReadOnly)
	{
		ErrorMessage = FString::Printf(TEXT("Variable %s is read only."), *Args.DestinationVariableName);
		return false;
	}

	Args.DestinationType = Definition.Type;
	return true;
}

void UShidenTextInputCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                               UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                               UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(ProcessName, Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Status = TryInitializeTextInput(Args, false, ShidenWidget, ErrorMessage)
		         ? EShidenPreProcessStatus::Complete
		         : EShidenPreProcessStatus::Error;
}

void UShidenTextInputCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                            UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                            FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!ShidenWidget->IsTextSubmitted())
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (!ShidenWidget->TrySetVisibilityByName(TEXT("TextInputLayer"), ESlateVisibility::Collapsed, true))
	{
		ErrorMessage = TEXT("Failed to set TextInputLayer visibility.");
		Status = EShidenProcessStatus::Error;
		return;
	}

	const FString ResultText = ShidenWidget->GetTextInput().ToString();

	FVector2D Vector2Value;
	FVector Vector3Value;

	// Only parse vectors if needed for the destination type
	if (Args.DestinationType == EShidenVariableType::Vector2)
	{
		if (!Vector2Value.InitFromString(*ResultText))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *ResultText);
			Status = EShidenProcessStatus::Error;
			return;
		}
	}
	else if (Args.DestinationType == EShidenVariableType::Vector3)
	{
		if (!Vector3Value.InitFromString(*ResultText))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector."), *ResultText);
			Status = EShidenProcessStatus::Error;
			return;
		}
	}

	if (!UShidenVariableBlueprintLibrary::TryUpdateVariable(ShidenWidget, ProcessName, Args.DestinationVariableKind,
	                                                        Args.DestinationType, Args.DestinationVariableName, ResultText.ToBool(),
	                                                        ResultText, FCString::Atoi(*ResultText),
	                                                        FCString::Atof(*ResultText), Vector2Value, Vector3Value, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	ShidenWidget->SetInputModeGameAndUI();

	UShidenBlueprintLibrary::AddBacklogItem(Command, {{TEXT("InputText"), ResultText}});

	Status = EShidenProcessStatus::Next;
}

void UShidenTextInputCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            const bool bIsCurrentCommand,
                                                            EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(TEXT("Default"), Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (bIsCurrentCommand)
	{
		if (!TryInitializeTextInput(Args, true, ShidenWidget, ErrorMessage))
		{
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}

	const FString SampleText = Args.DefaultText.IsEmpty() ? TEXT("Sample") : Args.DefaultText;

	FVector2D Vector2Value;
	FVector Vector3Value;

	// Only parse vectors if needed for the destination type
	if (Args.DestinationType == EShidenVariableType::Vector2)
	{
		if (!Vector2Value.InitFromString(*SampleText))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector2D."), *SampleText);
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}
	else if (Args.DestinationType == EShidenVariableType::Vector3)
	{
		if (!Vector3Value.InitFromString(*SampleText))
		{
			ErrorMessage = FString::Printf(TEXT("Failed to convert %s to FVector."), *SampleText);
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}

	const bool bSuccess = UShidenVariableBlueprintLibrary::TryUpdateVariable(ShidenWidget, TEXT("Default"), Args.DestinationVariableKind,
	                                                                         Args.DestinationType, Args.DestinationVariableName, SampleText.ToBool(),
	                                                                         SampleText, FCString::Atoi(*SampleText),
	                                                                         FCString::Atof(*SampleText), Vector2Value, Vector3Value, ErrorMessage);

	Status = bSuccess ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

bool UShidenTextInputCommand::TryInitializeTextInput(const FTextInputCommandArgs& Args, const bool bIsPreview, UShidenWidget* ShidenWidget, FString& ErrorMessage)
{
	if (!ShidenWidget->TrySetVisibilityByName(TEXT("TextInputLayer"), ESlateVisibility::Visible, true))
	{
		ErrorMessage = TEXT("Failed to show TextInputLayer.");
		return false;
	}

	if (Args.bHideTextLayer)
	{
		if (!ShidenWidget->TrySetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::Collapsed, true))
		{
			ErrorMessage = TEXT("Failed to hide TextLayer.");
			return false;
		}
	}

	ShidenWidget->InitTextInput(FShidenTextInputProperties{
		Args.MaxLength, Args.AllowedCharacterRegex, Args.DefaultText, Args.HintText, Args.MaxLineCount
	});

	if (!bIsPreview)
	{
		ShidenWidget->SetInputModeTextInput();
	}

	return true;
}
