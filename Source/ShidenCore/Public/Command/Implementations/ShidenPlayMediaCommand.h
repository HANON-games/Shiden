// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenPlayMediaCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenPlayMediaCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FPlayMediaCommandArgs
	{
		FString MediaSourcePath;
		bool bCanOpenPauseMenu;
		int32 MediaZOrder;
	};

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                              UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command,
	                                           UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static void ParseFromCommand(const FShidenCommand& Command, FPlayMediaCommandArgs& Args);

	FPlayMediaCommandArgs Args;
};
