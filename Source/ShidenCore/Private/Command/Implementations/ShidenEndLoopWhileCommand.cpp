// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenEndLoopWhileCommand.h"

void UShidenEndLoopWhileCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                               const float DeltaTime, UObject* CallerObject,
                                                               EShidenProcessStatus& Status, FString& BreakReason,
                                                               FString& NextScenarioName, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("Failed to find EndLoopWhile command.");
		return;
	}

	const FShidenScenarioProgress ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();
	UShidenScenario* Scenario = nullptr;
	bool bSuccess;
	UShidenScenarioBlueprintLibrary::GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
	if (!bSuccess)
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario from cache for %s."), *ProcessName);
		return;
	}

	FindLoopWhileIndex(ProcessName, ScenarioProgress.CurrentIndex, Scenario->Commands, bSuccess, ErrorMessage);

	Status = bSuccess
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

void UShidenEndLoopWhileCommand::FindLoopWhileIndex(const FString& ProcessName, const int32& CurrentIndex, const TArray<FShidenCommand>& Commands,
                                                    bool& bSuccess, FString& ErrorMessage)
{
	for (int32 Index = CurrentIndex - 1; Index >= 0; Index--)
	{
		const FShidenCommand& Command = Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}
		if (Command.CommandName == TEXT("LoopWhile"))
		{
			bSuccess = true;
			UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, Index - 1);
			return;
		}
		if (Command.CommandName == TEXT("EndLoopWhile"))
		{
			int32 ResultIndex;
			FindLoopWhileIndexWithoutCheckCondition(ProcessName, Index, Commands, ResultIndex, bSuccess, ErrorMessage);
			if (!bSuccess)
			{
				return;
			}
			FindLoopWhileIndex(ProcessName, ResultIndex, Commands, bSuccess, ErrorMessage);
			return;
		}
	}

	bSuccess = false;
	ErrorMessage = TEXT("Failed to find LoopWhile command.");
}

void UShidenEndLoopWhileCommand::FindLoopWhileIndexWithoutCheckCondition(const FString& ProcessName, const int32& CurrentIndex,
                                                                         const TArray<FShidenCommand>& Commands, int32& ResultIndex, bool& bSuccess,
                                                                         FString& ErrorMessage)
{
	for (int32 Index = CurrentIndex - 1; Index >= 0; Index--)
	{
		const FShidenCommand& Command = Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}
		if (Command.CommandName == TEXT("LoopWhile"))
		{
			ResultIndex = Index;
			bSuccess = true;
			return;
		}
		if (Command.CommandName == TEXT("EndLoopWhile"))
		{
			FindLoopWhileIndexWithoutCheckCondition(ProcessName, ResultIndex, Commands, ResultIndex, bSuccess, ErrorMessage);
			if (!bSuccess)
			{
				ResultIndex = 0;
				return;
			}
			FindLoopWhileIndexWithoutCheckCondition(ProcessName, ResultIndex, Commands, ResultIndex, bSuccess, ErrorMessage);
			return;
		}
	}

	bSuccess = false;
	ErrorMessage = TEXT("Failed to find LoopWhile command.");
}
