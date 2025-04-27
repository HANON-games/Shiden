// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenTextCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"

bool UShidenTextCommand::TryParseCommand(const FShidenCommand& Command, FTextCommandArgs& Args, FString& ErrorMessage)
{
	Args.TextWidgetName = Command.GetArg("TextWidgetName");
	Args.TextType = Command.GetArg("TextType");
	Args.bWaitForInput = Command.GetArgAsBool("WaitForInput");
	Args.bInstantTextDisplay = Command.GetArgAsBool("InstantTextDisplay");
	Args.VoicePath = Command.GetArg("Voice");
	Args.VoiceTrackId = Command.GetArgAsInt("VoiceTrackId");
	Args.bDisableAutoStopPreviousVoices = Command.GetArgAsBool("DisableAutoStopPreviousVoices");
	Args.bContinueFromThePreviousText = Command.GetArgAsBool("ContinueFromThePreviousText");

	return TryGetLanguageSpecificText(Command, Args.Text, ErrorMessage);
}

bool UShidenTextCommand::TryGetLanguageSpecificText(const FShidenCommand& Command, FString& Text, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	static const FString LanguageArgs[] = {
		"Text",
		"Language 2",
		"Language 3",
		"Language 4",
		"Language 5",
		"Language 6",
		"Language 7",
		"Language 8",
		"Language 9",
		"Language 10"
	};

	const int32 LanguageIndex = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
	if (LanguageIndex >= 0 && LanguageIndex < 10)
	{
		Text = Command.GetArg(LanguageArgs[LanguageIndex]);
		return true;
	}

	ErrorMessage = TEXT("Invalid LanguageIndex");
	return false;
}

void UShidenTextCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* Widget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TPair<FString, FString>& Property : ScenarioProperties)
	{
		if (Property.Value.IsEmpty())
		{
			continue;
		}

		FString TextWidgetName, TextType;
		if (!TryParsePropertyKey(Property.Key, TextWidgetName, TextType, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (!TryRestoreTextWidget(Widget, TextWidgetName, TextType, Property.Value, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenTextCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                          UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                          UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	HandleInputState(Widget, ShidenManager);

	CurrentState = EShidenTextCommandState::WaitingForAnimation;
	CurrentVoiceState = EShidenTextCommandVoiceState::None;
	WaitTime = 0.f;
	TotalElapsedTime = 0.f;
	VoiceDuration = 0.f;
	bIsWaitingForAnimation = true;

	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(Widget, Args.TextWidgetName, TextWidget, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
	}

	if (Args.bContinueFromThePreviousText)
	{
		if (!TryGetFullText(TextWidget, Args.TextType, CurrentText, ErrorMessage))
		{
			Status = EShidenPreProcessStatus::Error;
			return;
		}
		CurrentTextIndex = UShidenBlueprintLibrary::GetParsedLength(CurrentText);
		CurrentText = FString::Printf(TEXT("%s%s"), *CurrentText, *Args.Text);
	}
	else
	{
		CurrentTextIndex = -1;
		CurrentText = Args.Text;
	}

	TextLength = UShidenBlueprintLibrary::GetParsedLength(CurrentText);

	if (!TrySetTextWindowVisible(Widget, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	if (CurrentText.IsEmpty())
	{
		if (!TryCloseTextWindow(TextWidget, Args.TextType, ErrorMessage))
		{
			Status = EShidenPreProcessStatus::Error;
			return;
		}
	}
	else
	{
		if (!TryOpenTextWindow(TextWidget, Args.TextType, ErrorMessage))
		{
			Status = EShidenPreProcessStatus::Error;
			return;
		}
	}

	if (ShouldStopVoice(Args.VoicePath, Args.bDisableAutoStopPreviousVoices))
	{
		ShidenManager->Execute_StopVoices(ShidenManager.GetObject());
	}

	if (ShouldPlayVoice(Args.VoicePath, CurrentVoiceState))
	{
		if (!TryProcessVoicePlayback(Args.VoiceTrackId, Args.VoicePath, ShidenManager, ErrorMessage))
		{
			Status = EShidenPreProcessStatus::Error;
			return;
		}
	}
	
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenTextCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                       UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                       FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	HandleInputState(Widget, ShidenManager);
	UpdateTime(DeltaTime);
	UpdateVoiceState();

	switch (CurrentState)
	{
	case EShidenTextCommandState::WaitingForAnimation:
		{
			if (!bIsWaitingForAnimation)
			{
				CurrentState = ShouldPauseTextProcess(Widget)
					               ? EShidenTextCommandState::PausingText
					               : IsTextDisplayComplete()
					               ? EShidenTextCommandState::WaitingForInput
					               : EShidenTextCommandState::ProcessingText;
			}
			Status = EShidenProcessStatus::DelayUntilNextTick;
			break;
		}
	case EShidenTextCommandState::PausingText:
		{
			if (!ShouldPauseTextProcess(Widget))
			{
				CurrentState = bIsWaitingForAnimation
					               ? EShidenTextCommandState::WaitingForAnimation
					               : IsTextDisplayComplete()
					               ? EShidenTextCommandState::WaitingForInput
					               : EShidenTextCommandState::ProcessingText;
			}
			Status = EShidenProcessStatus::DelayUntilNextTick;
			break;
		}
	case EShidenTextCommandState::ProcessingText:
		{
			if (ShouldPauseTextProcess(Widget))
			{
				CurrentState = EShidenTextCommandState::PausingText;
				Status = EShidenProcessStatus::DelayUntilNextTick;
				break;
			}
			if (ShouldPlayVoice(Args.VoicePath, CurrentVoiceState))
			{
				if (!TryProcessVoicePlayback(Args.VoiceTrackId, Args.VoicePath, ShidenManager, ErrorMessage))
				{
					Status = EShidenProcessStatus::Error;
					return;
				}
			}
			if (!TryUpdateTextProgress(Args.TextWidgetName, Args.TextType, Args.bInstantTextDisplay, Widget, DeltaTime, ErrorMessage))
			{
				Status = EShidenProcessStatus::Error;
				return;
			}
			if (IsTextDisplayComplete())
			{
				CurrentState = EShidenTextCommandState::WaitingForInput;
			}
			Status = EShidenProcessStatus::DelayUntilNextTick;
			break;
		}
	case EShidenTextCommandState::WaitingForInput:
		{
			if (ShouldPauseTextProcess(Widget))
			{
				CurrentState = EShidenTextCommandState::PausingText;
				Status = EShidenProcessStatus::DelayUntilNextTick;
				break;
			}
			if (!CanTransitionToComplete(Args.bWaitForInput))
			{
				Status = EShidenProcessStatus::DelayUntilNextTick;
				return;
			}
			UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName,
			                                                          FString::Printf(TEXT("%s::%s"), *Args.TextWidgetName, *Args.TextType),
			                                                          CurrentText);
			UShidenTextWidget* TextWidget;
			if (!FindTextWidget(Widget, Args.TextWidgetName, TextWidget, ErrorMessage))
			{
				Status = EShidenProcessStatus::Error;
				return;
			}
			TMap<FString, FString> Texts;
			TextWidget->GetAllFullTexts(Texts);
			UShidenBlueprintLibrary::AddBacklogItem(Command, Texts);
			Status = EShidenProcessStatus::Next;
			break;
		}
	}
}

void UShidenTextCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                       EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!TrySetTextWindowVisible(Widget, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (bIsCurrentCommand)
	{
		if (!TryProcessVoicePlayback(Args.VoiceTrackId, Args.VoicePath, ShidenManager, ErrorMessage))
		{
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}

	Status = TryPreviewText(Widget, Args.TextWidgetName, Args.TextType, Args.Text, Args.bContinueFromThePreviousText, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

void UShidenTextCommand::UpdateTime(const float DeltaTime)
{
	TotalElapsedTime += DeltaTime;
	if (bSkipButtonPressStart)
	{
		WaitTime = 0;
	}
	else
	{
		WaitTime -= DeltaTime;
	}
}

bool UShidenTextCommand::ShouldPlayVoice(const FString& VoicePath, const EShidenTextCommandVoiceState VoiceState)
{
	return !IsVoicePathEmpty(VoicePath) && VoiceState == EShidenTextCommandVoiceState::None;
}

bool UShidenTextCommand::IsVoicePathEmpty(const FString& VoicePath)
{
	return VoicePath.IsEmpty() || VoicePath == TEXT("None");
}

bool UShidenTextCommand::IsTextDisplayComplete() const
{
	return CurrentTextIndex == TextLength;
}

bool UShidenTextCommand::TryCloseTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage)
{
	bool bSuccess;
	FShidenCloseTextWindowDelegate CloseTextWindowDelegate;
	CloseTextWindowDelegate.BindDynamic(this, &UShidenTextCommand::OnTextWindowOpenedOrClosed);
	TextWidget->CloseWindow(TextType, CloseTextWindowDelegate, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to close text window.");
	}
	return bSuccess;
}

bool UShidenTextCommand::TryOpenTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage)
{
	bool bSuccess;
	FShidenOpenTextWindowDelegate OpenTextWindowDelegate;
	OpenTextWindowDelegate.BindDynamic(this, &UShidenTextCommand::OnTextWindowOpenedOrClosed);
	TextWidget->OpenWindow(TextType, OpenTextWindowDelegate, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to open text window.");
	}
	return bSuccess;
}

bool UShidenTextCommand::FindTextWidget(const UShidenWidget* Widget, const FString& TextWidgetName, UShidenTextWidget*& TextWidget,
                                        FString& ErrorMessage)
{
	bool bSuccess;
	Widget->FindTextWidget(TextWidgetName, TextWidget, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *TextWidgetName);
	}
	return bSuccess;
}

bool UShidenTextCommand::TryParsePropertyKey(const FString& PropertyKey, FString& TextWidgetName, FString& TextType, FString& ErrorMessage)
{
	const bool bSuccess = PropertyKey.Split(TEXT("::"), &TextWidgetName, &TextType, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to split property name %s."), *PropertyKey);
	}
	return bSuccess;
}

bool UShidenTextCommand::TryRestoreTextWidget(UShidenWidget* Widget, const FString& TextWidgetName, const FString& TextType,
                                              const FString& TextValue, FString& ErrorMessage)
{
	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(Widget, TextWidgetName, TextWidget, ErrorMessage))
	{
		return false;
	}

	bool bSuccess;
	TextWidget->OpenWindow(TextType, FShidenOpenTextWindowDelegate(), bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to open text window.");
		return false;
	}

	TextWidget->SetText(TextType, TextValue, TextValue.Len());
	return true;
}

bool UShidenTextCommand::ShouldPauseTextProcess(const UShidenWidget* Widget)
{
	return Widget->IsMenuOpen() || !Widget->TextBaseLayer->IsVisible();
}

bool UShidenTextCommand::ShouldStopVoice(const FString& VoicePath, const bool& bDisableAutoStopPreviousVoices)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	switch (ShidenSubsystem->PredefinedSystemVariable.VoiceStopCondition)
	{
	case EShidenVoiceStopCondition::Never:
		return false;
	case EShidenVoiceStopCondition::NextTextOrVoice:
		return !bDisableAutoStopPreviousVoices;
	case EShidenVoiceStopCondition::NextVoice:
		return !bDisableAutoStopPreviousVoices && !IsVoicePathEmpty(VoicePath);
	default:
		return false;
	}
}

void UShidenTextCommand::UpdateVoiceState()
{
	if (CurrentVoiceState != EShidenTextCommandVoiceState::Playing)
	{
		return;
	}
	
	if (TotalElapsedTime > VoiceDuration)
	{
		CurrentVoiceState = EShidenTextCommandVoiceState::Completed;
	}
}

void UShidenTextCommand::HandleInputState(const UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager)
{
	UpdateTalkState(ShidenManager);
	UpdateSkipState(ShidenManager, Widget);
}

bool UShidenTextCommand::TrySetTextWindowVisible(UShidenWidget* Widget, FString& ErrorMessage)
{
	bool bSuccess;

	Widget->SetVisibilityByName(TEXT("TextBaseLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextBaseLayer.");
		return false;
	}

	Widget->SetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextLayer.");
		return false;
	}

	return true;
}

bool UShidenTextCommand::TryProcessVoicePlayback(const int32 VoiceTrackId, const FString& VoicePath,
                                                 const TScriptInterface<IShidenManagerInterface>& ShidenManager, FString& ErrorMessage)
{
	if (UShidenScenarioBlueprintLibrary::CanSkipCommand() && bSkipButtonPressed)
	{
		VoiceDuration = 0.f;
		CurrentVoiceState = EShidenTextCommandVoiceState::Completed;
	}
	else
	{
		const FShidenSoundInfo SoundInfo(VoiceTrackId, EShidenSoundType::Voice, VoicePath, 1.f, 1.f, 1.f, 0.f, EAudioFaderCurve::Linear, 0.f);
		bool bSuccess;
		ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, VoiceDuration, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to play voice.");
			return false;
		}
		CurrentVoiceState = EShidenTextCommandVoiceState::Playing;
	}

	return true;
}

bool UShidenTextCommand::TryUpdateTextProgress(const FString& TextWidgetName, const FString& TextType, const bool bInstantTextDisplay,
                                               UShidenWidget* Widget, const float DeltaTime, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	UShidenTextWidget* TextWidget;;
	if (!FindTextWidget(Widget, TextWidgetName, TextWidget, ErrorMessage))
	{
		return false;
	}

	const bool ShouldShowAllTexts = bTalkButtonPressStart || bInstantTextDisplay
		|| ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime == 0.f;

	if (ShouldShowAllTexts)
	{
		CurrentTextIndex = TextLength;
	}
	else
	{
		if (WaitTime > 0.f)
		{
			return true;
		}

		const float CharacterWaitTime = bSkipButtonPressed
			                                ? ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.
			                                SkipSpeedRate
			                                : ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime;

		CurrentTextIndex = FMath::Min(CurrentTextIndex + FMath::CeilToInt32(DeltaTime / CharacterWaitTime), TextLength);
	}

	TextWidget->SetText(TextType, CurrentText, CurrentTextIndex);

	WaitTime = CalculateWaitTime(CurrentTextIndex);

	return true;
}

bool UShidenTextCommand::CanTransitionToComplete(const bool bWaitForInput) const
{
	const bool bIsSkipping = UShidenScenarioBlueprintLibrary::CanSkipCommand() && bSkipButtonPressed;
	return (!bWaitForInput && CurrentVoiceState != EShidenTextCommandVoiceState::Playing)
		|| bIsSkipping
		|| bTalkButtonPressStart
		|| (UShidenBlueprintLibrary::IsAutoTextMode() && WaitTime <= 0.f && CurrentVoiceState != EShidenTextCommandVoiceState::Playing);
}

bool UShidenTextCommand::TryPreviewText(const UShidenWidget* Widget, const FString& TextWidgetName, const FString& TextType,
                                        const FString& Text, const bool bContinueFromThePreviousText, FString& ErrorMessage)
{
	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(Widget, TextWidgetName, TextWidget, ErrorMessage))
	{
		return false;
	}

	bool bSuccess;
	if (bContinueFromThePreviousText)
	{
		FString CurrentPreviewText;
		TextWidget->GetFullText(TextType, CurrentPreviewText, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to get full text of %s."), *TextType);
			return false;
		}
		TextWidget->PreviewText(TextType, FString::Printf(TEXT("%s%s"), *CurrentPreviewText, *Text), bSuccess);
	}
	else
	{
		TextWidget->PreviewText(TextType, Text, bSuccess);
	}

	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to preview text.");
		return false;
	}

	return true;
}

bool UShidenTextCommand::TryGetFullText(UShidenTextWidget* TextWidget, const FString& TextType, FString& Text, FString& ErrorMessage)
{
	bool bSuccess;
	TextWidget->GetFullText(TextType, Text, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get full text of %s."), *TextType);
	}
	return bSuccess;
}

void UShidenTextCommand::OnTextWindowOpenedOrClosed_Implementation()
{
	bIsWaitingForAnimation = false;
}

void UShidenTextCommand::UpdateSkipState(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* Widget)
{
	bool bValue, bSuccess;
	static const TObjectPtr<UInputAction> SkipInputAction = LoadInputActionFromPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenSkip.IA_ShidenSkip"));
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), SkipInputAction, bValue, bSuccess);

	if (Widget->IsSkipPressed() || (bSuccess && bValue))
	{
		bSkipButtonPressStart = !bSkipButtonPressed;
		bSkipButtonPressed = true;
	}
	else
	{
		bSkipButtonPressStart = false;
		bSkipButtonPressed = false;
	}
}

void UShidenTextCommand::UpdateTalkState(const TScriptInterface<IShidenManagerInterface>& ShidenManager)
{
	bool bValue, bSuccess;
	static const TObjectPtr<UInputAction> NextInputAction = LoadInputActionFromPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenNext.IA_ShidenNext"));
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), NextInputAction, bValue, bSuccess);

	if (bSuccess && bValue)
	{
		bTalkButtonPressStart = !bTalkButtonPressed;
		bTalkButtonPressed = true;
	}
	else
	{
		bTalkButtonPressStart = false;
		bTalkButtonPressed = false;
	}
}

float UShidenTextCommand::CalculateWaitTime(const int32 CurrentIndex) const
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (CurrentIndex == TextLength)
	{
		return ShidenSubsystem->PredefinedSystemVariable.AutoModeWaitTime;
	}

	const float BaseWaitTime = bSkipButtonPressed
		                           ? ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.
		                           SkipSpeedRate
		                           : ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime;

	return FMath::Max(BaseWaitTime, UShidenBlueprintLibrary::ParseWaitTimeFromLastTag(CurrentText, CurrentIndex));
}

UInputAction* UShidenTextCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}
