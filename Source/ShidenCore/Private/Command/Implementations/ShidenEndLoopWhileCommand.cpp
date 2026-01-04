// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenEndLoopWhileCommand.h"
#include "Engine/Engine.h"

void UShidenEndLoopWhileCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                               const float DeltaTime, UObject* CallerObject,
                                                               EShidenProcessStatus& Status, FString& BreakReason,
                                                               FString& NextScenarioName, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		return;
	}

	const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
	UShidenScenario* Scenario = nullptr;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario from cache for %s."), *ProcessName);
		return;
	}

	Status = TryFindLoopWhileIndex(ProcessName, ScenarioProgress.CurrentIndex, Scenario->Commands, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

bool UShidenEndLoopWhileCommand::TryFindLoopWhileIndex(const FString& ProcessName, const int32& CurrentIndex, const TArray<FShidenCommand>& Commands, FString& ErrorMessage)
{
	for (int32 Index = CurrentIndex - 1; Index >= 0; Index--)
	{
		const FShidenCommand& Command = Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}
		if (Command.CommandName == TEXT("LoopWhile") || Command.CommandName == TEXT("LoopWhileExpression"))
		{
			UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, Index - 1);
			return true;
		}
		if (Command.CommandName == TEXT("EndLoopWhile"))
		{
			int32 ResultIndex;
			if (!TryFindLoopWhileIndexWithoutCheckCondition(ProcessName, Index, Commands, ResultIndex, ErrorMessage))
			{
				return false;
			}
			return TryFindLoopWhileIndex(ProcessName, ResultIndex, Commands, ErrorMessage);
		}
	}

	ErrorMessage = TEXT("Failed to find LoopWhile or LoopWhileExpression command.");
	return false;
}

bool UShidenEndLoopWhileCommand::TryFindLoopWhileIndexWithoutCheckCondition(const FString& ProcessName, const int32& CurrentIndex,
                                                                            const TArray<FShidenCommand>& Commands, int32& ResultIndex, FString& ErrorMessage)
{
	for (int32 Index = CurrentIndex - 1; Index >= 0; Index--)
	{
		const FShidenCommand& Command = Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}
		if (Command.CommandName == TEXT("LoopWhile") || Command.CommandName == TEXT("LoopWhileExpression"))
		{
			ResultIndex = Index;
			return true;
		}
		if (Command.CommandName == TEXT("EndLoopWhile"))
		{
			if (!TryFindLoopWhileIndexWithoutCheckCondition(ProcessName, Index, Commands, ResultIndex, ErrorMessage))
			{
				ResultIndex = 0;
				return false;
			}
			Index = ResultIndex;
		}
	}

	ErrorMessage = TEXT("Failed to find LoopWhile or LoopWhileExpression command.");
	return false;
}
