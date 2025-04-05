// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenRunMacroCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"

bool UShidenRunMacroCommand::TryParseCommand(const FShidenCommand& Command, FRunMacroCommandArgs& Args, FString& ErrorMessage)
{
	const FString MacroNameOrId = Command.GetArg("MacroName");

	bool bSuccess;
	UShidenScenarioBlueprintLibrary::GetScenarioByIdOrObjectPath(MacroNameOrId, Args.ScenarioId, Args.Scenario, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to get scenario \"%s\"."), *MacroNameOrId);
		return false;
	}

	return true;
}

void UShidenRunMacroCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                           UShidenWidget* Widget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                           FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenProcessStatus::Error;
		return;
	}

	UShidenScenarioBlueprintLibrary::PushScenario(ProcessName, Args.Scenario);
	UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(ProcessName, -1);
	UShidenVariableBlueprintLibrary::InitLocalVariable(ProcessName, Args.Scenario, Command.Args);

	Status = EShidenProcessStatus::Next;
}

void UShidenRunMacroCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* Widget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                           EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	if (!TryParseCommand(Command, Args, ErrorMessage))
	{
		Status = EShidenPreviewStatus::Error;
		return;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.FindRef("Default").Stack.Num() > 50)
	{
		Status = EShidenPreviewStatus::Error;
		ErrorMessage = TEXT("The stack contains too many scenarios to be previewed.");
		return;
	}

	UShidenScenarioBlueprintLibrary::PushScenario("Default", Args.Scenario);
	UShidenVariableBlueprintLibrary::InitLocalVariable("Default", Args.Scenario, Command.Args);

	while (true)
	{
		const int32 CurrentIndex = ShidenSubsystem->ScenarioProgressStack["Default"].Stack.Last().CurrentIndex;
		FShidenCommand ShidenCommand;
		UShidenScenarioBlueprintLibrary::ConstructCommand("Default", Args.Scenario->Commands[CurrentIndex], ShidenCommand);
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
		bool bSuccess;
		UShidenScenarioBlueprintLibrary::GetCommandFromCache(this, "Default", CommandDefinition->CommandSoftObjectPath, CommandObject, bSuccess);
		if (!bSuccess)
		{
			Status = EShidenPreviewStatus::Error;
			ErrorMessage = FString::Printf(TEXT("Failed to get command object \"%s\"."), *CommandDefinition->CommandSoftObjectPath.ToString());
			return;
		}

		CommandObject->PreviewCommand(ShidenCommand, Widget, ShidenManager, bIsCurrentCommand, Status, ErrorMessage);
		if (Status == EShidenPreviewStatus::Error)
		{
			FShidenScenarioProgress Progress;
			bool bIsLastElement;
			UShidenScenarioBlueprintLibrary::PopScenario("Default", Progress, bIsLastElement);
			ErrorMessage = UShidenBlueprintLibrary::MakeErrorMessage(Args.ScenarioId, CurrentIndex, ShidenCommand.CommandName, ErrorMessage);
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
