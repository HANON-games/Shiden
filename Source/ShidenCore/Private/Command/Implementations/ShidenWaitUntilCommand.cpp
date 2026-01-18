// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenWaitUntilCommand.h"
#include "Command/ShidenCommandHelpers.h"

void UShidenWaitUntilCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                            UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                            FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!ShidenConditionalCommandHelpers::TryParseVariableCondition(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	bool bResult;
	if (!ShidenConditionalCommandHelpers::TryEvaluateVariableCondition(ProcessName, Args, bResult, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	Status = bResult ? EShidenProcessStatus::Next : EShidenProcessStatus::DelayUntilNextTick;
}
