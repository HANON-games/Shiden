// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Command/ShidenCommandHelpers.h"
#include "ShidenWaitUntilCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenWaitUntilCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	ShidenConditionalCommandHelpers::FVariableConditionArgs Args;
};
