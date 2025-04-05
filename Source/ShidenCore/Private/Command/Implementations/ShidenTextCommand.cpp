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

	return TryGetTextFromCommand(Command, Args.Text, ErrorMessage);
}

bool UShidenTextCommand::TryGetTextFromCommand(const FShidenCommand& Command, FString& Text, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const FString LanguageArgs[] = {
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

	const int32 LangIndex = ShidenSubsystem->PredefinedSystemVariable.LanguageIndex;
	if (LangIndex >= 0 && LangIndex < 10)
	{
		Text = Command.GetArg(LanguageArgs[LangIndex]);
		return true;
	}

	ErrorMessage = TEXT("Invalid LanguageIndex");
	return false;
}

void UShidenTextCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties,
                                                            UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TPair<FString, FString>& Property : ScenarioProperties)
	{
		FString TextWidgetName;
		FString TextType;
		bool bSuccess = Property.Key.Split(TEXT("::"), &TextWidgetName, &TextType, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to split property name %s."), *Property.Key);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (Property.Value.IsEmpty())
		{
			continue;
		}

		UShidenTextWidget* TextWidget;
		Widget->FindTextWidget(TextWidgetName, TextWidget, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *TextWidgetName);
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		TextWidget->OpenWindow(TextType, FShidenOpenTextWindowDelegate(), bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to open text window.");
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		TextWidget->SetText(TextType, Property.Value, Property.Value.Len());
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
	
	UShidenTextWidget* TextWidget;
	bool bSuccess;
	Widget->FindTextWidget(Args.TextWidgetName, TextWidget, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *Args.TextWidgetName);
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	WaitTime = 0.f;
	TotalElapsedTime = 0.f;

	if (Args.bContinueFromThePreviousText)
	{
		TextWidget->GetFullText(Args.TextType, CurrentText, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to get full text of %s."), *Args.TextType);
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

	Widget->SetVisibilityByName(TEXT("TextBaseLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextBaseLayer.");
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	Widget->SetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextLayer.");
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	bIsWaitingForAnimation = true;
	if (CurrentText.IsEmpty())
	{
		FShidenCloseTextWindowDelegate CloseTextWindowDelegate;
		CloseTextWindowDelegate.BindDynamic(this, &UShidenTextCommand::OnTextWindowOpenedOrClosed);
		TextWidget->CloseWindow(Args.TextType, CloseTextWindowDelegate, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to close text window.");
			Status = EShidenPreProcessStatus::Error;
			return;
		}
	}
	else
	{
		FShidenOpenTextWindowDelegate OpenTextWindowDelegate;
		OpenTextWindowDelegate.BindDynamic(this, &UShidenTextCommand::OnTextWindowOpenedOrClosed);
		TextWidget->OpenWindow(Args.TextType, OpenTextWindowDelegate, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to open text window.");
			Status = EShidenPreProcessStatus::Error;
			return;
		}
	}

	const bool bIsVoicePathEmpty = Args.VoicePath.IsEmpty() || Args.VoicePath == TEXT("None");
	if (ShouldStopVoice(bIsVoicePathEmpty, Args.bDisableAutoStopPreviousVoices))
	{
		ShidenManager->Execute_StopVoices(ShidenManager.GetObject());
	}

	UpdateInput(ShidenManager, Widget);

	const bool bIsSkipping = UShidenScenarioBlueprintLibrary::CanSkipCommand() && bSkipButtonPressed;
	if (!bIsSkipping && !bIsVoicePathEmpty)
	{
		const FShidenSoundInfo SoundInfo(Args.VoiceTrackId, EShidenSoundType::Voice, Args.VoicePath, 1.f, 1.f, 1.f, 0.f, EAudioFaderCurve::Linear,
		                                 0.f);
		ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, VoiceDuration, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to play voice.");
			Status = EShidenPreProcessStatus::Error;
			return;
		}
		VoiceStarted = true;
	}
	else
	{
		VoiceDuration = 0.f;
		VoiceStarted = false;
	}

	Status = EShidenPreProcessStatus::Complete;
}

void UShidenTextCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                       UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                       FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (bIsWaitingForAnimation)
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (Widget->IsMenuOpen() || !Widget->TextBaseLayer->IsVisible())
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	TotalElapsedTime += DeltaTime;
	UpdateInput(ShidenManager, Widget);

	if (!VoiceStarted)
	{
		const bool bIsVoicePathEmpty = Args.VoicePath.IsEmpty() || Args.VoicePath == TEXT("None");
		if (ShouldStopVoice(bIsVoicePathEmpty, Args.bDisableAutoStopPreviousVoices))
		{
			ShidenManager->Execute_StopVoices(ShidenManager.GetObject());
		}

		const bool bIsSkipping = UShidenScenarioBlueprintLibrary::CanSkipCommand() && bSkipButtonPressed;
		if (!bIsSkipping && !bIsVoicePathEmpty)
		{
			const FShidenSoundInfo SoundInfo(Args.VoiceTrackId, EShidenSoundType::Voice, Args.VoicePath, 1.f, 1.f, 1.f, 0.f, EAudioFaderCurve::Linear,
			                                 0.f);
			float ResultDuration;
			bool bSuccess;
			ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, ResultDuration, bSuccess);
			if (!bSuccess)
			{
				ErrorMessage = TEXT("Failed to play voice.");
				Status = EShidenProcessStatus::Error;
				return;
			}
			VoiceStarted = true;
		}
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (bSkipButtonPressStart)
	{
		WaitTime = 0;
	}
	
	if (CurrentTextIndex != TextLength)
	{
		const bool ShouldShowAllTexts = bTalkButtonPressStart || Args.bInstantTextDisplay
			|| ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime == 0.f;

		if (ShouldShowAllTexts)
		{
			CurrentTextIndex = TextLength;
		}
		else
		{
			if (WaitTime > 0.f)
			{
				WaitTime -= DeltaTime;
				Status = EShidenProcessStatus::DelayUntilNextTick;
				return;
			}
			const float CharacterWaitTime = bSkipButtonPressed
				? ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate
				: ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime;
			CurrentTextIndex = FMath::Min(CurrentTextIndex + FMath::CeilToInt32(DeltaTime / CharacterWaitTime), TextLength);
		}

		UShidenTextWidget* TextWidget;
		bool bSuccess;
		Widget->FindTextWidget(Args.TextWidgetName, TextWidget, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *Args.TextWidgetName);
			Status = EShidenProcessStatus::Error;
			return;
		}

		TextWidget->SetText(Args.TextType, CurrentText, CurrentTextIndex);

		WaitTime = TextLength == CurrentTextIndex
			           ? ShidenSubsystem->PredefinedSystemVariable.AutoModeWaitTime
			           : bSkipButtonPressed
							? FMath::Max(ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime / ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate,
			                        UShidenBlueprintLibrary::ParseWaitTimeFromLastTag(CurrentText, CurrentTextIndex))
				            : FMath::Max(ShidenSubsystem->PredefinedSystemVariable.CharacterWaitTime,
									UShidenBlueprintLibrary::ParseWaitTimeFromLastTag(CurrentText, CurrentTextIndex));

		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	const bool bIsSkipping = UShidenScenarioBlueprintLibrary::CanSkipCommand() && bSkipButtonPressed;
	if ((!Args.bWaitForInput && TotalElapsedTime > VoiceDuration)
		|| bIsSkipping
		|| bTalkButtonPressStart
		|| (UShidenBlueprintLibrary::IsAutoTextMode() && WaitTime <= 0.f && TotalElapsedTime > VoiceDuration))
	{
		UShidenTextWidget* TextWidget;
		bool bSuccess;
		Widget->FindTextWidget(Args.TextWidgetName, TextWidget, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *Args.TextWidgetName);
			Status = EShidenProcessStatus::Error;
			return;
		}

		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName,
																 FString::Printf(TEXT("%s::%s"), *Args.TextWidgetName, *Args.TextType), CurrentText);

		TMap<FString, FString> Texts;
		TextWidget->GetAllFullTexts(Texts);
		UShidenBlueprintLibrary::AddBacklogItem(Command, Texts);
		Status = EShidenProcessStatus::Next;
		return;
	}

	WaitTime -= DeltaTime;
	Status = EShidenProcessStatus::DelayUntilNextTick;
}

void UShidenTextCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                       EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	UShidenTextWidget* TextWidget;
	bool bSuccess;
	Widget->FindTextWidget(Args.TextWidgetName, TextWidget, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find text widget %s."), *Args.TextWidgetName);
		Status = EShidenPreviewStatus::Error;
		return;
	}

	const bool bIsVoicePathEmpty = Args.VoicePath.IsEmpty() || Args.VoicePath == TEXT("None");
	if (bIsCurrentCommand && !bIsVoicePathEmpty)
	{
		const FShidenSoundInfo SoundInfo(Args.VoiceTrackId, EShidenSoundType::Voice, Args.VoicePath, 1.f, 1.f, 1.f, 0.f, EAudioFaderCurve::Linear,
		                                 0.f);
		ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, VoiceDuration, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = TEXT("Failed to play voice.");
			Status = EShidenPreviewStatus::Error;
			return;
		}
	}

	Widget->SetVisibilityByName(TEXT("TextBaseLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextBaseLayer.");
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Widget->SetVisibilityByName(TEXT("TextLayer"), ESlateVisibility::SelfHitTestInvisible, true, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to set visibility of TextLayer.");
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (Args.bContinueFromThePreviousText)
	{
		TextWidget->GetFullText(Args.TextType, CurrentText, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to get full text of %s."), *Args.TextType);
			Status = EShidenPreviewStatus::Error;
			return;
		}
		CurrentText = FString::Printf(TEXT("%s%s"), *CurrentText, *Args.Text);
		TextWidget->PreviewText(Args.TextType, CurrentText, bSuccess);
	}
	else
	{
		TextWidget->PreviewText(Args.TextType, Args.Text, bSuccess);
	}

	if (!bSuccess)
	{
		ErrorMessage = TEXT("Failed to preview text.");
		Status = EShidenPreviewStatus::Error;
		return;
	}

	Status = EShidenPreviewStatus::Complete;
}


bool UShidenTextCommand::ShouldStopVoice(const bool& bIsVoicePathEmpty, const bool& bDisableAutoStopPreviousVoices)
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
		return !bDisableAutoStopPreviousVoices && !bIsVoicePathEmpty;
	}
	return false;
}

void UShidenTextCommand::UpdateInput(const TScriptInterface<IShidenManagerInterface>& ShidenManager, const UShidenWidget* Widget)
{
	bool bValue;
	bool bSuccess;

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

UInputAction* UShidenTextCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}

void UShidenTextCommand::OnTextWindowOpenedOrClosed()
{
	bIsWaitingForAnimation = false;
}
