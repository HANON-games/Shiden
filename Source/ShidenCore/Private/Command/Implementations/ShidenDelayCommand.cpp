// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenDelayCommand.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

void UShidenDelayCommand::ParseFromCommand(const FShidenCommand& Command, FDelayCommandArgs& Args)
{
	Args.Duration = Command.GetArgAsFloat(TEXT("Duration"));
}

void UShidenDelayCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject,
                                                           EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	ElapsedTime = 0.0f;
	Status = EShidenPreProcessStatus::Complete;
}

void UShidenDelayCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                        const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                        FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	static const TObjectPtr<UInputAction> InputAction = LoadInputActionFromPath(TEXT("/Shiden/Misc/EnhancedInput/IA_ShidenSkip.IA_ShidenSkip"));
	if (!InputAction)
	{
		Status = EShidenProcessStatus::Error;
		ErrorMessage = TEXT("Failed to load input action.");
		return;
	}

	bool bValue;
	bool bSuccess;
	ShidenManager->Execute_FindShidenDigitalInput(ShidenManager.GetObject(), InputAction, bValue, bSuccess);

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ElapsedTime += bValue && bSuccess && UShidenScenarioBlueprintLibrary::CanSkipCommand()
		               ? DeltaTime * ShidenSubsystem->PredefinedSystemVariable.SkipSpeedRate
		               : DeltaTime;

	Status = ElapsedTime < Args.Duration ? EShidenProcessStatus::DelayUntilNextTick : EShidenProcessStatus::Next;
}

UInputAction* UShidenDelayCommand::LoadInputActionFromPath(const FString& Path)
{
	return Cast<UInputAction>(StaticLoadObject(UInputAction::StaticClass(), nullptr, *Path));
}
