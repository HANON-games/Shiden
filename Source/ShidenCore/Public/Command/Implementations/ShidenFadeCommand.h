// Copyright (c) 2025 HANON. All Rights Reserved.

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
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FString>& ScenarioProperties,
	                                                UShidenWidget* Widget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status,
	                                                FString& ErrorMessage) override;

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                              UShidenWidget* Widget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status,
	                                              FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status,
	                                           FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FFadeCommandArgs& Args, FString& ErrorMessage);

	static bool TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage);

	static void StartFade(const FFadeCommandArgs& Args, UShidenWidget* Widget, const FString& OwnerProcessName, bool& bSuccess, FString& ErrorMessage);

	static bool IsFadeOut(const FString& FadeTypeStr);

	FFadeCommandArgs Args;
};
