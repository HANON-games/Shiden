// Copyright (c) 2025 HANON. All Rights Reserved.

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

	static FString MakeScenarioPropertyKey(const FString& TargetType, const FString& TargetName, const FString& ParameterName);

	static TTuple<FString, FString, FString> ParseScenarioPropertyKey(const FString& Key);

	static bool TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage);

	static bool TryAddCurrentValue(const FChangeMaterialScalarParameterCommandArgs& Args, float OriginalEndValue, UShidenWidget* ShidenWidget,
	                               float& ResultValue, FString& ErrorMessage);

	FChangeMaterialScalarParameterCommandArgs Args;
};
