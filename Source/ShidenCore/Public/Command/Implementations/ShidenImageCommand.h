// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenImageCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenImageCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FImageCommandArgs
	{
		FString SlotName;
		FString ImagePath;
		FString OverwritePosition;
		FString OverwriteSize;
		FString OverwriteSizeToContent;
		EEasingFunc::Type FadeFunction;
		float FadeDuration;
		int32 Steps;
		float BlendExp;
		bool bWaitForCompletion;
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
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status,
	                                           FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FImageCommandArgs& Args, FString& ErrorMessage);

	static bool TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage);

	static bool TryShowImage(const FImageCommandArgs& Args, UShidenWidget* ShidenWidget, const bool Animate, const FString& OwnerProcessName,
	                         FString& ErrorMessage);

	FImageCommandArgs Args;
};
