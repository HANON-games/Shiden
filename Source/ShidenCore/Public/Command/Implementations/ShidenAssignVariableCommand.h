// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableKind.h"
#include "Variable/ShidenVariableDefinition.h"
#include "ShidenAssignVariableCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenAssignVariableCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FAssignVariableCommandArgs
	{
		EShidenVariableKind VariableKind;
		FString VariableName;
		FString Value;
		EShidenVariableType VariableType;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager, const float DeltaTime,
	                                           UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FString& ProcessName, const FShidenCommand& Command, FAssignVariableCommandArgs& Args, FString& ErrorMessage);

	static bool TryAssign(const UObject* WorldContextObject, const FAssignVariableCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage);

	FAssignVariableCommandArgs Args;
};
