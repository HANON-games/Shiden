// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenAdjustVolumeCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenAdjustVolumeCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FAdjustVolumeCommandArgs
	{
		int32 TrackId;
		float Volume;
		EAudioFaderCurve FadeFunction;
		float FadeDuration;
		bool bWaitForCompletion;
	};

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

	static bool TryParseCommand(const FShidenCommand& Command, FAdjustVolumeCommandArgs& Args, FString& ErrorMessage);

	static bool TryConvertToAudioFaderCurve(const FString& AudioFaderCurveStr, EAudioFaderCurve& AudioFaderCurve, FString& ErrorMessage);

	FAdjustVolumeCommandArgs Args;

	float ElapsedTime = 0.0f;
};
