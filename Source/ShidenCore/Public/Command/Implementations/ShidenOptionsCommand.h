// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenOptionsCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenOptionsCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FOptions
	{
		TArray<FString> Options;
	};
	
	struct FOptionsCommandArgs
	{
		TArray<FOptions> LocalizedOptions;
		EShidenVariableKind DestinationVariableKind;
		FString DestinationVariableName;
		bool bHideTextLayer;
	};

	virtual void PreProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                              const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                              UObject* CallerObject, EShidenPreProcessStatus& Status, FString& ErrorMessage) override;

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FOptionsCommandArgs& Args, FString& ErrorMessage);

	static bool TrySetupOptions(const FOptionsCommandArgs& Args, UShidenWidget* ShidenWidget, bool bRegisterProperty, FString& ErrorMessage);

	static bool TryUpdateVariable(const FString& VariableName, EShidenVariableKind VariableKind, int32 Value, FString& ErrorMessage);

	FOptionsCommandArgs Args;
};
