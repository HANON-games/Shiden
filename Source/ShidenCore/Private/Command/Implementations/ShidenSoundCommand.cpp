// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenSoundCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Sound/SoundBase.h"

bool UShidenSoundCommand::TryParseCommand(const FShidenCommand& Command, FSoundCommandArgs& Args, FString& ErrorMessage)
{
	Args.SoundTypeStr = Command.GetArg(TEXT("SoundType"));
	Args.TrackId = Command.GetArgAsInt(TEXT("TrackId"));
	Args.SoundSourcePath = Command.GetArg(TEXT("SoundSource"));
	Args.Volume = Command.GetArgAsFloat(TEXT("Volume"));
	Args.Pitch = Command.GetArgAsFloat(TEXT("Pitch"));
	Args.StartTime = Command.GetArgAsFloat(TEXT("StartTime"));
	Args.FadeType = Command.GetArg(TEXT("FadeType"));
	const FString FadeFunctionStr = Command.GetArg(TEXT("FadeFunction"));
	Args.FadeDuration = Command.GetArgAsFloat(TEXT("FadeDuration"));
	Args.bDisableAutoStopPreviousVoices = Command.GetArgAsBool(TEXT("DisableAutoStopPreviousVoices"));
	Args.bWaitForFadeCompletion = Command.GetArgAsBool(TEXT("WaitForFadeCompletion"));
	Args.bWaitForSoundCompletion = Command.GetArgAsBool(TEXT("WaitForSoundCompletion"));

	if (!TryConvertToShidenSoundType(Args.SoundTypeStr, Args.SoundType, ErrorMessage))
	{
		return false;
	}

	if (!TryConvertToAudioFaderCurve(FadeFunctionStr, Args.FadeFunction, ErrorMessage))
	{
		return false;
	}

	if (Args.SoundType == EShidenSoundType::SE &&
		(Args.SoundSourcePath.IsEmpty() || Args.SoundSourcePath == TEXT("None")))
	{
		ErrorMessage = TEXT("Sound source path is empty while sound type is SE.");
		return false;
	}

	if (Args.SoundSourcePath.IsEmpty() || Args.SoundSourcePath == TEXT("None"))
	{
		return true;
	}
	
	UObject* SoundObject;
	if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(Args.SoundSourcePath, SoundObject))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get or load asset from %s."), *Args.SoundSourcePath);
		return false;
	}

	const TObjectPtr<USoundBase> SoundBase = Cast<USoundBase>(SoundObject);
	if (!SoundBase)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to cast %s to USoundBase."), *Args.SoundSourcePath);
		return false;
	}

	if (Args.bWaitForSoundCompletion && !SoundBase->IsOneShot())
	{
		ErrorMessage = TEXT("Cannot wait for sound completion of looping sound.");
		return false;
	}

	EShidenSoundType ObjectSoundType;
	if (!UShidenBlueprintLibrary::TryGetSoundTypeFromSoundBase(SoundBase, ObjectSoundType))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get sound type from %s."), *Args.SoundSourcePath);
		return false;
	}

	if (Args.SoundType != ObjectSoundType)
	{
		ErrorMessage = FString::Printf(TEXT("Sound type %s is not matched with %s."), *Args.SoundTypeStr, *Args.SoundSourcePath);
		return false;
	}

	return true;
}

void UShidenSoundCommand::RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
                                                             UShidenWidget* ShidenWidget,
                                                             const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                             UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage)
{
	for (const TPair<FString, FShidenScenarioProperty>& Property : ScenarioProperties)
	{
		FString SoundTrackStr = Property.Key;

		TMap<FString, FString> Values;
		if (!Property.Value.TryConvertToStringMap(Values))
		{
			continue;
		}

		const FString* VolumeStr = Values.Find(TEXT("Volume"));
		const FString* PitchStr = Values.Find(TEXT("Pitch"));
		const FString* StartTimeStr = Values.Find(TEXT("StartTime"));
		if (!VolumeStr || !PitchStr || !StartTimeStr)
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = TEXT("Failed to find Volume or Pitch or StartTime.");
			return;
		}

		const FString* Path = Values.Find(TEXT("Path"));
		if (!Path)
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = TEXT("Failed to find Path.");
			return;
		}
		const int32 TrackId = FCString::Atoi(*SoundTrackStr);
		const float Volume = FCString::Atof(**VolumeStr);
		const float Pitch = FCString::Atof(**PitchStr);
		const float StartTime = FCString::Atof(**StartTimeStr);
		const FShidenSoundInfo SoundInfo = FShidenSoundInfo(TrackId, EShidenSoundType::BGM, *Path, 0.0f, Volume, Pitch, StartTime,
															EAudioFaderCurve::Linear, 0.0f);

		float ResultDuration = 0.0f;
		bool bSuccess;
		ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, ResultDuration, bSuccess);
		if (!bSuccess)
		{
			Status = EShidenInitFromSaveDataStatus::Error;
			ErrorMessage = TEXT("Failed to play sound.");
			return;
		}
	}

	Status = EShidenInitFromSaveDataStatus::Complete;
}

void UShidenSoundCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                           UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	ElapsedTime = 0.0f;

	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	if (Args.SoundType == EShidenSoundType::Voice && ShouldStopVoice(Args.bDisableAutoStopPreviousVoices))
	{
		ShidenManager->Execute_StopVoices(ShidenManager.GetObject());
	}

	if ((Args.SoundType == EShidenSoundType::SE || Args.SoundType == EShidenSoundType::Voice)
		&& UShidenScenarioBlueprintLibrary::CanSkipCommand())
	{
		static const TObjectPtr<UInputAction> SkipInputAction = LoadInputActionFromPath(
			TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenSkip.IA_ShidenSkip"));
		bool bValue, bSuccess;
		ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), SkipInputAction, bValue, bSuccess);
		if (ShidenWidget->IsSkipPressed() || (bSuccess && bValue))
		{
			SoundDuration = 0.f;
			Args.FadeDuration = 0.f;
			Status = EShidenPreProcessStatus::Complete;
			return;
		}
	}

	const float ResultStartVolume = Args.FadeType == TEXT("FadeIn") ? 0.0f : Args.Volume;
	const float ResultEndVolume = Args.FadeType == TEXT("FadeIn") ? Args.Volume : 0.0f;
	const FShidenSoundInfo SoundInfo(Args.TrackId, Args.SoundType, Args.SoundSourcePath, ResultStartVolume, ResultEndVolume, Args.Pitch,
	                                 Args.StartTime, Args.FadeFunction, Args.FadeDuration);

	bool bSuccess;
	ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, SoundDuration, bSuccess);
	if (!bSuccess)
	{
		Status = EShidenPreProcessStatus::Error;
		ErrorMessage = TEXT("Failed to play sound.");
		return;
	}

	Status = EShidenPreProcessStatus::Complete;
}

void UShidenSoundCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                        UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                        const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                        FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	ElapsedTime += DeltaTime;

	if (Args.bWaitForFadeCompletion && ElapsedTime < Args.FadeDuration)
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (Args.bWaitForSoundCompletion && ElapsedTime < SoundDuration)
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (Args.SoundType == EShidenSoundType::BGM)
	{
		const FString TrackIdStr = FString::FromInt(Args.TrackId);
		if (Args.SoundSourcePath.IsEmpty() || Args.SoundSourcePath == TEXT("None"))
		{
			UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(Command.CommandName, TrackIdStr);
		}
		else
		{
				const float EndVolume = Args.FadeType == TEXT("FadeIn") ? Args.Volume : 0.0f;
				UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(Command.CommandName, TrackIdStr, {
				{TEXT("Path"), Args.SoundSourcePath},
				{TEXT("Volume"), FString::SanitizeFloat(EndVolume)},
				{TEXT("Pitch"), FString::SanitizeFloat(Args.Pitch)},
				{TEXT("StartTime"), FString::SanitizeFloat(Args.StartTime)}
			});
		}
	}

	Status = EShidenProcessStatus::Next;
}

void UShidenSoundCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager, const bool bIsCurrentCommand,
                                                        EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	if (!bIsCurrentCommand)
	{
		Args.FadeDuration = 0.f;
	}

	if ((Args.SoundType == EShidenSoundType::SE || Args.SoundType == EShidenSoundType::Voice) && !bIsCurrentCommand)
	{
		Status = EShidenPreviewStatus::Complete;
		return;
	}

	float ResultDuration = 0.f;
	const float ResultStartVolume = Args.FadeType == TEXT("FadeIn") ? 0.0f : Args.Volume;
	const float ResultEndVolume = Args.FadeType == TEXT("FadeIn") ? Args.Volume : 0.0f;
	const FShidenSoundInfo SoundInfo(Args.TrackId, Args.SoundType, Args.SoundSourcePath, ResultStartVolume, ResultEndVolume, Args.Pitch,
	                                 Args.StartTime, Args.FadeFunction, Args.FadeDuration);
	bool bSuccess;
	ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, ResultDuration, bSuccess);
	if (!bSuccess)
	{
		Status = EShidenPreviewStatus::Error;
		ErrorMessage = TEXT("Failed to play sound.");
		return;
	}

	Status = EShidenPreviewStatus::Complete;
}

bool UShidenSoundCommand::ShouldStopVoice(const bool bDisableAutoStopPreviousVoices)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	switch (ShidenSubsystem->PredefinedSystemVariable.VoiceStopCondition)
	{
	case EShidenVoiceStopCondition::Never:
		return false;
	case EShidenVoiceStopCondition::NextTextOrVoice:
	case EShidenVoiceStopCondition::NextVoice:
		return !bDisableAutoStopPreviousVoices;
	}
	return false;
}

bool UShidenSoundCommand::TryConvertToAudioFaderCurve(const FString& AudioFaderCurveStr, EAudioFaderCurve& AudioFaderCurve, FString& ErrorMessage)
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
	return false;
}

bool UShidenSoundCommand::TryConvertToShidenSoundType(const FString& SoundTypeStr, EShidenSoundType& SoundType, FString& ErrorMessage)
{
	static const TMap<FString, EShidenSoundType> TypeMap = {
		{TEXT("BGM"), EShidenSoundType::BGM},
		{TEXT("SE"), EShidenSoundType::SE},
		{TEXT("Voice"), EShidenSoundType::Voice}
	};

	if (const EShidenSoundType* FoundType = TypeMap.Find(SoundTypeStr))
	{
		SoundType = *FoundType;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenSoundType."), *SoundTypeStr);
	return false;
}

UInputAction* UShidenSoundCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}
