// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenario.h"
#include "ShidenRunMacroAsParallelCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenRunMacroAsParallelCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FRunMacroAsParallelCommandArgs
	{
		FString NewProcessName;
		UShidenScenario* Scenario;
		FGuid ScenarioId;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FRunMacroAsParallelCommandArgs& Args, FString& ErrorMessage);

	FRunMacroAsParallelCommandArgs Args;
};
