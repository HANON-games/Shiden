// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenBreakCommand.h"

void UShidenBreakCommand::ParseFromCommand(const FShidenCommand& Command, FBreakCommandArgs& Args)
{
	Args.ReasonForInterruption = Command.GetArg("ReasonForInterruption");
}

void UShidenBreakCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                        const float DeltaTime, UObject* CallerObject,
                                                        EShidenProcessStatus& Status, FString& BreakReason,
                                                        FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	BreakReason = Args.ReasonForInterruption;
	Status = EShidenProcessStatus::Break;
}
