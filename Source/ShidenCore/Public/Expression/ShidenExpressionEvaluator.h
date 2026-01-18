// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR
#include "Variable/ShidenVariableDefinition.h"
#endif

enum class EShidenExpressionValueType : uint8
{
	None,
	Integer,
	Float,
	String,
	Boolean,
	Vector2,
	Vector3
};

// Variant type that can hold different value types for expression evaluation
struct SHIDENCORE_API FShidenExpressionValue
{
	EShidenExpressionValueType Type = EShidenExpressionValueType::None;

	int32 IntValue = 0;
	float FloatValue = 0.0;
	FString StringValue;
	bool BoolValue = false;
	FVector2D Vector2Value = FVector2D::ZeroVector;
	FVector Vector3Value = FVector::ZeroVector;

	FShidenExpressionValue() = default;

	explicit FShidenExpressionValue(const int32 Value) noexcept : Type(EShidenExpressionValueType::Integer), IntValue(Value)
	{
	}

	explicit FShidenExpressionValue(const float Value) noexcept : Type(EShidenExpressionValueType::Float), FloatValue(Value)
	{
	}

	explicit FShidenExpressionValue(const double Value) noexcept : Type(EShidenExpressionValueType::Float), FloatValue(static_cast<float>(Value))
	{
	}

	explicit FShidenExpressionValue(const FString& Value) : Type(EShidenExpressionValueType::String), StringValue(Value)
	{
	}

	explicit FShidenExpressionValue(const bool Value) noexcept : Type(EShidenExpressionValueType::Boolean), BoolValue(Value)
	{
	}

	explicit FShidenExpressionValue(const FVector2D& Value) noexcept : Type(EShidenExpressionValueType::Vector2), Vector2Value(Value)
	{
	}

	explicit FShidenExpressionValue(const FVector& Value) noexcept : Type(EShidenExpressionValueType::Vector3), Vector3Value(Value)
	{
	}

	bool TryToNumeric(float& OutValue, FString& ErrorMessage) const;

	bool TryToInteger(int32& OutValue, FString& ErrorMessage) const;

	bool TryToBoolean(bool& OutValue, FString& ErrorMessage) const;

	FString ToString() const;
};

enum class EShidenExpressionTokenType : uint8
{
	None,
	Number, // 123, 45.67
	Integer, // Explicitly integer
	Float, // Explicitly float
	String, // "text"
	Boolean, // true, false
	Vector, // [1,2] or [1,2,3]
	Function, // sin, cos, sqrt, etc.
	Operator, // +, -, *, **, /, %, ^, &, |, ~, <<, >>
	Comparison, // ==, !=, >, <, >=, <=
	Logical, // &&, ||, !
	LeftParen, // (
	RightParen, // )
	LeftBracket, // [
	RightBracket, // ]
	Comma, // ,
	End // End of expression
};

struct SHIDENCORE_API FShidenExpressionToken
{
	EShidenExpressionTokenType Type = EShidenExpressionTokenType::None;
	FString Value;
	int32 Position = 0;

	FShidenExpressionToken() = default;

	FShidenExpressionToken(const EShidenExpressionTokenType InType, const FString& InValue, const int32 InPosition) : Type(InType), Value(InValue),
	                                                                                                                  Position(InPosition)
	{
	}
};

#if WITH_EDITOR
/**
 * Context for editor-only expression evaluation with variable definitions
 */
struct SHIDENCORE_API FShidenExpressionVariableDefinitionContext
{
	TMap<FString, FShidenVariableDefinition> UserVariables;
	TMap<FString, FShidenVariableDefinition> SystemVariables;
	TMap<FString, FShidenVariableDefinition> LocalVariables;
	TMap<FString, FShidenVariableDefinition> MacroParameters;
	bool bIsMacro = false;
	
	const FShidenVariableDefinition* FindVariable(const FString& Kind, const FString& Name) const;
};
#endif

/**
 * Expression Evaluator supporting arithmetic, bitwise, logical, string, and vector operations
 */
class SHIDENCORE_API FShidenExpressionEvaluator
{
public:
	// Default constructor for runtime evaluation without variable definitions
	FShidenExpressionEvaluator() = default;

#if WITH_EDITOR
	/**
	 * Constructor for editor-only expression evaluation with variable definitions.
	 *
	 * @param InContext Variable definitions context
	 */
	explicit FShidenExpressionEvaluator(const FShidenExpressionVariableDefinitionContext& InContext);
#endif

	/**
	 * Evaluates an expression and returns the result.
	 * In Editor with context, enables editor-specific functions like IsReadOnlyVariable("System::test").
	 *
	 * @param Expression The expression to evaluate
	 * @param OutResult The result of the evaluation
	 * @param ErrorMessage Error message if evaluation fails
	 * @return True if evaluation succeeded
	 */
	bool TryEvaluate(const FString& Expression, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	/**
	 * Validates an expression without evaluating it.
	 *
	 * @param Expression The expression to validate
	 * @param ErrorMessage Error message if validation fails
	 * @return True if validation succeeded
	 */
	bool TryValidate(const FString& Expression, FString& ErrorMessage) const;

private:
#if WITH_EDITOR
	// Editor-only member variable for context
	const FShidenExpressionVariableDefinitionContext* Context = nullptr;
#endif

	// Static helper methods (stateless)
	static bool TryTokenize(const FString& Expression, TArray<FShidenExpressionToken>& OutTokens, FString& ErrorMessage);

	static constexpr bool IsWhitespace(const TCHAR C) noexcept { return C == ' ' || C == '\t' || C == '\n' || C == '\r'; }

	static constexpr bool IsDigit(const TCHAR C) noexcept { return C >= '0' && C <= '9'; }

	static constexpr bool IsAlpha(const TCHAR C) noexcept { return (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z'); }

	static constexpr bool IsAlphaNumeric(const TCHAR C) noexcept { return IsAlpha(C) || IsDigit(C); }

	static bool TryParseNumber(const FString& NumberStr, FShidenExpressionValue& OutValue);

	static bool TryApplyBinaryOperation(const FShidenExpressionValue& Left, const FString& Operator, const FShidenExpressionValue& Right,
	                                    FShidenExpressionValue& OutResult, FString& ErrorMessage);
	
	static bool TryApplyUnaryOperation(const FString& Operator, const FShidenExpressionValue& Value, FShidenExpressionValue& OutResult,
	                                   FString& ErrorMessage);

	// Instance methods (recursive descent parser following operator precedence)
	bool TryParseExpression(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;
	
	bool TryParseLogicalOr(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseLogicalAnd(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseBitwiseOr(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseBitwiseXor(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseBitwiseAnd(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseComparison(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseShift(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseAddition(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseMultiplication(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParsePower(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseUnary(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParsePrimary(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryParseVector(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage) const;

	bool TryEvaluateFunction(const FString& FunctionName, const TArray<FShidenExpressionValue>& Args, FShidenExpressionValue& OutResult,
	                         FString& ErrorMessage) const;

#if WITH_EDITOR
	// Editor-only functions for variable definition queries
	bool TryEvaluateEditorFunction(const FString& FunctionName, const TArray<FShidenExpressionValue>& Args,
	                               FShidenExpressionValue& OutResult, FString& ErrorMessage) const;
#endif
};
