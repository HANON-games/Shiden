// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenAutoSaveCommand.h"
#include "System/ShidenSubsystem.h"
#include "System/ShidenBlueprintLibrary.h"

void UShidenAutoSaveCommand::ParseFromCommand(const FShidenCommand& Command, FAutoSaveCommandArgs& Args)
{
	Args.SlotName = Command.GetArg(TEXT("SlotName"));
	Args.OverwriteThumbnail = Command.GetArg(TEXT("OverwriteThumbnail"));
}

void UShidenAutoSaveCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                           UShidenWidget* ShidenWidget,
                                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                           FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!IsWidgetInitialized(ShidenWidget))
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	ParseFromCommand(Command, Args);

	Status = TryExecuteAutoSave(Args, ProcessName, ShidenWidget, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}

bool UShidenAutoSaveCommand::IsWidgetInitialized(const UShidenWidget* ShidenWidget)
{
	const FVector2d DesiredSize = ShidenWidget->GetDesiredSize();
	return DesiredSize.X > 0 && DesiredSize.Y > 0;
}

bool UShidenAutoSaveCommand::TryExecuteAutoSave(const FAutoSaveCommandArgs& Args, const FString& ProcessName, UShidenWidget* ShidenWidget,
                                                FString& ErrorMessage)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName) ||
		ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to peek scenario %s."), *ProcessName);
		return false;
	}

	const int32 CurrentIndex = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().CurrentIndex;

	// Temporarily advance the scenario index so that the game does not resume from the AutoSave command
	ShidenSubsystem->ScenarioProgressStack[ProcessName].SetCurrentIndex(CurrentIndex + 1);

	if (Args.OverwriteThumbnail.IsEmpty())
	{
		ShidenWidget->SaveGameWithScreenCapture(Args.SlotName);
	}
	else if (Args.OverwriteThumbnail == TEXT("None"))
	{
		ShidenWidget->SaveGame(Args.SlotName, Cast<UTexture2D>(FSlateNoResource().GetResourceObject()));
	}
	else
	{
		UObject* Thumbnail = nullptr;
		bool bSuccess;
		UShidenBlueprintLibrary::GetOrLoadAsset(Args.OverwriteThumbnail, Thumbnail, bSuccess);
		if (!bSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Failed to load thumbnail asset %s."), *Args.OverwriteThumbnail);
			return false;
		}
		ShidenWidget->SaveGame(Args.SlotName, Cast<UTexture2D>(Thumbnail));
	}

	ShidenSubsystem->ScenarioProgressStack[ProcessName].SetCurrentIndex(CurrentIndex);
	return true;
}
