// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenWidgetAnimationCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenWidgetAnimationCommand::TryParseCommand(const FShidenCommand& Command, const UShidenWidget* ShidenWidget,
                                                    FWidgetAnimationCommandArgs& Args,
                                                    FString& ErrorMessage)
{
	Args.AnimationName = Command.GetArg("AnimationName");
	Args.StartTime = Command.GetArgAsFloat("StartTime");
	Args.NumLoopToPlay = Command.GetArgAsInt("NumLoopToPlay");
	Args.PlayModeStr = Command.GetArg("PlayMode");
	Args.PlaybackSpeed = Command.GetArgAsFloat("PlaybackSpeed");
	Args.bRestoreState = Command.GetArgAsBool("RestoreState");
	Args.bWaitForCompletion = Command.GetArgAsBool("WaitForCompletion");

	if (!ShidenWidget->TryFindAnimation(Args.AnimationName, Args.UserWidget, Args.WidgetAnimation))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find animation %s."), *Args.AnimationName);
		return false;
	}

	return TryConvertToPlayMode(Args.PlayModeStr, Args.PlayMode, ErrorMessage);
}

void UShidenWidgetAnimationCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
                                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                       UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                       FString& ErrorMessage)
{
	for (const TTuple<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
		UUserWidget* UserWidget;
		UWidgetAnimation* WidgetAnimation;
		if (!ShidenWidget->TryFindAnimation(Property.Key, UserWidget, WidgetAnimation))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to find animation %s."), *Property.Key);
			return;
		}

		EUMGSequencePlayMode::Type PlayMode;
		if (!TryConvertToPlayMode(Property.Value.GetValueAsString(), PlayMode, ErrorMessage))
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			return;
		}

		if (PlayMode == EUMGSequencePlayMode::PingPong)
		{
			PlayMode = EUMGSequencePlayMode::Reverse;
		}

		const float EndTime = WidgetAnimation->GetEndTime();
		ShidenWidget->PlayAnimation(WidgetAnimation, EndTime, 1, PlayMode, 1.0f, false);
	}
	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenWidgetAnimationCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                     UShidenWidget* ShidenWidget,
                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                     UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, ShidenWidget, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	StartAnimation(ShidenWidget);
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenWidgetAnimationCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                  UShidenWidget* ShidenWidget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                  FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!bIsAnimationEnd)
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (!Args.bRestoreState)
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Args.AnimationName, Args.PlayModeStr);
	}
	Status = EShidenProcessStatus::Next;
}

void UShidenWidgetAnimationCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, ShidenWidget, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		if (Args.PlayMode == EUMGSequencePlayMode::PingPong)
		{
			Args.PlayMode = EUMGSequencePlayMode::Reverse;
		}
		Args.StartTime = Args.WidgetAnimation->GetEndTime();
		Args.NumLoopToPlay = 1;
		Args.bWaitForCompletion = false;
	}

	StartAnimation(ShidenWidget);
	Status = EShidenPreviewStatus::Complete;
}

void UShidenWidgetAnimationCommand::OnAnimationFinished()
{
	bIsAnimationEnd = true;
}

void UShidenWidgetAnimationCommand::StartAnimation(UShidenWidget* ShidenWidget)
{
	bIsAnimationEnd = !Args.bWaitForCompletion;

	if (!bIsAnimationEnd)
	{
		FWidgetAnimationDynamicEvent WidgetAnimationDynamicEvent;
		WidgetAnimationDynamicEvent.BindDynamic(this, &UShidenWidgetAnimationCommand::OnAnimationFinished);
		ShidenWidget->BindToAnimationFinished(Args.WidgetAnimation, WidgetAnimationDynamicEvent);
	}

	ShidenWidget->PlayAnimation(Args.WidgetAnimation, Args.StartTime, Args.NumLoopToPlay,
	                            Args.PlayMode, Args.PlaybackSpeed, Args.bRestoreState);
}

bool UShidenWidgetAnimationCommand::TryConvertToPlayMode(const FString& PlayModeStr, EUMGSequencePlayMode::Type& PlayMode, FString& ErrorMessage)
{
	static const TMap<FString, EUMGSequencePlayMode::Type> PlayModeMap = {
		{TEXT("Forward"), EUMGSequencePlayMode::Forward},
		{TEXT("Reverse"), EUMGSequencePlayMode::Reverse},
		{TEXT("Ping Pong"), EUMGSequencePlayMode::PingPong}
	};

	if (const EUMGSequencePlayMode::Type* Temp = PlayModeMap.Find(PlayModeStr))
	{
		PlayMode = *Temp;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EUMGSequencePlayMode."), *PlayModeStr);
	PlayMode = EUMGSequencePlayMode::Forward;
	return false;
}
