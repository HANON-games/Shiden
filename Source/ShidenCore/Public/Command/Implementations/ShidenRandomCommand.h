// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableDefinition.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenRandomCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenRandomCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FRandomCommandArgs
	{
		EShidenVariableKind VariableKind;
		FString DestinationVariableName;
		FString MinValue;
		FString MaxValue;
		EShidenVariableType VariableType;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FRandomCommandArgs& Args, FString& ErrorMessage);

	static bool TrySetRandomValue(const UObject* WorldContextObject, const FRandomCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage);

	FRandomCommandArgs Args;
};
