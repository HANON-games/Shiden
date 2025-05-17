// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenTextInputCommand.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"
#include "System/ShidenBlueprintLibrary.h"

bool UShidenTextInputCommand::TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FTextInputCommandArgs& Args,
                                              FString& ErrorMessage)
{
	const FString MaxLengthStr = Command.GetArg("MaxLength");
	const FString MaxLineCountStr = Command.GetArg("MaxLineCount");
	Args.AllowedCharacterRegex = Command.GetArg("AllowedCharacterRegex");
	Args.DefaultText = Command.GetArg("DefaultText");
	Args.HintText = Command.GetArg("HintText");
	const FString DestinationVariableKindStr = Command.GetArg("DestinationVariableKind");
	Args.DestinationVariableName = Command.GetArg("DestinationVariableName");
	Args.bHideTextLayer = Command.GetArgAsBool("HideTextLayer");

	Args.MaxLength = MaxLengthStr.IsEmpty() ? -1 : FCString::Atoi(*MaxLengthStr);
	Args.MaxLineCount = MaxLineCountStr.IsEmpty() ? -1 : FCString::Atoi(*MaxLineCountStr);

	bool bSuccess;
	UShidenVariableBlueprintLibrary::ConvertToVariableKind(DestinationVariableKindStr, Args.DestinationVariableKind, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenVariableKind."), *DestinationVariableKindStr);
		return false;
	}

	FShidenVariableDefinition Definition;
	UShidenVariableBlueprintLibrary::FindVariableDefinition(ProcessName, Args.DestinationVariableKind, Args.DestinationVariableName, Definition,
	                                                        bSuccess, ErrorMessage);
	if (!bSuccess)
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

	Status = TryInitializeTextInput(Args, ShidenWidget, ErrorMessage)
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

	bool bSuccess;
	ShidenWidget->SetVisibilityByName("TextInputLayer", ESlateVisibility::Collapsed, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set TextInputLayer visibility.");
		Status = EShidenProcessStatus::Error;
		return;
	}

	const FString ResultText = ShidenWidget->GetTextInput().ToString();

	FVector2d Vector2Value;
	Vector2Value.InitFromString(*ResultText);
	FVector Vector3Value;
	Vector3Value.InitFromString(*ResultText);

	UShidenVariableBlueprintLibrary::UpdateVariable(ShidenWidget, ProcessName, Args.DestinationVariableKind,
	                                                Args.DestinationType, Args.DestinationVariableName, ResultText.ToBool(),
	                                                ResultText, FCString::Atoi(*ResultText),
	                                                FCString::Atof(*ResultText), Vector2Value, Vector3Value, bSuccess, ErrorMessage);
	if (!bSuccess)
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	ShidenWidget->SetInputModeGameAndUI();

	UShidenBlueprintLibrary::AddBacklogItem(Command, {{"InputText", ResultText}});

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
		if (!TryInitializeTextInput(Args, ShidenWidget, ErrorMessage))
		{
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}

	const FString SampleText = Args.DefaultText.IsEmpty() ? TEXT("Sample") : Args.DefaultText;

	FVector2d Vector2Value;
	Vector2Value.InitFromString(*SampleText);
	FVector Vector3Value;
	Vector3Value.InitFromString(*SampleText);

	bool bSuccess;
	UShidenVariableBlueprintLibrary::UpdateVariable(ShidenWidget, TEXT("Default"), Args.DestinationVariableKind,
	                                                Args.DestinationType, Args.DestinationVariableName, SampleText.ToBool(),
	                                                SampleText, FCString::Atoi(*SampleText),
	                                                FCString::Atof(*SampleText), Vector2Value, Vector3Value, bSuccess, ErrorMessage);

	Status = bSuccess ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

bool UShidenTextInputCommand::TryInitializeTextInput(const FTextInputCommandArgs& Args, UShidenWidget* ShidenWidget, FString& ErrorMessage)
{
	bool bSuccess;
	ShidenWidget->SetVisibilityByName("TextInputLayer", ESlateVisibility::Visible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to show TextInputLayer.");
		return false;
	}

	if (Args.bHideTextLayer)
	{
		ShidenWidget->SetVisibilityByName("TextLayer", ESlateVisibility::Collapsed, true, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to hide TextLayer.");
			return false;
		}
	}

	ShidenWidget->InitTextInput(FShidenTextInputProperties{
		Args.MaxLength, Args.AllowedCharacterRegex, Args.DefaultText, Args.HintText, Args.MaxLineCount
	});
	ShidenWidget->SetInputModeTextInput();

	return true;
}
