// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

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

/**
 * Expression Evaluator supporting arithmetic, bitwise, logical, string, and vector operations
 */
class SHIDENCORE_API FShidenExpressionEvaluator
{
public:
	static bool TryEvaluate(const FString& Expression, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryValidate(const FString& Expression, FString& ErrorMessage);

private:
	static bool TryTokenize(const FString& Expression, TArray<FShidenExpressionToken>& OutTokens, FString& ErrorMessage);

	static constexpr bool IsWhitespace(const TCHAR C) noexcept { return C == ' ' || C == '\t' || C == '\n' || C == '\r'; }

	static constexpr bool IsDigit(const TCHAR C) noexcept { return C >= '0' && C <= '9'; }

	static constexpr bool IsAlpha(const TCHAR C) noexcept { return (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z'); }

	static constexpr bool IsAlphaNumeric(const TCHAR C) noexcept { return IsAlpha(C) || IsDigit(C); }

	// Recursive descent parser following operator precedence
	static bool TryParseExpression(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseLogicalOr(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseLogicalAnd(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseBitwiseOr(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseBitwiseXor(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseBitwiseAnd(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseComparison(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseShift(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseAddition(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseMultiplication(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
	                                   FString& ErrorMessage);

	static bool TryParsePower(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseUnary(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParsePrimary(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryParseVector(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryApplyBinaryOperation(const FShidenExpressionValue& Left, const FString& Operator, const FShidenExpressionValue& Right,
	                                    FShidenExpressionValue& OutResult, FString& ErrorMessage);

	static bool TryApplyUnaryOperation(const FString& Operator, const FShidenExpressionValue& Value, FShidenExpressionValue& OutResult,
	                                   FString& ErrorMessage);

	static bool TryParseNumber(const FString& NumberStr, FShidenExpressionValue& OutValue);

	static bool TryEvaluateFunction(const FString& FunctionName, const TArray<FShidenExpressionValue>& Args, FShidenExpressionValue& OutResult,
	                                FString& ErrorMessage);
};
