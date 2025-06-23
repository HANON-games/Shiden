// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenElseIfCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

void UShidenElseIfCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                         const float DeltaTime, UObject* CallerObject,
                                                         EShidenProcessStatus& Status, FString& BreakReason,
                                                         FString& NextScenarioName, FString& ErrorMessage)
{
	Status = TryFindEndIfIndex(ProcessName, ErrorMessage) ? EShidenProcessStatus::Next : EShidenProcessStatus::Error;
}

void UShidenElseIfCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                         EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	Status = TryFindEndIfIndex("Default", ErrorMessage) ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

bool UShidenElseIfCommand::TryFindEndIfIndex(const FString& ProcessName, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].IsEmpty())
	{
		ErrorMessage = TEXT("Failed to find EndIf command.");
		return false;
	}

	const FShidenScenarioProgress ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();
	UShidenScenario* Scenario = nullptr;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario for %s."), *ProcessName);
		return false;
	}

	int32 Depth = 0;
	for (int32 Index = ScenarioProgress.CurrentIndex; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& Command = Scenario->Commands[Index];
		if (!Command.bEnabled)
		{
			continue;
		}

		if (Command.CommandName == TEXT("If"))
		{
			Depth++;
		}
		else if (Command.CommandName == TEXT("EndIf"))
		{
			if (Depth == 0)
			{
				UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, Index);
				return true;
			}
			Depth--;
		}
	}

	ErrorMessage = TEXT("Failed to find EndIf command.");
	return false;
}
