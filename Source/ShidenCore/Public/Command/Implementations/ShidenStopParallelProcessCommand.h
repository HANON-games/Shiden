// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenStopParallelProcessCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenStopParallelProcessCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FStopParallelProcessArgs
	{
		FString ProcessName;
		FString Reason;
		FString StopTiming;
		bool bWaitForCompletion;
	};

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FStopParallelProcessArgs& Args, FString& ErrorMessage);

	FStopParallelProcessArgs Args;
};
