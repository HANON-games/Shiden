// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenWaitForClickCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenWaitForClickCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FWaitForClickCommandArgs
	{
		bool bCanSkip;
	};

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject,
	                                           EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FWaitForClickCommandArgs& Args);

	static UInputAction* LoadInputActionFromPath(const FString& Path);

	FWaitForClickCommandArgs Args;

	bool bPressNext = true;
};
