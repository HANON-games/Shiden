// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "ShidenAutoSaveCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenAutoSaveCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FAutoSaveCommandArgs
	{
		FString SlotName;
		FString OverwriteThumbnail;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FAutoSaveCommandArgs& Args);

	static bool TryExecuteAutoSave(const FAutoSaveCommandArgs& Args, const FString& ProcessName, UShidenWidget* Widget, FString& ErrorMessage);

	static bool IsWidgetInitialized(const UShidenWidget* Widget);

	FAutoSaveCommandArgs Args;
};
