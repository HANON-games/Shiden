// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenTextCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenStructuredLog.h"
#include "Engine/Engine.h"

void UShidenTextCommand::ParseCommand(const FShidenCommand& Command, FTextCommandArgs& OutArgs)
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

	OutArgs.Texts.SetNum(MaxLanguageCount);
	static const TArray<FString> LanguageArgs = {
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
	for (int32 i = 0; i < MaxLanguageCount; i++)
	{
		OutArgs.Texts[i] = Command.GetArg(LanguageArgs[i]);
	}
}

void UShidenTextCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TPair<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
		TArray<FString> Texts;
		if (!Property.Value.TryConvertToStringArray(Texts))
		{
			continue;
		}

		FString TextWidgetName, TextType;
		if (!TryParsePropertyKey(Property.Key, TextWidgetName, TextType, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		int32 LanguageIndex;
		if (!TryGetLanguageIndex(LanguageIndex, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (!TryRestoreTextWidget(ShidenWidget, TextWidgetName, TextType, GetTextByLanguageIndex(Texts, LanguageIndex), ErrorMessage))
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
	ParseCommand(Command, Args);

	HandleInputState(ShidenWidget, ShidenManager);

	CurrentState = EShidenTextCommandState::WaitingForAnimation;
	CurrentVoiceState = EShidenTextCommandVoiceState::None;
	WaitTime = 0.f;
	TotalElapsedTime = 0.f;
	VoiceDuration = 0.f;
	bIsWaitingForAnimation = true;

	int32 LanguageIndex;
	if (!TryGetLanguageIndex(LanguageIndex, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	if (Args.bContinueFromThePreviousText)
	{
		FShidenScenarioProperty ScenarioProperty;
		UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(Command.CommandName,
		                                                         FString::Printf(TEXT("%s::%s"), *Args.TextWidgetName, *Args.TextType),
		                                                         ScenarioProperty);
		TArray<FString> PreviousTexts;
		const FString PreviousText = ScenarioProperty.TryConvertToStringArray(PreviousTexts)
			                             ? GetTextByLanguageIndex(PreviousTexts, LanguageIndex)
			                             : TEXT("");

		CurrentTextIndex = UShidenBlueprintLibrary::GetParsedLength(PreviousText);
		CurrentTexts.Empty();
		for (int32 i = 0; i < MaxLanguageCount; i++)
		{
			CurrentTexts.Add(FString::Printf(TEXT("%s%s"), *GetTextByLanguageIndex(PreviousTexts, i), *GetTextByLanguageIndex(Args.Texts, i)));
		}
		CurrentText = GetTextByLanguageIndex(CurrentTexts, LanguageIndex);
	}
	else
	{
		CurrentTextIndex = -1;
		CurrentTexts = Args.Texts;
		CurrentText = GetTextByLanguageIndex(Args.Texts, LanguageIndex);
	}

	TextLength = UShidenBlueprintLibrary::GetParsedLength(CurrentText);

	if (!TrySetTextWindowVisible(ShidenWidget, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	UShidenTextWidget* TextWidget;
	if (!TryFindTextWidget(ShidenWidget, Args.TextWidgetName, TextWidget, ErrorMessage))
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
		ShidenManager->Execute_StopSounds(ShidenManager.GetObject(), EShidenSoundType::Voice);
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
			if (!TryUpdateTextProgress(Args.TextWidgetName, Args.TextType, Args.bInstantTextDisplay, ShidenWidget, DeltaTime, bTextUpdated,
			                           ErrorMessage))
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
			UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromArray(Command.CommandName,
			                                                                   FString::Printf(TEXT("%s::%s"), *Args.TextWidgetName, *Args.TextType),
			                                                                   CurrentTexts);
			UShidenTextWidget* TextWidget;
			if (!TryFindTextWidget(ShidenWidget, Args.TextWidgetName, TextWidget, ErrorMessage))
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
	default:
		Status = EShidenProcessStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Unknown text command state: %d"), static_cast<int32>(CurrentState));
		break;
	}
}

void UShidenTextCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                       EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	ParseCommand(Command, Args);

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

	Status = TryPreviewText(ShidenWidget, Args.TextWidgetName, Args.TextType, Args.Texts, Args.bContinueFromThePreviousText, ErrorMessage)
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

bool UShidenTextCommand::ShouldPlayTextBlip(const FString& TextBlipPath, const FString& VoicePath)
{
	if (CurrentTextIndex <= 0)
	{
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	if (IsAssetPathEmpty(VoicePath) && !IsAssetPathEmpty(TextBlipPath))
	{
		switch (ShidenSubsystem->PredefinedSystemVariable.TextBlipTriggerMode)
		{
		case EShidenTextBlipTriggerMode::TimeInterval:
			return TextBlipWaitTime <= 0.f;
		case EShidenTextBlipTriggerMode::CharacterInterval:
			return TextBlipCharacterCount <= 0;
		default:
			SHIDEN_WARNING("Unknown TextBlipTriggerMode: {mode}", static_cast<int32>(ShidenSubsystem->PredefinedSystemVariable.TextBlipTriggerMode));
			return false;
		}
	}
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UShidenTextCommand::IsAssetPathEmpty(const FString& VoicePath)
{
	return VoicePath.IsEmpty() || VoicePath == TEXT("None");
}

bool UShidenTextCommand::IsTextDisplayComplete() const noexcept
{
	return CurrentTextIndex == TextLength;
}

bool UShidenTextCommand::TryCloseTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage)
{
	FShidenCloseTextWindowDelegate CloseTextWindowDelegate;
	CloseTextWindowDelegate.BindDynamic(this, &UShidenTextCommand::OnTextWindowOpenedOrClosed);
	bool bSuccess;
	TextWidget->CloseWindow(TextType, CloseTextWindowDelegate, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to close text window.");
		return false;
	}
	return true;
}

bool UShidenTextCommand::TryOpenTextWindow(UShidenTextWidget* TextWidget, const FString& TextType, FString& ErrorMessage)
{
	FShidenOpenTextWindowDelegate OpenTextWindowDelegate;
	OpenTextWindowDelegate.BindDynamic(this, &UShidenTextCommand::OnTextWindowOpenedOrClosed);
	bool bSuccess;
	TextWidget->OpenWindow(TextType, OpenTextWindowDelegate, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to open text window.");
		return false;
	}
	return true;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UShidenTextCommand::TryFindTextWidget(const UShidenWidget* ShidenWidget, const FString& TextWidgetName, UShidenTextWidget*& TextWidget,
                                           FString& ErrorMessage)
{
	if (ShidenWidget->TryFindTextWidget(TextWidgetName, TextWidget))
	{
		return true;
	}
	ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *TextWidgetName);
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UShidenTextCommand::TryParsePropertyKey(const FString& PropertyKey, FString& TextWidgetName, FString& TextType, FString& ErrorMessage)
{
	if (PropertyKey.Split(TEXT("::"), &TextWidgetName, &TextType, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		return true;
	}
	ErrorMessage = FString::Printf(TEXT("Failed to split property name %s."), *PropertyKey);
	return false;
}

bool UShidenTextCommand::TryRestoreTextWidget(UShidenWidget* ShidenWidget, const FString& TextWidgetName, const FString& TextType,
                                              const FString& TextValue, FString& ErrorMessage)
{
	UShidenTextWidget* TextWidget;
	if (!TryFindTextWidget(ShidenWidget, TextWidgetName, TextWidget, ErrorMessage))
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

// ReSharper disable once CppMemberFunctionMayBeStatic
bool UShidenTextCommand::ShouldPauseTextProcess(const UShidenWidget* ShidenWidget)
{
	return ShidenWidget->IsMenuOpen() || !ShidenWidget->TextBaseLayer->IsVisible();
}

bool UShidenTextCommand::ShouldStopVoice(const FString& VoicePath, const bool bDisableAutoStopPreviousVoices)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	switch (ShidenSubsystem->PredefinedSystemVariable.VoiceStopCondition)
	{
	case EShidenVoiceStopCondition::Never:
		return false;
	case EShidenVoiceStopCondition::NextTextOrVoice:
		return !bDisableAutoStopPreviousVoices;
	case EShidenVoiceStopCondition::NextVoice:
		return !bDisableAutoStopPreviousVoices && !IsAssetPathEmpty(VoicePath);
	default:
		SHIDEN_WARNING("Unknown VoiceStopCondition value: {value}", static_cast<int32>(ShidenSubsystem->PredefinedSystemVariable.VoiceStopCondition));
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

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeStatic
bool UShidenTextCommand::TrySetTextWindowVisible(UShidenWidget* ShidenWidget, FString& ErrorMessage)
{
	if (!ShidenWidget->TrySetVisibilityByName(TEXT("TextBaseLayer"), ESlateVisibility::SelfHitTestInvisible, true))
	{
		ErrorMessage = TEXT("Failed to set visibility of TextBaseLayer.");
		return false;
	}

	if (!ShidenWidget->TrySetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::SelfHitTestInvisible, true))
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

bool UShidenTextCommand::TryProcessTextBlipPlayback(const FString& TextBlipPath, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                    FString& ErrorMessage)
{
	const FShidenSoundInfo SoundInfo(-1, EShidenSoundType::Voice, TextBlipPath, 1.f, 1.f, 1.f, 0.f, EAudioFaderCurve::Linear, 0.f);
	float TextBlipDuration;
	bool bSuccess;
	ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, false, TextBlipDuration, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to play text blip.");
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

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

	UShidenTextWidget* TextWidget;
	if (!TryFindTextWidget(ShidenWidget, TextWidgetName, TextWidget, ErrorMessage))
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

		const float CharacterWaitTime = bSkipButtonPressed && ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate > 0.f
			                                ? ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate
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
                                        const TArray<FString>& Texts, const bool bContinueFromThePreviousText, FString& ErrorMessage)
{
	UShidenTextWidget* TextWidget;
	if (!TryFindTextWidget(ShidenWidget, TextWidgetName, TextWidget, ErrorMessage))
	{
		return false;
	}

	int32 LanguageIndex;
	if (!TryGetLanguageIndex(LanguageIndex, ErrorMessage))
	{
		return false;
	}

	bool bSuccess;
	if (bContinueFromThePreviousText)
	{
		// It should be retrieved from ScenarioProperty,
		// but since preview does not save to ScenarioProperty, it is retrieved from TextWidget instead.
		FString CurrentPreviewText;
		// Even if Text cannot be retrieved, TryPreviewText will still be executed
		TryGetFullText(TextWidget, TextType, CurrentPreviewText, ErrorMessage);
		TextWidget->PreviewText(TextType, FString::Printf(TEXT("%s%s"), *CurrentPreviewText, *GetTextByLanguageIndex(Texts, LanguageIndex)), bSuccess);
	}
	else
	{
		TextWidget->PreviewText(TextType, GetTextByLanguageIndex(Texts, LanguageIndex), bSuccess);
	}

	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to preview text.");
		return false;
	}
	return true;
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeStatic
bool UShidenTextCommand::TryGetFullText(UShidenTextWidget* TextWidget, const FString& TextType, FString& Text, FString& ErrorMessage)
{
	bool bSuccess;
	TextWidget->GetFullText(TextType, Text, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get full text of %s."), *TextType);
		return false;
	}
	return true;
}

void UShidenTextCommand::OnTextWindowOpenedOrClosed_Implementation()
{
	bIsWaitingForAnimation = false;
}

void UShidenTextCommand::UpdateSkipState(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* ShidenWidget)
{
	bool bValue, bSuccess;
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), GetSkipInputAction(), bValue, bSuccess);

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
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), GetNextInputAction(), bValue, bSuccess);

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

	if (CurrentIndex == TextLength)
	{
		return ShidenSubsystem->PredefinedSystemVariable.AutoModeWaitTime;
	}

	const float BaseWaitTime = bSkipButtonPressed && ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate > 0.f
		                           ? ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate
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

bool UShidenTextCommand::TryGetLanguageIndex(int32& LanguageIndex, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	LanguageIndex = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
	if (LanguageIndex < 0 || LanguageIndex >= MaxLanguageCount)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid LanguageIndex: %d. Must be between 0 and %d."), LanguageIndex, MaxLanguageCount - 1);
		return false;
	}
	return true;
}

UInputAction* UShidenTextCommand::GetSkipInputAction() const
{
	if (!SkipInputAction.IsValid())
	{
		return SkipInputAction.LoadSynchronous();
	}
	return SkipInputAction.Get();
}

UInputAction* UShidenTextCommand::GetNextInputAction() const
{
	if (!NextInputAction.IsValid())
	{
		return NextInputAction.LoadSynchronous();
	}
	return NextInputAction.Get();
}

FString UShidenTextCommand::GetTextByLanguageIndex(const TArray<FString>& Texts, const int32 LanguageIndex)
{
	return Texts.IsValidIndex(LanguageIndex) ? Texts[LanguageIndex] : TEXT("");
}
