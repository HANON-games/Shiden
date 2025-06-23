// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenWaitClickCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "EnhancedInputComponent.h"

void UShidenWaitClickCommand::ParseFromCommand(const FShidenCommand& Command, FWaitClickCommandArgs& Args)
{
	Args.bCanSkip = Command.GetArgAsBool("CanSkip");
}

void UShidenWaitClickCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                               UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                               UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	bPressNext = true;
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenWaitClickCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                            UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                            FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	const bool bCanSkipCommand = UShidenScenarioBlueprintLibrary::CanSkipCommand();

	if (Args.bCanSkip && bCanSkipCommand)
	{
		static const UInputAction* SkipInputAction = LoadInputActionFromPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenSkip.IA_ShidenSkip"));
		bool bValue, bSuccess;
		ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), SkipInputAction, bValue, bSuccess);
		if (bValue && bSuccess)
		{
			Status = EShidenProcessStatus::Next;
			return;
		}
	}

	static const UInputAction* NextInputAction = LoadInputActionFromPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenNext.IA_ShidenNext"));
	bool bValue, bSuccess;
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), NextInputAction, bValue, bSuccess);
	if (!bSuccess)
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	if (bValue && !bPressNext)
	{
		Status = EShidenProcessStatus::Next;
	}
	else
	{
		bPressNext = bValue;
		Status = EShidenProcessStatus::DelayUntilNextTick;
	}
}

UInputAction* UShidenWaitClickCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}
