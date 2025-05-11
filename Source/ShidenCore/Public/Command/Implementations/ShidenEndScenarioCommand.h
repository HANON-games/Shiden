// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenEndScenarioCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenEndScenarioCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FEndScenarioCommandArgs
	{
		FString NextScenario;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FEndScenarioCommandArgs& Args);

	FEndScenarioCommandArgs Args;
};
