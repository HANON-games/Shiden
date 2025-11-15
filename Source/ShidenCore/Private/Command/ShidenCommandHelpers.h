// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommand.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Variable/ShidenVariableKind.h"

class UShidenScenario;

namespace ShidenCommandHelpers
{
	bool TryConvertToEasingFunc(const FString& EasingFuncStr, EEasingFunc::Type& EasingFunc, FString& ErrorMessage);

	bool TryConvertToAudioFaderCurve(const FString& AudioFaderCurveStr, EAudioFaderCurve& AudioFaderCurve, FString& ErrorMessage);

	bool TryGetCurrentOriginalCommand(const FString& ProcessName, FShidenCommand*& OriginalCommand, FString& ErrorMessage);
}

// Conditional statement navigation helpers (If/IfExpression/ElseIf/ElseIfExpression/LoopWhile/LoopWhileExpression)
namespace ShidenConditionalCommandHelpers
{
	struct FVariableConditionArgs
	{
		EShidenVariableKind VariableKind;
		FString VariableName;
		FString Operator;
		FString RightHandValue;
	};
	
	bool TryParseVariableCondition(const FShidenCommand& Command, FVariableConditionArgs& Args, FString& ErrorMessage);

	bool TryEvaluateVariableCondition(const FString& ProcessName, const FVariableConditionArgs& Args, bool& bResult, FString& ErrorMessage);

	struct FExpressionConditionArgs
	{
		FString Expression;
	};

	bool TryParseExpressionCondition(const FShidenCommand& Command, FExpressionConditionArgs& Args, FString& ErrorMessage);

	bool TryEvaluateExpressionCondition(const FString& ProcessName, const FExpressionConditionArgs& Args, bool& bResult, FString& ErrorMessage);

	// For If/IfExpression/ElseIf/ElseIfExpression
	bool TrySkipToNextIfBranch(const FString& ProcessName, int32 StartIndex, FString& CommandName, int32& ResultIndex, FString& ErrorMessage);

	bool TryFindEndIfIndex(const FString& ProcessName, int32 StartIndex, int32& ResultIndex, FString& ErrorMessage);
	
	// For LoopWhile/LoopWhileExpression
	bool TryFindEndLoopWhileIndex(const FString& ProcessName, int32 StartIndex, int32& ResultIndex, FString& ErrorMessage);
}
