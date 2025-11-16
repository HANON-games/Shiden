// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Audio/ShidenSoundType.h"
#include "ShidenSoundCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenSoundCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FSoundCommandArgs
	{
		FString SoundTypeStr;
		int32 TrackId;
		FString SoundSourcePath;
		float Volume;
		float Pitch;
		float StartTime;
		FString FadeType;
		EAudioFaderCurve FadeFunction;
		float FadeDuration;
		bool bDisableAutoStopPreviousVoices;
		bool bWaitForFadeCompletion;
		bool bWaitForSoundCompletion;
		EShidenSoundType SoundType;
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage) override;

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool ShouldStopVoice(const bool bDisableAutoStopPreviousVoices);

	static bool TryConvertToShidenSoundType(const FString& SoundTypeStr, EShidenSoundType& SoundType, FString& ErrorMessage);

	static UInputAction* LoadInputActionFromPath(const FString& Path);

	static bool TryParseCommand(const FShidenCommand& Command, FSoundCommandArgs& Args, FString& ErrorMessage);

	FSoundCommandArgs Args;

	float SoundDuration = 0.0f;

	float ElapsedTime = 0.0f;
};
