// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenChangeVisibilityCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenChangeVisibilityCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FChangeVisibilityCommandArgs
	{
		FString Name;
		ESlateVisibility Visibility;
	};

	virtual void RestoreFromSaveData_Implementation(const TMap<FString, FShidenScenarioProperty>& ScenarioProperties, UShidenWidget* ShidenWidget,
	                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                                UObject* CallerObject, EShidenInitFromSaveDataStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FChangeVisibilityCommandArgs& Args, FString& ErrorMessage);

	static bool TryChangeVisibility(const FChangeVisibilityCommandArgs& Args, UShidenWidget* ShidenWidget, bool bRegisterProperty,
	                                FString& ErrorMessage);

	static bool TryConvertToVisibility(const FString& Visibility, ESlateVisibility& Result, FString& ErrorMessage);

	FChangeVisibilityCommandArgs Args;
};
