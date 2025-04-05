// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenEndScenarioCommand.h"

void UShidenEndScenarioCommand::ParseFromCommand(const FShidenCommand& Command, FEndScenarioCommandArgs& Args)
{
	Args.NextScenario = Command.GetArg("NextScenario");
}

void UShidenEndScenarioCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                              UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                              const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                              FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	NextScenarioName = Args.NextScenario;
	Status = EShidenProcessStatus::Complete;
}
