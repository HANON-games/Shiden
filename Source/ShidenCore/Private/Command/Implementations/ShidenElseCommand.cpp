// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenElseCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "Engine/Engine.h"

void UShidenElseCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                       FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	Status = TryFindAndSetEndIfIndex(ProcessName, ErrorMessage) ? EShidenProcessStatus::Next : EShidenProcessStatus::Error;
}

void UShidenElseCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                       const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                       EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	Status = TryFindAndSetEndIfIndex(TEXT("Default"), ErrorMessage) ? EShidenPreviewStatus::Complete : EShidenPreviewStatus::Error;
}

bool UShidenElseCommand::TryFindAndSetEndIfIndex(const FString& ProcessName, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		return false;
	}

	const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
	int32 EndIfIndex;
	if (!ShidenConditionalCommandHelpers::TryFindEndIfIndex(ProcessName, ScenarioProgress.CurrentIndex, EndIfIndex, ErrorMessage))
	{
		return false;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, EndIfIndex);
	return true;
}
