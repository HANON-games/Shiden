// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenPrintCommand.h"
#include "Kismet/KismetSystemLibrary.h"

void UShidenPrintCommand::ParseFromCommand(const FShidenCommand& Command, FPrintCommandArgs& Args)
{
	Args.Content = Command.GetArg("Content");
}

void UShidenPrintCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
                                                        UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                        const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status,
                                                        FString& BreakReason, FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	UKismetSystemLibrary::PrintString(nullptr, Args.Content);
	Status = EShidenProcessStatus::Next;
}

void UShidenPrintCommand::PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                        const TScriptInterface<IShidenManagerInterface>& ShidenManager, bool bIsCurrentCommand,
                                                        EShidenPreviewStatus& Status, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	UKismetSystemLibrary::PrintString(nullptr, Args.Content);
	Status = EShidenPreviewStatus::Complete;
}
