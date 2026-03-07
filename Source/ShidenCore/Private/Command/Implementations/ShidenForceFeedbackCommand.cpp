// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/Implementations/ShidenForceFeedbackCommand.h"

void UShidenForceFeedbackCommand::ParseFromCommand(const FShidenCommand& Command, FForceFeedbackCommandArgs& Args)
{
	Args.FeedbackEffectPath = Command.GetArg(TEXT("FeedbackEffect"));
}

void UShidenForceFeedbackCommand::ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
                                                                const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
                                                                UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
                                                                FString& NextScenarioName, FString& ErrorMessage)
{
	ParseFromCommand(Command, Args);
	Status = TryPlayForceFeedback(ShidenManager, Args, ErrorMessage)
		         ? EShidenProcessStatus::Next
		         : EShidenProcessStatus::Error;
}


bool UShidenForceFeedbackCommand::TryPlayForceFeedback(const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                       const FForceFeedbackCommandArgs& Args, FString& ErrorMessage)
{
	bool bSuccess;
	ShidenManager->Execute_PlayForceFeedback(ShidenManager.GetObject(), Args.FeedbackEffectPath, bSuccess);
	if (!bSuccess)
	{
		ErrorMessage = FString::Printf(TEXT("Failed to play force feedback %s."), *Args.FeedbackEffectPath);
		return false;
	}

	return true;
}
