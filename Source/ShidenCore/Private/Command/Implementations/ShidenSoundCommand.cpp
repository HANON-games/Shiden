// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenSoundCommand.h"

#include "Audio/ShidenSoundFadeType.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenStructuredLog.h"
#include "Sound/SoundBase.h"
#include "Command/ShidenCommandHelpers.h"
#include "Engine/Engine.h"

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
	Args.bUseGlobalBGM = Command.GetArgAsBool(TEXT("UseGlobalBGM"));

	if (!TryConvertToShidenSoundType(Args.SoundTypeStr, Args.SoundType, ErrorMessage))
	{
		return false;
	}

	if (!ShidenCommandHelpers::TryConvertToAudioFaderCurve(FadeFunctionStr, Args.FadeFunction, ErrorMessage))
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

		const FString UseGlobalBGMStr = Values.FindRef(TEXT("UseGlobalBGM"));
		const bool bUseGlobalBGM = UseGlobalBGMStr.Equals(TEXT("true"), ESearchCase::IgnoreCase);
		const int32 TrackId = bUseGlobalBGM && Property.Key.StartsWith(TEXT("Global$"))
			? FCString::Atoi(*Property.Key.RightChop(7))
			: FCString::Atoi(*Property.Key);
		const float Volume = FCString::Atof(**VolumeStr);
		const float Pitch = FCString::Atof(**PitchStr);
		const float StartTime = FCString::Atof(**StartTimeStr);
		const FShidenSoundInfo SoundInfo = FShidenSoundInfo(TrackId, EShidenSoundType::BGM, *Path, 0.0f, Volume, Pitch, StartTime,
		                                                    EAudioFaderCurve::Linear, 0.0f);

		float ResultDuration = 0.0f;
		bool bSuccess;
		if (bUseGlobalBGM)
		{
			UObject* SoundObject;
			if (!UShidenBlueprintLibrary::TryGetOrLoadAsset(*Path, SoundObject))
			{
				Status = EShidenInitFromSaveDataStatus::Error;
				ErrorMessage = FString::Printf(TEXT("Failed to load sound asset from path: %s"), **Path);
				return;
			}

			USoundBase* SoundBase = Cast<USoundBase>(SoundObject);
			if (!SoundBase)
			{
				Status = EShidenInitFromSaveDataStatus::Error;
				ErrorMessage = FString::Printf(TEXT("Failed to cast to USoundBase: %s"), **Path);
				return;
			}
			// Pass false to skip re-saving during restoration
			bSuccess = UShidenBlueprintLibrary::TryPlayGlobalBGM(CallerObject, TrackId, SoundBase, EShidenSoundFadeType::FadeIn,
				Volume, Pitch,StartTime,0.0f, EAudioFaderCurve::Linear, false, ResultDuration);
		}
		else
		{
			ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, ResultDuration, bSuccess);
		}
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
		ShidenManager->Execute_StopSounds(ShidenManager.GetObject(), EShidenSoundType::Voice);
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

	const bool bIsFadeIn = IsFadeIn(Args.FadeType);
	const float ResultStartVolume = bIsFadeIn ? 0.0f : Args.Volume;
	const float ResultEndVolume = bIsFadeIn ? Args.Volume : 0.0f;
	const FShidenSoundInfo SoundInfo(Args.TrackId, Args.SoundType, Args.SoundSourcePath, ResultStartVolume, ResultEndVolume, Args.Pitch,
	                                 Args.StartTime, Args.FadeFunction, Args.FadeDuration);

	bool bSuccess;
	if (Args.bUseGlobalBGM && Args.SoundType == EShidenSoundType::BGM)
	{
		// Load sound asset for TryPlayGlobalBGM
		USoundBase* SoundAsset = nullptr;
		if (!Args.SoundSourcePath.IsEmpty() && Args.SoundSourcePath != TEXT("None"))
		{
			UObject* SoundObject;
			if (UShidenBlueprintLibrary::TryGetOrLoadAsset(Args.SoundSourcePath, SoundObject))
			{
				SoundAsset = Cast<USoundBase>(SoundObject);
			}
		}
		// Pass false - scenario property will be saved in ProcessCommand
		bSuccess = UShidenBlueprintLibrary::TryPlayGlobalBGM(CallerObject, Args.TrackId, SoundAsset, bIsFadeIn ? EShidenSoundFadeType::FadeIn : EShidenSoundFadeType::FadeOut,
			ResultEndVolume, Args.Pitch, Args.StartTime, Args.FadeDuration, Args.FadeFunction, false, SoundDuration);
	}
	else
	{
		ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, SoundDuration, bSuccess);
	}
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
		const FString TrackKey = Args.bUseGlobalBGM
			? TEXT("Global$") + FString::FromInt(Args.TrackId)
			: FString::FromInt(Args.TrackId);

		if (Args.SoundSourcePath.IsEmpty() || Args.SoundSourcePath == TEXT("None"))
		{
			UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(Command.CommandName, TrackKey);
		}
		else
		{
			const float EndVolume = IsFadeIn(Args.FadeType) ? Args.Volume : 0.0f;
			UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(Command.CommandName, TrackKey, {
				{TEXT("Path"), Args.SoundSourcePath},
				{TEXT("Volume"), FString::SanitizeFloat(EndVolume)},
				{TEXT("Pitch"), FString::SanitizeFloat(Args.Pitch)},
				{TEXT("StartTime"), FString::SanitizeFloat(Args.StartTime)},
				{TEXT("UseGlobalBGM"), Args.bUseGlobalBGM ? TEXT("true") : TEXT("false")}
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
	const bool bIsFadeIn = IsFadeIn(Args.FadeType);
	const float ResultStartVolume = bIsFadeIn ? 0.0f : Args.Volume;
	const float ResultEndVolume = bIsFadeIn ? Args.Volume : 0.0f;
	const FShidenSoundInfo SoundInfo(Args.TrackId, Args.SoundType, Args.SoundSourcePath, ResultStartVolume, ResultEndVolume, Args.Pitch,
	                                 Args.StartTime, Args.FadeFunction, Args.FadeDuration);
	bool bSuccess;
	if (Args.bUseGlobalBGM && Args.SoundType == EShidenSoundType::BGM)
	{
		// Load sound asset for TryPlayGlobalBGM
		USoundBase* SoundAsset = nullptr;
		if (!Args.SoundSourcePath.IsEmpty() && Args.SoundSourcePath != TEXT("None"))
		{
			UObject* SoundObject;
			if (UShidenBlueprintLibrary::TryGetOrLoadAsset(Args.SoundSourcePath, SoundObject))
			{
				SoundAsset = Cast<USoundBase>(SoundObject);
			}
		}
		bSuccess = UShidenBlueprintLibrary::TryPlayGlobalBGM(ShidenWidget, Args.TrackId, SoundAsset, bIsFadeIn ? EShidenSoundFadeType::FadeIn : EShidenSoundFadeType::FadeOut,
			ResultEndVolume, Args.Pitch, Args.StartTime, Args.FadeDuration, Args.FadeFunction, false, ResultDuration);
	}
	else
	{
		ShidenManager->Execute_PlaySound(ShidenManager.GetObject(), SoundInfo, true, ResultDuration, bSuccess);
	}
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

	switch (ShidenSubsystem->PredefinedSystemVariable.VoiceStopCondition)
	{
	case EShidenVoiceStopCondition::Never:
		return false;
	case EShidenVoiceStopCondition::NextTextOrVoice:
	case EShidenVoiceStopCondition::NextVoice:
		return !bDisableAutoStopPreviousVoices;
	default:
		SHIDEN_WARNING("Unknown VoiceStopCondition value: {value}", static_cast<int32>(ShidenSubsystem->PredefinedSystemVariable.VoiceStopCondition));
		return false;
	}
}

bool UShidenSoundCommand::TryConvertToShidenSoundType(const FString& SoundTypeStr, EShidenSoundType& SoundType, FString& ErrorMessage)
{
	static const TMap<FString, EShidenSoundType> TypeMap = {
		{TEXT("bgm"), EShidenSoundType::BGM},
		{TEXT("se"), EShidenSoundType::SE},
		{TEXT("voice"), EShidenSoundType::Voice}
	};

	if (const EShidenSoundType* FoundType = TypeMap.Find(SoundTypeStr.ToLower()))
	{
		SoundType = *FoundType;
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Failed to convert %s to EShidenSoundType."), *SoundTypeStr);
	return false;
}

bool UShidenSoundCommand::IsFadeIn(const FString& FadeTypeStr)
{
	return FadeTypeStr.Compare(TEXT("FadeIn"), ESearchCase::IgnoreCase) == 0;
}

UInputAction* UShidenSoundCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}
