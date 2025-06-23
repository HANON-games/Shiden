// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenScenario.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenIfCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenIfCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FIfCommandArgs
	{
		EShidenVariableKind VariableKind;
		FString VariableName;
		FString Operator;
		FString RightHandValue;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FIfCommandArgs& Args, FString& ErrorMessage);

	static bool TryEvaluateCondition(const FIfCommandArgs& Args, const FString& ProcessName, bool& bResult, FString& ErrorMessage);

	static bool TryExecuteCommand(const FIfCommandArgs& Args, const FString& ProcessName, FString& ErrorMessage);

	static bool TryFindNextElseIfOrElseOrEndIf(int32 StartIndex, const UShidenScenario* Scenario, FString& CommandName, int32& ResultIndex,
	                                           FString& ErrorMessage);

	FIfCommandArgs Args;
};
