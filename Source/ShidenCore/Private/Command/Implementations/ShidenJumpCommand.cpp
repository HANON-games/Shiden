// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenJumpCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

void UShidenJumpCommand::ParseFromCommand(const FShidenCommand& Command, FJumpCommandArgs& Args)
{
	Args.DestinationTagName = Command.GetArg(TEXT("DestinationTagName"));
}

void UShidenJumpCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                       UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                       FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].IsEmpty())
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("ScenarioProgressStack is empty.");
		return;
	}

	const FGuid ScenarioId = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().ScenarioId;
	UShidenScenario* Scenario;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioId, Scenario))
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("GetScenario failed.");
		return;
	}

	int32 TagIndex;
	if (!TryFindTagIndex(Args, Scenario, TagIndex, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, TagIndex);
	Status = EShidenProcessStatus::Next;
}

bool UShidenJumpCommand::TryFindTagIndex(const FJumpCommandArgs& Args, UShidenScenario* Scenario, int32& FoundIndex, FString& ErrorMessage)
{
	for (int32 Index = 0; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& Command = Scenario->Commands[Index];
		if (Command.CommandName == TEXT("Tag") && Command.GetArg(TEXT("Name")) == Args.DestinationTagName)
		{
			FoundIndex = Index;
			return true;
		}
	}

	ErrorMessage = FString::Printf(TEXT("Destination Tag Name \"%s\" is not found."), *Args.DestinationTagName);
	return false;
}
