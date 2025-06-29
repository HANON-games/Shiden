// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenRunMacroAsParallelCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

bool UShidenRunMacroAsParallelCommand::TryParseCommand(const FShidenCommand& Command, FRunMacroAsParallelCommandArgs& Args, FString& ErrorMessage)
{
	Args.NewProcessName = Command.GetArg("NewProcessName");
	const FString MacroIdOrPath = Command.GetArg("MacroName");

	if (Args.NewProcessName == TEXT("Default"))
	{
		ErrorMessage = TEXT("Cannot use \"Default\" as parallel process name.");
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(Args.NewProcessName))
	{
		ErrorMessage = FString::Printf(TEXT("The process name %s already running."), *Args.NewProcessName);
		return false;
	}

	if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(MacroIdOrPath, Args.ScenarioId, Args.Scenario))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario %s."), *MacroIdOrPath);
		return false;
	}

	return true;
}

void UShidenRunMacroAsParallelCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                     UShidenWidget* ShidenWidget,
                                                                     const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                     const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                     FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	UShidenScenarioBlueprintLibrary::PushScenario(Args.NewProcessName, Args.Scenario);
	UShidenVariableBlueprintLibrary::InitLocalVariable(Args.NewProcessName, Args.Scenario, Command.Args);

	ShidenManager->Execute_CallMacroAsParallel(ShidenManager.GetObject(), Args.NewProcessName, CallerObject);

	Status = EShidenProcessStatus::Next;
}
