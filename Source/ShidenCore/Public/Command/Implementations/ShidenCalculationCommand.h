// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandObject.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenCalculationCommand.generated.h"

UCLASS()
class SHIDENCORE_API UShidenCalculationCommand : public UShidenCommandObject
{
	GENERATED_BODY()

	struct FCalculationCommandArgs
	{
		EShidenVariableKind VariableKind;
		FString VariableName;
		FString Operator;
		FString Value;
	};

	virtual void ProcessCommand_Implementation(const FString& ProcessName, const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           const float DeltaTime, UObject* CallerObject, EShidenProcessStatus& Status, FString& BreakReason,
	                                           FString& NextScenarioName, FString& ErrorMessage) override;

	virtual void PreviewCommand_Implementation(const FShidenCommand& Command, UShidenWidget* ShidenWidget,
	                                           const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                                           bool bIsCurrentCommand, EShidenPreviewStatus& Status, FString& ErrorMessage) override;

	static bool TryParseCommand(const FShidenCommand& Command, FCalculationCommandArgs& Args, FString& ErrorMessage);

	static bool TryCalculateAndUpdateVariable(const FCalculationCommandArgs& Args, const UObject* WorldContextObject, const FString& ProcessName,
	                                          FString& ErrorMessage);

	static bool CalculateFloat(const FString& Operator, const float& A, const float& B, float& Result, FString& ErrorMessage);

	static bool CalculateInteger(const FString& Operator, const int32& A, const int32& B, int32& Result, FString& ErrorMessage);

	static bool CalculateVector2(const FString& Operator, const FVector2D& A, const FVector2D& B, FVector2D& Result, FString& ErrorMessage);

	static bool CalculateVector3(const FString& Operator, const FVector& A, const FVector& B, FVector& Result, FString& ErrorMessage);

	FCalculationCommandArgs Args;
};
