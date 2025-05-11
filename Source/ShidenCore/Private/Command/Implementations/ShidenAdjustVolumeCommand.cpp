// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenAdjustVolumeCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

bool UShidenAdjustVolumeCommand::TryParseCommand(const FShidenCommand& Command, FAdjustVolumeCommandArgs& Args, FString& ErrorMessage)
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

	if (Args.Volume < 0.0f || Args.Volume > 1.0f)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid Volume: %f. Must be between 0.0 and 1.0."), Args.Volume);
		return false;
	}

	if (Args.FadeDuration < 0.0f)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid FadeDuration: %f. Must be non-negative."), Args.FadeDuration);
		return false;
	}

	return TryConvertToAudioFaderCurve(FadeFunctionStr, Args.FadeFunction, ErrorMessage);
}

void UShidenAdjustVolumeCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
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

void UShidenAdjustVolumeCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
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

	const FString Key = FString::Printf(TEXT("%s::Volume"), *FString::FromInt(Args.TrackId));
	UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(Command.CommandName, Key, FString::SanitizeFloat(Args.Volume));
	Status = EShidenProcessStatus::Next;
}

void UShidenAdjustVolumeCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
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

bool UShidenAdjustVolumeCommand::TryConvertToAudioFaderCurve(const FString& AudioFaderCurveStr, EAudioFaderCurve& AudioFaderCurve,
                                                             FString& ErrorMessage)
{
	static const TMap<FString, EAudioFaderCurve> CurveMap = {
		{TEXT("Linear"), EAudioFaderCurve::Linear},
		{TEXT("Logarithmic"), EAudioFaderCurve::Logarithmic},
		{TEXT("Sin (S-Curve)"), EAudioFaderCurve::SCurve},
		{TEXT("Sin (Equal Power)"), EAudioFaderCurve::Sin}
	};

	if (const EAudioFaderCurve* FoundCurve = CurveMap.Find(AudioFaderCurveStr))
	{
		AudioFaderCurve = *FoundCurve;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EAudioFaderCurve."), *AudioFaderCurveStr);
	AudioFaderCurve = EAudioFaderCurve::Linear;
	return false;
}
