// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenFadeCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenFadeCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FFadeCommandArgs
	{
		FString LayerName;
		FString FadeType;
		float FadeDuration;
		EEasingFunc::Type FadeFunction;
		FVector TargetColor;
		int32 Steps;
		float BlendExp;
		bool bWaitForCompletion;
		int32 ZOrder;
		bool bUseGlobalFade;
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties,
	                                                UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage) override;

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                              UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FFadeCommandArgs& Args, FString& ErrorMessage);

	static bool TryStartFade(const FFadeCommandArgs& Args, UShidenWidget* ShidenWidget, const FString& OwnerProcessName, FString& ErrorMessage);

	static bool IsFadeOut(const FString& FadeTypeStr);

	FFadeCommandArgs Args;
};
