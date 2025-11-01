// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenAdjustBGMVolumeCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenAdjustBGMVolumeCommand::TryParseCommand(const FShidenCommand& Command, FAdjustVolumeCommandArgs& Args, FString& ErrorMessage)
{
	Args.TrackId = Command.GetArgAsInt("TrackId");
	Args.Volume = Command.GetArgAsFloat("Volume");
	const FString FadeFunctionStr = Command.GetArg("FadeFunction");
	Args.FadeDuration = Command.GetArgAsFloat("FadeDuration");
	Args.bWaitForCompletion = Command.GetArgAsBool("WaitForCompletion");

	if (Args.TrackId < 0)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid TrackId: %d. Must be non-negative."), Args.TrackId);
		return false;
	}

	if (Args.Volume < 0.0f)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid Volume: %f. Must be non-negative."), Args.Volume);
		return false;
	}

	if (Args.FadeDuration < 0.0f)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid FadeDuration: %f. Must be non-negative."), Args.FadeDuration);
		return false;
	}

	return TryConvertToAudioFaderCurve(FadeFunctionStr, Args.FadeFunction, ErrorMessage);
}

void UShidenAdjustBGMVolumeCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                     UShidenWidget* ShidenWidget,
                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                     UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	ElapsedTime = 0.0f;
	ShidenManager->Execute_AdjustBGMVolume(ShidenManager.GetObject(), Args.TrackId, Args.FadeDuration, Args.Volume, Args.FadeFunction);
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenAdjustBGMVolumeCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                  UShidenWidget* ShidenWidget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                  FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (Args.bWaitForCompletion)
	{
		ElapsedTime += DeltaTime;
		if (ElapsedTime < Args.FadeDuration)
		{
			Status = EShidenProcessStatus::DelayUntilNextTick;
			return;
		}
	}

	// Update the volume property in the scenario
	const FString PropertyKey = FString::FromInt(Args.TrackId);
	FShidenScenarioProperty ScenarioProperty;
	if (UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(TEXT("Sound"), PropertyKey, ScenarioProperty))
	{
		TMap<FString, FString> ScenarioProperties;
		ScenarioProperty.TryConvertToStringMap(ScenarioProperties);
		ScenarioProperties.Add(TEXT("Volume"), FString::SanitizeFloat(Args.Volume));
		UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(TEXT("Sound"), PropertyKey, ScenarioProperties);
	}

	Status = EShidenProcessStatus::Next;
}

void UShidenAdjustBGMVolumeCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		Args.FadeDuration = 0.0f;
	}

	ShidenManager->Execute_AdjustBGMVolume(ShidenManager.GetObject(), Args.TrackId, Args.FadeDuration, Args.Volume, Args.FadeFunction);
	Status = EShidenPreviewStatus::Complete;
}

bool UShidenAdjustBGMVolumeCommand::TryConvertToAudioFaderCurve(const FString& AudioFaderCurveStr, EAudioFaderCurve& AudioFaderCurve,
                                                                FString& ErrorMessage)
{
	static const TMap<FString, EAudioFaderCurve> CurveMap = {
		{TEXT("linear"), EAudioFaderCurve::Linear},
		{TEXT("logarithmic"), EAudioFaderCurve::Logarithmic},
		{TEXT("sin (s-curve)"), EAudioFaderCurve::SCurve},
		{TEXT("sin (equal power)"), EAudioFaderCurve::Sin}
	};

	if (const EAudioFaderCurve* FoundCurve = CurveMap.Find(AudioFaderCurveStr.ToLower()))
	{
		AudioFaderCurve = *FoundCurve;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EAudioFaderCurve."), *AudioFaderCurveStr);
	AudioFaderCurve = EAudioFaderCurve::Linear;
	return false;
}
