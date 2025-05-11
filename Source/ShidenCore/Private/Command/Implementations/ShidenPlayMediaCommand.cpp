// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenPlayMediaCommand.h"

void UShidenPlayMediaCommand::ParseFromCommand(const FShidenCommand& Command, FPlayMediaCommandArgs& Args)
{
	Args.MediaSourcePath = Command.GetArg("MediaSource");
	Args.bCanOpenPauseMenu = Command.GetArgAsBool("CanOpenPauseMenu");
	Args.MediaZOrder = Command.GetArgAsInt("MediaZOrder");
}

void UShidenPlayMediaCommand::PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                               UShidenWidget* ShidenWidget,
                                                               const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                               UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);

	bool bSuccess;
	ShidenWidget->PlayMedia(Args.MediaSourcePath, Args.bCanOpenPauseMenu, Args.MediaZOrder, bSuccess);
	if (!bSuccess)
	{
		Status = EShidenPreProcessStatus::Error;
		ErrorMessage = FString::Printf(TEXT("Failed to play media %s."), *Args.MediaSourcePath);
		return;
	}

	Status = EShidenPreProcessStatus::Complete;
}

void UShidenPlayMediaCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                            UShidenWidget* ShidenWidget,
                                                            const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                            const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                            FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	if (!ShidenWidget->IsMediaCompleted())
	{
		Status = EShidenProcessStatus::DelayUntilNextTick;
		return;
	}

	ShidenWidget->CollapseMedia();
	Status = EShidenProcessStatus::Next;
}
