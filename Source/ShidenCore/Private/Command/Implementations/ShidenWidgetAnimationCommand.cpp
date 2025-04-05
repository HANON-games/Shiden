// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenWidgetAnimationCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenWidgetAnimationCommand::TryParseCommand(const FShidenCommand& Command, const UShidenWidget* Widget, FWidgetAnimationCommandArgs& Args,
                                                    FString& ErrorMessage)
{
	Args.AnimationName = Command.GetArg("AnimationName");
	Args.StartTime = Command.GetArg("StartTime");
	Args.NumLoopToPlay = Command.GetArg("NumLoopToPlay");
	const FString PlayModeStr = Command.GetArg("PlayMode");
	Args.PlaybackSpeed = Command.GetArg("PlaybackSpeed");
	Args.RestoreState = Command.GetArg("RestoreState");
	Args.bWaitForCompletion = Command.GetArgAsBool("WaitForCompletion");

	bool bSuccess;
	Widget->FindAnimation(Args.AnimationName, Args.UserWidget, Args.WidgetAnimation, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to find animation %s."), *Args.AnimationName);
		return false;
	}

	return TryConvertToPlayMode(PlayModeStr, Args.PlayMode, ErrorMessage);
}

void UShidenWidgetAnimationCommand::RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties, UShidenWidget* Widget,
                                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                       UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
                                                                       FString& ErrorMessage)
{
	for (const TTuple<FString, FString>& Property : ScenarioProperties)
	{
		UUserWidget* UserWidget;
		UWidgetAnimation* WidgetAnimation;
		bool bSuccess;
		Widget->FindAnimation(Property.Key, UserWidget, WidgetAnimation, bSuccess);
		if (!bSuccess)
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to find animation %s."), *Property.Key);
			return;
		}

		const float EndTime = WidgetAnimation->GetEndTime();
		Widget->PlayAnimation(WidgetAnimation, EndTime, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
	}
	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenWidgetAnimationCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                     UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Widget, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	StartAnimation(Widget);
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenWidgetAnimationCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                  FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!bIsAnimationEnd)
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Args.AnimationName, "");
	Status = EShidenProcessStatus::Next;
}

void UShidenWidgetAnimationCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Widget, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		Args.StartTime = FString::SanitizeFloat(Args.WidgetAnimation->GetEndTime());
	}

	StartAnimation(Widget);
	Status = EShidenPreviewStatus::Complete;
}

void UShidenWidgetAnimationCommand::OnAnimationFinished()
{
	bIsAnimationEnd = true;
}

void UShidenWidgetAnimationCommand::StartAnimation(UShidenWidget* Widget)
{
	bIsAnimationEnd = !Args.bWaitForCompletion;

	if (!bIsAnimationEnd)
	{
		FWidgetAnimationDynamicEvent WidgetAnimationDynamicEvent;
		WidgetAnimationDynamicEvent.BindDynamic(this, &UShidenWidgetAnimationCommand::OnAnimationFinished);
		Widget->BindToAnimationFinished(Args.WidgetAnimation, WidgetAnimationDynamicEvent);
	}

	Widget->PlayAnimation(Args.WidgetAnimation, FCString::Atof(*Args.StartTime), FCString::Atoi(*Args.NumLoopToPlay),
	                      Args.PlayMode, FCString::Atof(*Args.PlaybackSpeed), Args.RestoreState.ToBool());
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
