// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenChangeMaterialScalarParameterCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenChangeMaterialScalarParameterCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FChangeMaterialScalarParameterCommandArgs
	{
		FString Target;
		FString TargetName;
		FString ParameterName;
		EEasingFunc::Type EasingFunction;
		float Duration;
		FString ChangeType;
		float EndValue;
		int32 Steps;
		float BlendExp;
		bool bWaitForCompletion;
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage) override;

	// Restore after brush-setting commands (e.g. Image), since this operates on a dynamic material
	// created from the target's brush. See GetRestoreFromSaveDataPriority in the base class.
	virtual int32 GetRestoreFromSaveDataPriority() const override { return 100; }

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

	static bool TryParseCommand(const FShidenCommand& Command, UShidenWidget* ShidenWidget, FChangeMaterialScalarParameterCommandArgs& Args,
	                            FString& ErrorMessage);

	static bool TryStartChangeParameter(const FChangeMaterialScalarParameterCommandArgs& Args, UShidenWidget* ShidenWidget,
	                                    const FString& ProcessName, FString& ErrorMessage);

	static bool TryResolveEndValue(const FChangeMaterialScalarParameterCommandArgs& Args, float OriginalEndValue, UShidenWidget* ShidenWidget,
	                               float& ResultValue, FString& ErrorMessage);

	FChangeMaterialScalarParameterCommandArgs Args;
};
