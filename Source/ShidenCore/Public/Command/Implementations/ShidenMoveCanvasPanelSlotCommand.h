// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenMoveCanvasPanelSlotCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenMoveCanvasPanelSlotCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FMoveCanvasPanelSlotCommandArgs
	{
		FString SlotName;
		EEasingFunc::Type EasingFunction;
		float Duration;
		FString ChangeType;
		FVector2d EndPosition;
		FVector2d EndSize;
		int32 OverwriteZOrder;
		int32 Steps;
		float BlendExp;
		bool bWaitForCompletion;
		bool bChangePosition;
		bool bChangeSize;
		bool bOverwriteZOrder;
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
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
	
	static bool TryAddCurrentValue(const FMoveCanvasPanelSlotCommandArgs& Args, FVector2D OriginalPosition, FVector2D OriginalSize,
	                               UShidenWidget* ShidenWidget, FVector2D& ResultPosition, FVector2D& ResultSize, FString& ErrorMessage);

	static bool TryParseCommand(const FShidenCommand& Command, UShidenWidget* ShidenWidget, FMoveCanvasPanelSlotCommandArgs& Args, FString& ErrorMessage);

	static bool TryStartMoveSlot(const FMoveCanvasPanelSlotCommandArgs& Args, UShidenWidget* ShidenWidget, const FString& ProcessName, FString& ErrorMessage);

	static bool TryOverwriteZOrder(const FMoveCanvasPanelSlotCommandArgs& Args, const UShidenWidget* ShidenWidget, bool bRegisterCurrentProperty,
	                               FString& ErrorMessage);

	static bool TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage);

	FMoveCanvasPanelSlotCommandArgs Args;
};
