// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenWaitClickCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenWaitClickCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FWaitClickCommandArgs
	{
		bool bCanSkip;
	};

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                              UShidenWidget* Widget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status,
	                                              FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* Widget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FWaitClickCommandArgs& Args);

	static UInputAction* LoadInputActionFromPath(const FString& Path);

	FWaitClickCommandArgs Args;

	bool PressNext = true;
};
