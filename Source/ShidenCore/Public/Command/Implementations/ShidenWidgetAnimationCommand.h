// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Blueprint/UserWidget.h"
#include "ShidenWidgetAnimationCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenWidgetAnimationCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FWidgetAnimationCommandArgs
	{
		FString AnimationName;
		float StartTime;
		int32 NumLoopToPlay;
		FString PlayModeStr;
		EUMGSequencePlayMode::Type PlayMode;
		float PlaybackSpeed;
		bool bRestoreState;
		bool bWaitForCompletion;
		UUserWidget* UserWidget;
		UWidgetAnimation* WidgetAnimation;
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties,
	                                                UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
	                                                FString& ErrorMessage) override;

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                              UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status,
	                                              FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const bool bIsCurrentCommand, EShidenPreviewStatus& Status,
	                                           FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, const UShidenWidget* ShidenWidget, FWidgetAnimationCommandArgs& Args,
	                            FString& ErrorMessage);

	void StartAnimation(UShidenWidget* ShidenWidget);

	static bool TryConvertToPlayMode(const FString& PlayModeStr, EUMGSequencePlayMode::Type& PlayMode, FString& ErrorMessage);

	UFUNCTION()
	void OnAnimationFinished();

	FWidgetAnimationCommandArgs Args;

	bool bIsAnimationEnd = false;
};
