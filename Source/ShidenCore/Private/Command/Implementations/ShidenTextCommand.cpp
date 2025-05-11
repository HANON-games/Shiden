// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenTextCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

bool UShidenTextCommand::TryParseCommand(const FShidenCommand& Command, FTextCommandArgs& OutArgs, FString& ErrorMessage)
{
	OutArgs.TextWidgetName = Command.GetArg("TextWidgetName");
	OutArgs.TextType = Command.GetArg("TextType");
	OutArgs.bWaitForInput = Command.GetArgAsBool("WaitForInput");
	OutArgs.bInstantTextDisplay = Command.GetArgAsBool("InstantTextDisplay");
	OutArgs.VoicePath = Command.GetArg("Voice");
	OutArgs.TextBlipPath = Command.GetArg("TextBlip");
	OutArgs.VoiceTrackId = Command.GetArgAsInt("VoiceTrackId");
	OutArgs.bDisableAutoStopPreviousVoices = Command.GetArgAsBool("DisableAutoStopPreviousVoices");
	OutArgs.bContinueFromThePreviousText = Command.GetArgAsBool("ContinueFromThePreviousText");

	return TryGetLanguageSpecificText(Command, OutArgs.Text, ErrorMessage);
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

void UShidenTextCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* ShidenWidget,
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

		if (!TryRestoreTextWidget(ShidenWidget, TextWidgetName, TextType, Property.Value, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenTextCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                          UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                          UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	HandleInputState(ShidenWidget, ShidenManager);

	CurrentState = EShidenTextCommandState::WaitingForAnimation;
	CurrentVoiceState = EShidenTextCommandVoiceState::None;
	WaitTime = 0.f;
	TotalElapsedTime = 0.f;
	VoiceDuration = 0.f;
	bIsWaitingForAnimation = true;

	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(ShidenWidget, Args.TextWidgetName, TextWidget, ErrorMessage))
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

	if (!TrySetTextWindowVisible(ShidenWidget, ErrorMessage))
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
                                                       UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                       FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	HandleInputState(ShidenWidget, ShidenManager);
	UpdateTime(DeltaTime);
	UpdateVoiceState();

	switch (CurrentState)
	{
	case EShidenTextCommandState::WaitingForAnimation:
		{
			if (!bIsWaitingForAnimation)
			{
				CurrentState = ShouldPauseTextProcess(ShidenWidget)
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
			if (!ShouldPauseTextProcess(ShidenWidget))
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
			if (ShouldPauseTextProcess(ShidenWidget))
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
			bool bTextUpdated;
			if (!TryUpdateTextProgress(Args.TextWidgetName, Args.TextType, Args.bInstantTextDisplay, ShidenWidget, DeltaTime, bTextUpdated, ErrorMessage))
			{
				Status = EShidenProcessStatus::Error;
				return;
			}
			if (bTextUpdated && ShouldPlayTextBlip(Args.TextBlipPath, Args.VoicePath))
			{
				if (!TryProcessTextBlipPlayback(Args.TextBlipPath, ShidenManager, ErrorMessage))
				{
					Status = EShidenProcessStatus::Error;
					return;
				}
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
			if (ShouldPauseTextProcess(ShidenWidget))
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
			if (!FindTextWidget(ShidenWidget, Args.TextWidgetName, TextWidget, ErrorMessage))
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

void UShidenTextCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                       EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!TrySetTextWindowVisible(ShidenWidget, ErrorMessage))
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

	Status = TryPreviewText(ShidenWidget, Args.TextWidgetName, Args.TextType, Args.Text, Args.bContinueFromThePreviousText, ErrorMessage)
		         ? EShidenPreviewStatus::Complete
		         : EShidenPreviewStatus::Error;
}

void UShidenTextCommand::UpdateTime(const float DeltaTime)
{
	TotalElapsedTime += DeltaTime;
	TextBlipWaitTime -= DeltaTime;
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
	return !IsAssetPathEmpty(VoicePath) && VoiceState == EShidenTextCommandVoiceState::None;
}

bool UShidenTextCommand::ShouldPlayTextBlip(const FString& TextBlipPath, const FString& VoicePath) const
{
	if (CurrentTextIndex <= 0)
	{
		return false;
	}
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (IsAssetPathEmpty(VoicePath) && !IsAssetPathEmpty(TextBlipPath))
	{
		switch (ShidenSubsystem->PredefinedSystemVariable.TextBlipTriggerMode) {
		case EShidenTextBlipTriggerMode::TimeInterval:
			return TextBlipWaitTime <= 0.f;
		case EShidenTextBlipTriggerMode::CharacterInterval:
			return TextBlipCharacterCount <= 0;
		}
	}
	return false;
}

bool UShidenTextCommand::IsAssetPathEmpty(const FString& VoicePath)
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

bool UShidenTextCommand::FindTextWidget(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, UShidenTextWidget*& TextWidget,
                                        FString& ErrorMessage)
{
	bool bSuccess;
	ShidenWidget->FindTextWidget(TextWidgetName, TextWidget, bSuccess);
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

bool UShidenTextCommand::TryRestoreTextWidget(UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
                                              const FString& TextValue, FString& ErrorMessage)
{
	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(ShidenWidget, TextWidgetName, TextWidget, ErrorMessage))
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

bool UShidenTextCommand::ShouldPauseTextProcess(const UShidenWidget* ShidenWidget)
{
	return ShidenWidget->IsMenuOpen() || !ShidenWidget->TextBaseLayer->IsVisible();
}

bool UShidenTextCommand::ShouldStopVoice(const FString& VoicePath, const bool bDisableAutoStopPreviousVoices)
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
		return !bDisableAutoStopPreviousVoices && !IsAssetPathEmpty(VoicePath);
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

void UShidenTextCommand::HandleInputState(const UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager)
{
	UpdateTalkState(ShidenManager);
	UpdateSkipState(ShidenManager, ShidenWidget);
}

bool UShidenTextCommand::TrySetTextWindowVisible(UShidenWidget* ShidenWidget, FString& ErrorMessage)
{
	bool bSuccess;

	ShidenWidget->SetVisibilityByName(TEXT("TextBaseLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextBaseLayer.");
		return false;
	}

	ShidenWidget->SetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
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

bool UShidenTextCommand::TryProcessTextBlipPlayback(const FString& TextBlipPath, const TScriptInterface<IShidenManagerInterface>& ShidenManager, FString& ErrorMessage)
{
	const FShidenSoundInfo SoundInfo(-1, EShidenSoundType::Voice, TextBlipPath, 1.f, 1.f, 1.f, 0.f, EAudioFaderCurve::Linear, 0.f);
	bool bSuccess;
	float TextBlipDuration;
	ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, false, TextBlipDuration, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to play text blip.");
		return false;
	}
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	if (ShidenSubsystem->PredefinedSystemVariable.TextBlipTriggerMode == EShidenTextBlipTriggerMode::TimeInterval)
	{
		TextBlipWaitTime = ShidenSubsystem->PredefinedSystemVariable.BlipIntervalSeconds;
	}
	else
	{
		TextBlipCharacterCount = ShidenSubsystem->PredefinedSystemVariable.BlipIntervalCharacters;
	}
	return true;
}

bool UShidenTextCommand::TryUpdateTextProgress(const FString& TextWidgetName, const FString& TextType, const bool bInstantTextDisplay,
                                               UShidenWidget* ShidenWidget, const float DeltaTime, bool& bTextUpdated, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(ShidenWidget, TextWidgetName, TextWidget, ErrorMessage))
	{
		bTextUpdated = false;
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
			bTextUpdated = false;
			return true;
		}

		const float CharacterWaitTime = bSkipButtonPressed
			                                ? ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.
			                                SkipSpeedRate
			                                : ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime;
		const int32 NextIndex = FMath::Min(CurrentTextIndex + FMath::CeilToInt32(DeltaTime / CharacterWaitTime), TextLength);
		TextBlipCharacterCount -= NextIndex - CurrentTextIndex;
		CurrentTextIndex = NextIndex;

	}
	
	TextWidget->SetText(TextType, CurrentText, CurrentTextIndex);
	WaitTime = CalculateWaitTime(CurrentTextIndex);
	bTextUpdated = true;
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

bool UShidenTextCommand::TryPreviewText(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
                                        const FString& Text, const bool bContinueFromThePreviousText, FString& ErrorMessage)
{
	UShidenTextWidget* TextWidget;
	if (!FindTextWidget(ShidenWidget, TextWidgetName, TextWidget, ErrorMessage))
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

void UShidenTextCommand::UpdateSkipState(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* ShidenWidget)
{
	bool bValue, bSuccess;
	static const TObjectPtr<UInputAction> SkipInputAction = LoadInputActionFromPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenSkip.IA_ShidenSkip"));
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), SkipInputAction, bValue, bSuccess);

	if (ShidenWidget->IsSkipPressed() || (bSuccess && bValue))
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

float UShidenTextCommand::CalculateWaitTime(const int32 CurrentIndex)
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

	float WaitTimeFromTag;
	const bool bFound = UShidenBlueprintLibrary::TryParseWaitTimeFromLastTag(CurrentText, CurrentIndex, WaitTimeFromTag);
	if (bFound)
	{
		TextBlipCharacterCount = 0;
		TextBlipWaitTime = 0.f;
	}
	
	return bFound ? FMath::Max(BaseWaitTime, WaitTimeFromTag) : BaseWaitTime;
}

UInputAction* UShidenTextCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}
