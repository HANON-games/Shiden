// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenElseIfCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

void UShidenElseIfCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* Widget,
                                                                  const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                  const float DeltaTime, UObject* CallerObject,
                                                                  EShidenProcessStatus& Status, FString& BreakReason,
                                                                  FString& NextScenarioName, FString& ErrorMessage)
{
	bool bSuccess;
	FindEndIfIndex(ProcessName, bSuccess, ErrorMessage);
	Status = bSuccess ? EShidenProcessStatus::Next : EShidenProcessStatus::Error;
}

void UShidenElseIfCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
																 const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	bool bSuccess;
	FindEndIfIndex("Default", bSuccess, ErrorMessage);
	Status = bSuccess ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

void UShidenElseIfCommand::FindEndIfIndex(const FString& ProcessName, bool& bSuccess, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		bSuccess = false;
		ErrorMessage = TEXT("Failed to find EndIf command.");
		return;
	}

	const FShidenScenarioProgress ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();
	UShidenScenario* Scenario = nullptr;
	UShidenScenarioBlueprintLibrary::GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);

	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario from cache for %s."), *ProcessName);
		return;
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
				bSuccess = true;
				return;
			}
			Depth--;
		}
	}
	
	bSuccess = false;
	ErrorMessage = TEXT("Failed to find EndIf command.");
}
