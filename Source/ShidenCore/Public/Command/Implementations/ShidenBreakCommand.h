// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenBreakCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenBreakCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FBreakCommandArgs
	{
		FString ReasonForInterruption;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FBreakCommandArgs& Args);

	FBreakCommandArgs Args;
};
