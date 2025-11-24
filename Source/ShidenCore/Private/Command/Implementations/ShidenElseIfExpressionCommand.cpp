// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenElseIfExpressionCommand.h"
#include "Command/ShidenCommandHelpers.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "Engine/Engine.h"

void UShidenElseIfExpressionCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                                   UShidenWidget* ShidenWidget,
                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                   const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                                   FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		Status = EShidenProcessStatus::Error;
		return;
	}

	const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
	int32 EndIfIndex;
	if (!ShidenConditionalCommandHelpers::TryFindEndIfIndex(ProcessName, ScenarioProgress.CurrentIndex, EndIfIndex, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, EndIfIndex);
	Status = EShidenProcessStatus::Next;
}

void UShidenElseIfExpressionCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                   const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                   bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage
)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(TEXT("Default"));
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		Status = EShidenPreviewStatus::Error;
		return;
	}

	const FShidenScenarioProgress ScenarioProgress = ProgressStack->Stack.Last();
	int32 EndIfIndex;
	if (!ShidenConditionalCommandHelpers::TryFindEndIfIndex(TEXT("Default"), ScenarioProgress.CurrentIndex, EndIfIndex, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(TEXT("Default"), EndIfIndex);
	Status = EShidenPreviewStatus::Complete;
}
