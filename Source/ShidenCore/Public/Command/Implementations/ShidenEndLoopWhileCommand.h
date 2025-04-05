// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "ShidenEndLoopWhileCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenEndLoopWhileCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void FindLoopWhileIndexWithoutCheckCondition(const FString& ProcessName, const int32& CurrentIndex, const TArray<FShidenCommand>& Commands, int32& ResultIndex, bool& bSuccess, FString& ErrorMessage);

	static void FindLoopWhileIndex(const FString& ProcessName, const int32& CurrentIndex, const TArray<FShidenCommand>& Commands, bool& bSuccess, FString& ErrorMessage);
};
