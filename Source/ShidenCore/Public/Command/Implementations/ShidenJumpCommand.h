// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenario.h"
#include "ShidenJumpCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenJumpCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FJumpCommandArgs
	{
		FString DestinationTagName;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FJumpCommandArgs& Args);

	static bool TryFindTagIndex(const FJumpCommandArgs& Args, const UShidenScenario* Scenario, int32& FoundIndex, FString& ErrorMessage);

	FJumpCommandArgs Args;
};
