// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Variable/ShidenLocalVariable.h"
#include "ShidenExpressionBlueprintLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenExpressionBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Evaluates an expression and returns an integer result.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to evaluate (e.g., "2 + 3", "5 * 10")
	 * @param Result [out] The integer result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is an integer
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Evaluate Expression (Integer)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionInteger(const FString& ProcessName, const FString& Expression, int32& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a float result.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to evaluate (e.g., "2.5 + 3.7", "sin(1.5)")
	 * @param Result [out] The float result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is numeric
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Evaluate Expression (Float)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionFloat(const FString& ProcessName, const FString& Expression, float& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a boolean result.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to evaluate (e.g., "3 > 2", "true && false")
	 * @param Result [out] The boolean result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is a boolean
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Evaluate Expression (Boolean)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionBoolean(const FString& ProcessName, const FString& Expression, bool& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a string result.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to evaluate (e.g., "\"Hello\" + \" World\"")
	 * @param Result [out] The string result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Evaluate Expression (String)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionString(const FString& ProcessName, const FString& Expression, FString& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a Vector2 result.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to evaluate (e.g., "{1,2} * 2", "{3,4} + {1,1}")
	 * @param Result [out] The Vector2D result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is a Vector2D
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Evaluate Expression (Vector2)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionVector2(const FString& ProcessName, const FString& Expression, FVector2D& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a Vector3 result.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to evaluate (e.g., "{1,2,3} * 2", "{3,4,5} + {1,1,1}")
	 * @param Result [out] The Vector3 result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is a Vector3
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Evaluate Expression (Vector3)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionVector3(const FString& ProcessName, const FString& Expression, FVector& Result, FString& ErrorMessage);

	/**
	 * Validates an expression without evaluating it.
	 *
	 * @param ProcessName The process name (use "Default" if unsure)
	 * @param Expression The expression to validate
	 * @param ErrorMessage [out] Error message if validation failed
	 * @return True if the expression is valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (ProcessName = "Default", DisplayName = "Validate Expression"))
	static UPARAM(DisplayName = "Success") bool TryValidateExpression(const FString& ProcessName, const FString& Expression, FString& ErrorMessage);

	/**
	 * Validates an expression without evaluating it and without variable substitution.
	 *
	 * @param Expression The expression to validate
	 * @param ErrorMessage [out] Error message if validation failed
	 * @return True if the expression is valid
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Validate Expression Raw"))
	static UPARAM(DisplayName = "Success") bool TryValidateExpressionRaw(const FString& Expression, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns an integer result without variable substitution.
	 *
	 * @param Expression The expression to evaluate (e.g., "2 + 3", "5 * 10")
	 * @param Result [out] The integer result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is an integer
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Evaluate Expression Raw (Integer)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionIntegerRaw(const FString& Expression, int32& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a float result without variable substitution.
	 *
	 * @param Expression The expression to evaluate (e.g., "2.5 + 3.7", "sin(1.5)")
	 * @param Result [out] The float result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is numeric
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Evaluate Expression Raw (Float)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionFloatRaw(const FString& Expression, float& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a boolean result without variable substitution.
	 *
	 * @param Expression The expression to evaluate (e.g., "3 > 2", "true && false")
	 * @param Result [out] The boolean result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is a boolean
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Evaluate Expression Raw (Boolean)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionBooleanRaw(const FString& Expression, bool& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a string result without variable substitution.
	 *
	 * @param Expression The expression to evaluate (e.g., "\"Hello\" + \" World\"")
	 * @param Result [out] The string result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Evaluate Expression Raw (String)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionStringRaw(const FString& Expression, FString& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a Vector2 result without variable substitution.
	 *
	 * @param Expression The expression to evaluate (e.g., "[1,2] * 2", "[3,4] + [1,1]")
	 * @param Result [out] The Vector2D result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is a Vector2D
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Evaluate Expression Raw (Vector2)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionVector2Raw(const FString& Expression, FVector2D& Result, FString& ErrorMessage);

	/**
	 * Evaluates an expression and returns a Vector3 result without variable substitution.
	 *
	 * @param Expression The expression to evaluate (e.g., "[1,2,3] * 2", "[3,4,5] + [1,1,1]")
	 * @param Result [out] The Vector3 result
	 * @param ErrorMessage [out] Error message if evaluation failed
	 * @return True if evaluation was successful and result is a Vector3
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Expression", meta = (DisplayName = "Evaluate Expression Raw (Vector3)"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateExpressionVector3Raw(const FString& Expression, FVector& Result, FString& ErrorMessage);

	static FString ReplaceVariablesInExpression(const FString& ProcessName, const FString& Expression);

private:
	static bool TryCreateScopeKey(const FString& ProcessName, FString& ScenarioKey);

	static FString EscapeStringForExpression(const FString& StringValue);

	template<typename TVariable>
	static FString TryGetVariableValueAsExpressionString(TVariable& Variable, const FShidenVariableDefinition& Definition);

	static FString TryGetLocalVariableValueAsExpressionString(FShidenLocalVariable& LocalVariable, const FString& ScopeKey, const FShidenVariableDefinition& Definition);
};
