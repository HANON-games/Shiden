// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenStopParallelProcessCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "Engine/Engine.h"

bool UShidenStopParallelProcessCommand::TryParseCommand(const FShidenCommand& Command, FStopParallelProcessArgs& Args, FString& ErrorMessage)
{
	Args.ProcessName = Command.GetArg("ProcessName");
	Args.Reason = Command.GetArg("Reason");
	Args.StopTiming = Command.GetArg("StopTiming");
	Args.bWaitForCompletion = Command.GetArgAsBool("WaitForCompletion");

	if (Args.StopTiming.Compare("Immediately", ESearchCase::IgnoreCase) != 0
		&& Args.StopTiming.Compare("OnCurrentCommandEnd", ESearchCase::IgnoreCase) != 0)
	{
		ErrorMessage = FString::Printf(TEXT("Invalid StopTiming: %s"), *Args.StopTiming);
		return false;
	}

	return true;
}

void UShidenStopParallelProcessCommand::PreProcessCommand_Implementation(const FString& ProcessName,
                                                                         const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                         UObject* CallerObject,
                                                                         EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreProcessStatus::Error;
		return;
	}

	const EShidenCancelType CancelType = Args.StopTiming.Compare("Immediately", ESearchCase::IgnoreCase) == 0
		                                     ? EShidenCancelType::Immediately
		                                     : EShidenCancelType::OnCurrentCommandEnd;

	UShidenScenarioBlueprintLibrary::RequestCancelScenario(Args.ProcessName, Args.Reason, CancelType);
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenStopParallelProcessCommand::ProcessCommand_Implementation(const FString& ProcessName,
                                                                      const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                      const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                      const float DeltaTime,
                                                                      UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                                      FString& NextScenarioName, FString& ErrorMessage)
{
	if (!Args.bWaitForCompletion)
	{
		Status = EShidenProcessStatus::Next;
		return;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	Status = ShidenSubsystem->ScenarioProgressStack.Contains(Args.ProcessName)
		         ? EShidenProcessStatus::DelayUntilNextTick
		         : EShidenProcessStatus::Next;
}
