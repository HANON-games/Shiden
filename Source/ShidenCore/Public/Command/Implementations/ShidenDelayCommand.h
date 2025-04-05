// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "ShidenDelayCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenDelayCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FDelayCommandArgs
	{
		float Duration;
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
	
	static UInputAction* LoadInputActionFromPath(const FString& Path);

	static void ParseFromCommand(const FShidenCommand& Command, FDelayCommandArgs& Args);

	FDelayCommandArgs Args;
	
	float ElapsedTime = 0.0f;
};
