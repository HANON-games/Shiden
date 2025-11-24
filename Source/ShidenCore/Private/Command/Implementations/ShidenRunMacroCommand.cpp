// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenRunMacroCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

void UShidenRunMacroCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                           UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                           FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	const FString MacroNameOrId = Command.GetArg("MacroName");

	FGuid ScenarioId;
	UShidenScenario* Scenario;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(MacroNameOrId, ScenarioId, Scenario))
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario \"%s\"."), *MacroNameOrId);
		return;
	}

	UShidenScenarioBlueprintLibrary::PushScenario(ProcessName, Scenario);
	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, -1);
	UShidenVariableBlueprintLibrary::InitLocalVariable(ProcessName, Scenario, Command.Args);

	Status = EShidenProcessStatus::Next;
}

void UShidenRunMacroCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           const bool bIsCurrentCommand,
                                                           EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	const FString MacroNameOrId = Command.GetArg("MacroName");

	FGuid ScenarioId;
	UShidenScenario* Scenario;
	if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(MacroNameOrId, ScenarioId, Scenario))
	{
		Status = EShidenPreviewStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario \"%s\"."), *MacroNameOrId);
		return;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	constexpr int32 MaxStackDepth = 50;
	const FShidenScenarioProgressStack* InitialProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(TEXT("Default"));
	if (InitialProgressStack && InitialProgressStack->Stack.Num() > MaxStackDepth)
	{
		Status = EShidenPreviewStatus::Error;
		ErrorMessage = FString::Printf(TEXT("The stack contains too many scenarios to be previewed (limit: %d)."), MaxStackDepth);
		return;
	}

	UShidenScenarioBlueprintLibrary::PushScenario("Default", Scenario);
	UShidenVariableBlueprintLibrary::InitLocalVariable("Default", Scenario, Command.Args);

	while (true)
	{
		const FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(TEXT("Default"));
		if (!ProgressStack)
		{
			Status = EShidenPreviewStatus::Error;
			ErrorMessage = TEXT("ScenarioProgressStack 'Default' not found.");
			return;
		}
		const int32 CurrentIndex = ProgressStack->GetCurrentScenarioIndex();
		FShidenCommand ShidenCommand;
		if (!Scenario->Commands.IsValidIndex(CurrentIndex))
		{
			Status = EShidenPreviewStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Invalid command index %d."), CurrentIndex);
			return;
		}
		UShidenScenarioBlueprintLibrary::ConstructCommand("Default", Scenario->Commands[CurrentIndex], ShidenCommand);
		if (!ShidenCommand.bEnabled)
		{
			int32 NextIndex;
			UShidenScenarioBlueprintLibrary::ToNext("Default", NextIndex);
			if (UShidenScenarioBlueprintLibrary::IsEndOfScenario("Default"))
			{
				FShidenScenarioProgress Progress;
				bool bIsLastElement;
				UShidenScenarioBlueprintLibrary::PopScenario("Default", Progress, bIsLastElement);
				Status = EShidenPreviewStatus::Complete;
				return;
			}
			continue;
		}

		const FShidenCommandDefinition* CommandDefinition = ShidenSubsystem->CommandDefinitionCache.Find(ShidenCommand.CommandName);
		if (!CommandDefinition)
		{
			Status = EShidenPreviewStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to get command definition \"%s\"."), *ShidenCommand.CommandName);
			return;
		}

		if (!CommandDefinition->bCanCallInMacro)
		{
			Status = EShidenPreviewStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Command \"%s\" cannot be called in a macro."), *ShidenCommand.CommandName);
			return;
		}

		UShidenCommandObject* CommandObject;
		if (!UShidenScenarioBlueprintLibrary::TryGetCommand(this, "Default", CommandDefinition->CommandSoftObjectPath, CommandObject))
		{
			Status = EShidenPreviewStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to get command object \"%s\"."), *CommandDefinition->CommandSoftObjectPath.ToString());
			return;
		}

		CommandObject->PreviewCommand(ShidenCommand, ShidenWidget, ShidenManager, bIsCurrentCommand, Status, ErrorMessage);
		if (Status == EShidenPreviewStatus::Error)
		{
			FShidenScenarioProgress Progress;
			bool bIsLastElement;
			UShidenScenarioBlueprintLibrary::PopScenario("Default", Progress, bIsLastElement);
			ErrorMessage = UShidenBlueprintLibrary::MakeErrorMessage(ScenarioId, CurrentIndex, ShidenCommand.CommandName, ErrorMessage);
			return;
		}

		int32 NextIndex;
		UShidenScenarioBlueprintLibrary::ToNext("Default", NextIndex);
		if (UShidenScenarioBlueprintLibrary::IsEndOfScenario("Default"))
		{
			FShidenScenarioProgress Progress;
			bool bIsLastElement;
			UShidenScenarioBlueprintLibrary::PopScenario("Default", Progress, bIsLastElement);
			Status = EShidenPreviewStatus::Complete;
			return;
		}
	}
}
