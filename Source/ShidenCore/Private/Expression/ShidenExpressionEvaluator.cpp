// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Expression/ShidenExpressionEvaluator.h"
#include "Misc/DefaultValueHelper.h"

bool FShidenExpressionValue::TryToNumeric(float& OutValue, FString& ErrorMessage) const
{
	switch (Type)
	{
	case EShidenExpressionValueType::Integer:
		OutValue = static_cast<float>(IntValue);
		return true;

	case EShidenExpressionValueType::Float:
		OutValue = FloatValue;
		return true;

	case EShidenExpressionValueType::Boolean:
		OutValue = BoolValue ? 1.0f : 0.0f;
		return true;

	default:
		ErrorMessage = TEXT("Cannot convert to numeric value");
		return false;
	}
}

bool FShidenExpressionValue::TryToInteger(int32& OutValue, FString& ErrorMessage) const
{
	switch (Type)
	{
	case EShidenExpressionValueType::Integer:
		OutValue = IntValue;
		return true;

	case EShidenExpressionValueType::Float:
		OutValue = static_cast<int32>(FloatValue);
		return true;

	case EShidenExpressionValueType::Boolean:
		OutValue = BoolValue ? 1 : 0;
		return true;

	default:
		ErrorMessage = TEXT("Cannot convert to integer value");
		return false;
	}
}

bool FShidenExpressionValue::TryToBoolean(bool& OutValue, FString& ErrorMessage) const
{
	switch (Type)
	{
	case EShidenExpressionValueType::Boolean:
		OutValue = BoolValue;
		return true;

	case EShidenExpressionValueType::Integer:
		OutValue = IntValue != 0;
		return true;

	case EShidenExpressionValueType::Float:
		OutValue = !FMath::IsNearlyZero(FloatValue);
		return true;

	default:
		ErrorMessage = TEXT("Cannot convert to boolean value");
		return false;
	}
}

FString FShidenExpressionValue::ToString() const
{
	switch (Type)
	{
	case EShidenExpressionValueType::Integer:
		return FString::Printf(TEXT("%d"), IntValue);

	case EShidenExpressionValueType::Float:
		return FString::Printf(TEXT("%f"), FloatValue);

	case EShidenExpressionValueType::String:
		return StringValue;

	case EShidenExpressionValueType::Boolean:
		return BoolValue ? TEXT("true") : TEXT("false");

	case EShidenExpressionValueType::Vector2:
		return FString::Printf(TEXT("X=%f Y=%f"), Vector2Value.X, Vector2Value.Y);

	case EShidenExpressionValueType::Vector3:
		return FString::Printf(TEXT("X=%f Y=%f Z=%f"), Vector3Value.X, Vector3Value.Y, Vector3Value.Z);

	default:
		return TEXT("");
	}
}

bool FShidenExpressionEvaluator::TryEvaluate(const FString& Expression, FShidenExpressionValue& OutResult, FString& ErrorMessage)
{
	TArray<FShidenExpressionToken> Tokens;
	if (!TryTokenize(Expression, Tokens, ErrorMessage))
	{
		return false;
	}

	if (Tokens.Num() == 0)
	{
		ErrorMessage = TEXT("Empty expression");
		return false;
	}

	int32 Index = 0;
	if (!TryParseExpression(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	if (Index < Tokens.Num() && Tokens[Index].Type != EShidenExpressionTokenType::End)
	{
		ErrorMessage = FString::Printf(TEXT("Unexpected token at position %d: %s"), Tokens[Index].Position, *Tokens[Index].Value);
		return false;
	}

	return true;
}

bool FShidenExpressionEvaluator::TryValidate(const FString& Expression, FString& ErrorMessage)
{
	FShidenExpressionValue Result;
	return TryEvaluate(Expression, Result, ErrorMessage);
}

bool FShidenExpressionEvaluator::TryTokenize(const FString& Expression, TArray<FShidenExpressionToken>& OutTokens, FString& ErrorMessage)
{
	OutTokens.Empty();
	const int32 Len = Expression.Len();
	int32 Pos = 0;

	while (Pos < Len)
	{
		const TCHAR C = Expression[Pos];

		if (IsWhitespace(C))
		{
			Pos++;
			continue;
		}

		if (IsDigit(C) || (C == '.' && Pos + 1 < Len && IsDigit(Expression[Pos + 1])))
		{
			const int32 Start = Pos;
			bool bHasDot = false;
			while (Pos < Len && (IsDigit(Expression[Pos]) || Expression[Pos] == '.'))
			{
				if (Expression[Pos] == '.')
				{
					if (bHasDot)
					{
						ErrorMessage = FString::Printf(TEXT("Invalid number format at position %d"), Start);
						return false;
					}
					bHasDot = true;
				}
				Pos++;
			}
			const FString NumberStr = Expression.Mid(Start, Pos - Start);
			OutTokens.Add(FShidenExpressionToken(bHasDot ? EShidenExpressionTokenType::Float : EShidenExpressionTokenType::Integer, NumberStr,
			                                     Start));
			continue;
		}

		if (C == '"')
		{
			const int32 Start = Pos;
			Pos++;
			FString StringValue;
			while (Pos < Len && Expression[Pos] != '"')
			{
				if (Expression[Pos] == '\\' && Pos + 1 < Len)
				{
					Pos++;
					switch (Expression[Pos])
					{
					case 'n': StringValue += '\n';
						break;
					case 't': StringValue += '\t';
						break;
					case 'r': StringValue += '\r';
						break;
					case '\\': StringValue += '\\';
						break;
					case '"': StringValue += '"';
						break;
					default: StringValue += Expression[Pos];
						break;
					}
				}
				else
				{
					StringValue += Expression[Pos];
				}
				Pos++;
			}
			if (Pos >= Len)
			{
				ErrorMessage = FString::Printf(TEXT("Unterminated string at position %d"), Start);
				return false;
			}
			Pos++;
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::String, StringValue, Start));
			continue;
		}

		if (IsAlpha(C) || C == '_')
		{
			const int32 Start = Pos;
			while (Pos < Len && (IsAlphaNumeric(Expression[Pos]) || Expression[Pos] == '_'))
			{
				Pos++;
			}

			const FString Identifier = Expression.Mid(Start, Pos - Start);

			if (Identifier.Equals(TEXT("true"), ESearchCase::IgnoreCase))
			{
				OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Boolean, TEXT("true"), Start));
				continue;
			}

			if (Identifier.Equals(TEXT("false"), ESearchCase::IgnoreCase))
			{
				OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Boolean, TEXT("false"), Start));
				continue;
			}

			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Function, Identifier, Start));
			continue;
		}

		if (C == '[')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::LeftBracket, TEXT("["), Pos));
			Pos++;
			continue;
		}
		if (C == ']')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::RightBracket, TEXT("]"), Pos));
			Pos++;
			continue;
		}

		if (C == '(')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::LeftParen, TEXT("("), Pos));
			Pos++;
			continue;
		}
		if (C == ')')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::RightParen, TEXT(")"), Pos));
			Pos++;
			continue;
		}

		if (C == ',')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Comma, TEXT(","), Pos));
			Pos++;
			continue;
		}

		if (Pos + 1 < Len)
		{
			const FString TwoChar = Expression.Mid(Pos, 2);
			if (TwoChar == TEXT("==") || TwoChar == TEXT("!=") || TwoChar == TEXT(">=") || TwoChar == TEXT("<="))
			{
				OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Comparison, TwoChar, Pos));
				Pos += 2;
				continue;
			}
			if (TwoChar == TEXT("&&") || TwoChar == TEXT("||"))
			{
				OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Logical, TwoChar, Pos));
				Pos += 2;
				continue;
			}
			if (TwoChar == TEXT("<<") || TwoChar == TEXT(">>") || TwoChar == TEXT("**"))
			{
				OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Operator, TwoChar, Pos));
				Pos += 2;
				continue;
			}
		}

		if (C == '+' || C == '-' || C == '*' || C == '/' || C == '%' || C == '^' || C == '&' || C == '|' || C == '~')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Operator, FString::Chr(C), Pos));
			Pos++;
			continue;
		}

		if (C == '>' || C == '<')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Comparison, FString::Chr(C), Pos));
			Pos++;
			continue;
		}

		if (C == '!')
		{
			OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::Logical, TEXT("!"), Pos));
			Pos++;
			continue;
		}

		ErrorMessage = FString::Printf(TEXT("Unexpected character '%c' at position %d"), C, Pos);
		return false;
	}

	OutTokens.Add(FShidenExpressionToken(EShidenExpressionTokenType::End, TEXT(""), Len));
	return true;
}

bool FShidenExpressionEvaluator::TryParseNumber(const FString& NumberStr, FShidenExpressionValue& OutValue)
{
	if (NumberStr.Contains(TEXT(".")))
	{
		float Value;
		if (FDefaultValueHelper::ParseFloat(NumberStr, Value))
		{
			OutValue = FShidenExpressionValue(Value);
			return true;
		}
	}
	else
	{
		int32 Value;
		if (FDefaultValueHelper::ParseInt(NumberStr, Value))
		{
			OutValue = FShidenExpressionValue(Value);
			return true;
		}
	}
	return false;
}

bool FShidenExpressionEvaluator::TryParseExpression(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                    FString& ErrorMessage)
{
	return TryParseLogicalOr(Tokens, Index, OutResult, ErrorMessage);
}

bool FShidenExpressionEvaluator::TryParseLogicalOr(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                   FString& ErrorMessage)
{
	if (!TryParseLogicalAnd(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Logical && Tokens[Index].Value == TEXT("||"))
	{
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseLogicalAnd(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		bool LeftBool, RightBool;
		if (!OutResult.TryToBoolean(LeftBool, ErrorMessage) || !Right.TryToBoolean(RightBool, ErrorMessage))
		{
			return false;
		}

		OutResult = FShidenExpressionValue(LeftBool || RightBool);
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseLogicalAnd(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                    FString& ErrorMessage)
{
	if (!TryParseBitwiseOr(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Logical && Tokens[Index].Value == TEXT("&&"))
	{
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseBitwiseOr(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		bool LeftBool, RightBool;
		if (!OutResult.TryToBoolean(LeftBool, ErrorMessage) || !Right.TryToBoolean(RightBool, ErrorMessage))
		{
			return false;
		}

		OutResult = FShidenExpressionValue(LeftBool && RightBool);
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseBitwiseOr(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                   FString& ErrorMessage)
{
	if (!TryParseBitwiseXor(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("|"))
	{
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseBitwiseXor(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, TEXT("|"), Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseBitwiseXor(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                    FString& ErrorMessage)
{
	if (!TryParseBitwiseAnd(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("^"))
	{
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseBitwiseAnd(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, TEXT("^"), Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseBitwiseAnd(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                    FString& ErrorMessage)
{
	if (!TryParseComparison(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("&"))
	{
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseComparison(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, TEXT("&"), Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseComparison(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                    FString& ErrorMessage)
{
	if (!TryParseShift(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	if (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Comparison)
	{
		const FString Operator = Tokens[Index].Value;
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseShift(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, Operator, Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseShift(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                               FString& ErrorMessage)
{
	if (!TryParseAddition(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && (Tokens[Index].Value == TEXT("<<") || Tokens[Index].
		Value == TEXT(">>")))
	{
		const FString Operator = Tokens[Index].Value;
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseAddition(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, Operator, Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseAddition(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                  FString& ErrorMessage)
{
	if (!TryParseMultiplication(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && (Tokens[Index].Value == TEXT("+") || Tokens[Index].
		Value == TEXT("-")))
	{
		const FString Operator = Tokens[Index].Value;
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseMultiplication(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, Operator, Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseMultiplication(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                        FString& ErrorMessage)
{
	if (!TryParsePower(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && (Tokens[Index].Value == TEXT("*") || Tokens[Index].
		Value == TEXT("/") || Tokens[Index].Value == TEXT("%")))
	{
		const FString Operator = Tokens[Index].Value;
		Index++;
		FShidenExpressionValue Right;
		if (!TryParsePower(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, Operator, Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParsePower(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                               FString& ErrorMessage)
{
	if (!TryParseUnary(Tokens, Index, OutResult, ErrorMessage))
	{
		return false;
	}

	while (Index < Tokens.Num() && Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("**"))
	{
		Index++;
		FShidenExpressionValue Right;
		if (!TryParseUnary(Tokens, Index, Right, ErrorMessage))
		{
			return false;
		}

		if (!TryApplyBinaryOperation(OutResult, TEXT("**"), Right, OutResult, ErrorMessage))
		{
			return false;
		}
	}

	return true;
}

bool FShidenExpressionEvaluator::TryParseUnary(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                               FString& ErrorMessage)
{
	if (Index >= Tokens.Num())
	{
		ErrorMessage = TEXT("Unexpected end of expression");
		return false;
	}

	if (Tokens[Index].Type == EShidenExpressionTokenType::Logical && Tokens[Index].Value == TEXT("!"))
	{
		Index++;
		if (!TryParseUnary(Tokens, Index, OutResult, ErrorMessage))
		{
			return false;
		}
		return TryApplyUnaryOperation(TEXT("!"), OutResult, OutResult, ErrorMessage);
	}

	if (Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("-"))
	{
		Index++;
		if (!TryParseUnary(Tokens, Index, OutResult, ErrorMessage))
		{
			return false;
		}
		return TryApplyUnaryOperation(TEXT("-"), OutResult, OutResult, ErrorMessage);
	}

	if (Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("+"))
	{
		Index++;
		if (!TryParseUnary(Tokens, Index, OutResult, ErrorMessage))
		{
			return false;
		}
		return TryApplyUnaryOperation(TEXT("+"), OutResult, OutResult, ErrorMessage);
	}

	if (Tokens[Index].Type == EShidenExpressionTokenType::Operator && Tokens[Index].Value == TEXT("~"))
	{
		Index++;
		if (!TryParseUnary(Tokens, Index, OutResult, ErrorMessage))
		{
			return false;
		}
		return TryApplyUnaryOperation(TEXT("~"), OutResult, OutResult, ErrorMessage);
	}

	return TryParsePrimary(Tokens, Index, OutResult, ErrorMessage);
}

bool FShidenExpressionEvaluator::TryParsePrimary(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                 FString& ErrorMessage)
{
	if (Index >= Tokens.Num())
	{
		ErrorMessage = TEXT("Unexpected end of expression");
		return false;
	}

	const FShidenExpressionToken& Token = Tokens[Index];

	if (Token.Type == EShidenExpressionTokenType::Integer || Token.Type == EShidenExpressionTokenType::Float)
	{
		if (!TryParseNumber(Token.Value, OutResult))
		{
			ErrorMessage = FString::Printf(TEXT("Invalid number: %s"), *Token.Value);
			return false;
		}
		Index++;
		return true;
	}

	if (Token.Type == EShidenExpressionTokenType::String)
	{
		OutResult = FShidenExpressionValue(Token.Value);
		Index++;
		return true;
	}

	if (Token.Type == EShidenExpressionTokenType::Boolean)
	{
		OutResult = FShidenExpressionValue(Token.Value.Equals(TEXT("true"), ESearchCase::IgnoreCase));
		Index++;
		return true;
	}

	if (Token.Type == EShidenExpressionTokenType::LeftBracket)
	{
		return TryParseVector(Tokens, Index, OutResult, ErrorMessage);
	}

	if (Token.Type == EShidenExpressionTokenType::LeftParen)
	{
		Index++;
		if (!TryParseExpression(Tokens, Index, OutResult, ErrorMessage))
		{
			return false;
		}
		if (Index >= Tokens.Num() || Tokens[Index].Type != EShidenExpressionTokenType::RightParen)
		{
			ErrorMessage = TEXT("Missing closing parenthesis");
			return false;
		}
		Index++;
		return true;
	}

	if (Token.Type == EShidenExpressionTokenType::Function)
	{
		const FString FunctionName = Token.Value;
		Index++;

		if (Index >= Tokens.Num() || Tokens[Index].Type != EShidenExpressionTokenType::LeftParen)
		{
			ErrorMessage = FString::Printf(TEXT("Expected '(' after function name '%s'"), *FunctionName);
			return false;
		}
		Index++;

		TArray<FShidenExpressionValue> Args;
		if (Tokens[Index].Type != EShidenExpressionTokenType::RightParen)
		{
			while (true)
			{
				FShidenExpressionValue Arg;
				if (!TryParseExpression(Tokens, Index, Arg, ErrorMessage))
				{
					return false;
				}
				Args.Add(Arg);

				if (Index >= Tokens.Num())
				{
					ErrorMessage = TEXT("Unexpected end of expression in function arguments");
					return false;
				}

				if (Tokens[Index].Type == EShidenExpressionTokenType::RightParen)
				{
					break;
				}

				if (Tokens[Index].Type != EShidenExpressionTokenType::Comma)
				{
					ErrorMessage = TEXT("Expected ',' or ')' in function arguments");
					return false;
				}
				Index++;
			}
		}

		Index++;
		return TryEvaluateFunction(FunctionName, Args, OutResult, ErrorMessage);
	}

	ErrorMessage = FString::Printf(TEXT("Unexpected token: %s"), *Token.Value);
	return false;
}

bool FShidenExpressionEvaluator::TryParseVector(const TArray<FShidenExpressionToken>& Tokens, int32& Index, FShidenExpressionValue& OutResult,
                                                FString& ErrorMessage)
{
	if (Index >= Tokens.Num() || Tokens[Index].Type != EShidenExpressionTokenType::LeftBracket)
	{
		ErrorMessage = TEXT("Expected '['");
		return false;
	}
	Index++;

	TArray<double> Components;

	while (true)
	{
		FShidenExpressionValue ComponentValue;
		if (!TryParseExpression(Tokens, Index, ComponentValue, ErrorMessage))
		{
			return false;
		}

		float ComponentNumeric;
		if (!ComponentValue.TryToNumeric(ComponentNumeric, ErrorMessage))
		{
			ErrorMessage = TEXT("Vector components must be numeric");
			return false;
		}
		Components.Add(ComponentNumeric);

		if (Index >= Tokens.Num())
		{
			ErrorMessage = TEXT("Unexpected end of expression in vector");
			return false;
		}

		if (Tokens[Index].Type == EShidenExpressionTokenType::RightBracket)
		{
			break;
		}

		if (Tokens[Index].Type != EShidenExpressionTokenType::Comma)
		{
			ErrorMessage = TEXT("Expected ',' or ']' in vector");
			return false;
		}
		Index++;
	}

	Index++;

	if (Components.Num() == 2)
	{
		OutResult = FShidenExpressionValue(FVector2D(Components[0], Components[1]));
		return true;
	}
	if (Components.Num() == 3)
	{
		OutResult = FShidenExpressionValue(FVector(Components[0], Components[1], Components[2]));
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Invalid vector dimension: %d (expected 2 or 3)"), Components.Num());
	return false;
}

bool FShidenExpressionEvaluator::TryApplyBinaryOperation(const FShidenExpressionValue& Left, const FString& Operator,
                                                         const FShidenExpressionValue& Right, FShidenExpressionValue& OutResult, FString& ErrorMessage)
{
	if (Operator == TEXT("+") && (Left.Type == EShidenExpressionValueType::String || Right.Type == EShidenExpressionValueType::String))
	{
		OutResult = FShidenExpressionValue(Left.ToString() + Right.ToString());
		return true;
	}

	if (Left.Type == EShidenExpressionValueType::Vector2 && Right.Type == EShidenExpressionValueType::Vector2)
	{
		if (Operator == TEXT("+"))
		{
			OutResult = FShidenExpressionValue(Left.Vector2Value + Right.Vector2Value);
			return true;
		}
		if (Operator == TEXT("-"))
		{
			OutResult = FShidenExpressionValue(Left.Vector2Value - Right.Vector2Value);
			return true;
		}
		if (Operator == TEXT("*"))
		{
			OutResult = FShidenExpressionValue(Left.Vector2Value * Right.Vector2Value);
			return true;
		}
		if (Operator == TEXT("/"))
		{
			if (FMath::IsNearlyZero(Right.Vector2Value.X) || FMath::IsNearlyZero(Right.Vector2Value.Y))
			{
				ErrorMessage = TEXT("Division by zero in vector operation");
				return false;
			}
			OutResult = FShidenExpressionValue(Left.Vector2Value / Right.Vector2Value);
			return true;
		}
	}

	if (Left.Type == EShidenExpressionValueType::Vector2)
	{
		float RightScalar;
		if (Right.TryToNumeric(RightScalar, ErrorMessage))
		{
			if (Operator == TEXT("*"))
			{
				OutResult = FShidenExpressionValue(Left.Vector2Value * RightScalar);
				return true;
			}

			if (Operator == TEXT("/"))
			{
				if (FMath::IsNearlyZero(RightScalar))
				{
					ErrorMessage = TEXT("Division by zero");
					return false;
				}
				OutResult = FShidenExpressionValue(Left.Vector2Value / RightScalar);
				return true;
			}
		}
	}

	if (Right.Type == EShidenExpressionValueType::Vector2)
	{
		float LeftScalar;
		if (Left.TryToNumeric(LeftScalar, ErrorMessage))
		{
			if (Operator == TEXT("*"))
			{
				OutResult = FShidenExpressionValue(Right.Vector2Value * LeftScalar);
				return true;
			}
		}
	}

	if (Left.Type == EShidenExpressionValueType::Vector3 && Right.Type == EShidenExpressionValueType::Vector3)
	{
		if (Operator == TEXT("+"))
		{
			OutResult = FShidenExpressionValue(Left.Vector3Value + Right.Vector3Value);
			return true;
		}

		if (Operator == TEXT("-"))
		{
			OutResult = FShidenExpressionValue(Left.Vector3Value - Right.Vector3Value);
			return true;
		}

		if (Operator == TEXT("*"))
		{
			OutResult = FShidenExpressionValue(Left.Vector3Value * Right.Vector3Value);
			return true;
		}

		if (Operator == TEXT("/"))
		{
			if (FMath::IsNearlyZero(Right.Vector3Value.X) || FMath::IsNearlyZero(Right.Vector3Value.Y) || FMath::IsNearlyZero(Right.Vector3Value.Z))
			{
				ErrorMessage = TEXT("Division by zero in vector operation");
				return false;
			}
			OutResult = FShidenExpressionValue(Left.Vector3Value / Right.Vector3Value);
			return true;
		}
	}

	if (Left.Type == EShidenExpressionValueType::Vector3)
	{
		float RightScalar;
		if (Right.TryToNumeric(RightScalar, ErrorMessage))
		{
			if (Operator == TEXT("*"))
			{
				OutResult = FShidenExpressionValue(Left.Vector3Value * RightScalar);
				return true;
			}

			if (Operator == TEXT("/"))
			{
				if (FMath::IsNearlyZero(RightScalar))
				{
					ErrorMessage = TEXT("Division by zero");
					return false;
				}
				OutResult = FShidenExpressionValue(Left.Vector3Value / RightScalar);
				return true;
			}
		}
	}

	if (Right.Type == EShidenExpressionValueType::Vector3)
	{
		float LeftScalar;
		if (Left.TryToNumeric(LeftScalar, ErrorMessage))
		{
			if (Operator == TEXT("*"))
			{
				OutResult = FShidenExpressionValue(Right.Vector3Value * LeftScalar);
				return true;
			}
		}
	}

	if (Operator == TEXT("==") || Operator == TEXT("!=") || Operator == TEXT(">") || Operator == TEXT("<") || Operator == TEXT(">=") || Operator ==
		TEXT("<="))
	{
		// String comparison (case-sensitive, ASCII order)
		if (Left.Type == EShidenExpressionValueType::String && Right.Type == EShidenExpressionValueType::String)
		{
			const int32 CompareResult = Left.StringValue.Compare(Right.StringValue, ESearchCase::CaseSensitive);

			if (Operator == TEXT("=="))
			{
				OutResult = FShidenExpressionValue(CompareResult == 0);
				return true;
			}
			if (Operator == TEXT("!="))
			{
				OutResult = FShidenExpressionValue(CompareResult != 0);
				return true;
			}
			if (Operator == TEXT(">"))
			{
				OutResult = FShidenExpressionValue(CompareResult > 0);
				return true;
			}
			if (Operator == TEXT("<"))
			{
				OutResult = FShidenExpressionValue(CompareResult < 0);
				return true;
			}
			if (Operator == TEXT(">="))
			{
				OutResult = FShidenExpressionValue(CompareResult >= 0);
				return true;
			}
			if (Operator == TEXT("<="))
			{
				OutResult = FShidenExpressionValue(CompareResult <= 0);
				return true;
			}
		}

		// Boolean comparison
		if (Left.Type == EShidenExpressionValueType::Boolean && Right.Type == EShidenExpressionValueType::Boolean)
		{
			if (Operator == TEXT("=="))
			{
				OutResult = FShidenExpressionValue(Left.BoolValue == Right.BoolValue);
				return true;
			}
			if (Operator == TEXT("!="))
			{
				OutResult = FShidenExpressionValue(Left.BoolValue != Right.BoolValue);
				return true;
			}
			// >, <, >=, <= not supported for boolean
			ErrorMessage = FString::Printf(TEXT("Operator '%s' not supported for boolean values"), *Operator);
			return false;
		}

		// Vector2 comparison
		if (Left.Type == EShidenExpressionValueType::Vector2 && Right.Type == EShidenExpressionValueType::Vector2)
		{
			if (Operator == TEXT("=="))
			{
				OutResult = FShidenExpressionValue(Left.Vector2Value.Equals(Right.Vector2Value));
				return true;
			}
			if (Operator == TEXT("!="))
			{
				OutResult = FShidenExpressionValue(!Left.Vector2Value.Equals(Right.Vector2Value));
				return true;
			}
			// >, <, >=, <= not supported for vectors
			ErrorMessage = FString::Printf(TEXT("Operator '%s' not supported for Vector2 values"), *Operator);
			return false;
		}

		// Vector3 comparison
		if (Left.Type == EShidenExpressionValueType::Vector3 && Right.Type == EShidenExpressionValueType::Vector3)
		{
			if (Operator == TEXT("=="))
			{
				OutResult = FShidenExpressionValue(Left.Vector3Value.Equals(Right.Vector3Value));
				return true;
			}
			if (Operator == TEXT("!="))
			{
				OutResult = FShidenExpressionValue(!Left.Vector3Value.Equals(Right.Vector3Value));
				return true;
			}
			// >, <, >=, <= not supported for vectors
			ErrorMessage = FString::Printf(TEXT("Operator '%s' not supported for Vector3 values"), *Operator);
			return false;
		}

		// Numeric comparison (existing logic)
		float LeftValue, RightValue;
		if (!Left.TryToNumeric(LeftValue, ErrorMessage) || !Right.TryToNumeric(RightValue, ErrorMessage))
		{
			return false;
		}

		if (Operator == TEXT("=="))
		{
			OutResult = FShidenExpressionValue(FMath::IsNearlyEqual(LeftValue, RightValue));
			return true;
		}

		if (Operator == TEXT("!="))
		{
			OutResult = FShidenExpressionValue(!FMath::IsNearlyEqual(LeftValue, RightValue));
			return true;
		}

		if (Operator == TEXT(">"))
		{
			OutResult = FShidenExpressionValue(LeftValue > RightValue);
			return true;
		}

		if (Operator == TEXT("<"))
		{
			OutResult = FShidenExpressionValue(LeftValue < RightValue);
			return true;
		}

		if (Operator == TEXT(">="))
		{
			OutResult = FShidenExpressionValue(LeftValue >= RightValue);
			return true;
		}

		// else case for "<="
		OutResult = FShidenExpressionValue(LeftValue <= RightValue);
		return true;
	}

	if (Operator == TEXT("&") || Operator == TEXT("|") || Operator == TEXT("^") || Operator == TEXT("<<") || Operator == TEXT(">>"))
	{
		int32 LeftInt, RightInt;
		if (!Left.TryToInteger(LeftInt, ErrorMessage) || !Right.TryToInteger(RightInt, ErrorMessage))
		{
			return false;
		}

		if (Operator == TEXT("&"))
		{
			OutResult = FShidenExpressionValue(LeftInt & RightInt);
			return true;
		}

		if (Operator == TEXT("|"))
		{
			OutResult = FShidenExpressionValue(LeftInt | RightInt);
			return true;
		}

		if (Operator == TEXT("^"))
		{
			OutResult = FShidenExpressionValue(LeftInt ^ RightInt);
			return true;
		}

		// Validate shift amount (must be non-negative and less than bit width)
		if (RightInt < 0)
		{
			ErrorMessage = TEXT("Shift amount must be non-negative");
			return false;
		}
		if (RightInt >= 32)
		{
			ErrorMessage = TEXT("Shift amount must be less than 32 for int32");
			return false;
		}

		if (Operator == TEXT("<<"))
		{
			OutResult = FShidenExpressionValue(LeftInt << RightInt);
			return true;
		}

		// else case for ">>"
		OutResult = FShidenExpressionValue(LeftInt >> RightInt);
		return true;
	}

	float LeftValue, RightValue;
	if (!Left.TryToNumeric(LeftValue, ErrorMessage) || !Right.TryToNumeric(RightValue, ErrorMessage))
	{
		return false;
	}

	if (Operator == TEXT("+"))
	{
		const float Result = LeftValue + RightValue;
		if (Left.Type == EShidenExpressionValueType::Integer && Right.Type == EShidenExpressionValueType::Integer)
		{
			OutResult = FShidenExpressionValue(static_cast<int32>(Result));
		}
		else
		{
			OutResult = FShidenExpressionValue(Result);
		}
		return true;
	}

	if (Operator == TEXT("-"))
	{
		const float Result = LeftValue - RightValue;
		if (Left.Type == EShidenExpressionValueType::Integer && Right.Type == EShidenExpressionValueType::Integer)
		{
			OutResult = FShidenExpressionValue(static_cast<int32>(Result));
		}
		else
		{
			OutResult = FShidenExpressionValue(Result);
		}
		return true;
	}

	if (Operator == TEXT("*"))
	{
		const float Result = LeftValue * RightValue;
		if (Left.Type == EShidenExpressionValueType::Integer && Right.Type == EShidenExpressionValueType::Integer)
		{
			OutResult = FShidenExpressionValue(static_cast<int32>(Result));
		}
		else
		{
			OutResult = FShidenExpressionValue(Result);
		}
		return true;
	}

	if (Operator == TEXT("/"))
	{
		if (FMath::IsNearlyZero(RightValue))
		{
			ErrorMessage = TEXT("Division by zero");
			return false;
		}
		const float Result = LeftValue / RightValue;
		// Division always returns float, even for integer operands
		// This matches the behavior of modern languages like Python 3 and JavaScript
		OutResult = FShidenExpressionValue(Result);
		return true;
	}

	if (Operator == TEXT("%"))
	{
		const float Result = FMath::Fmod(LeftValue, RightValue);
		if (Left.Type == EShidenExpressionValueType::Integer && Right.Type == EShidenExpressionValueType::Integer)
		{
			OutResult = FShidenExpressionValue(static_cast<int32>(Result));
		}
		else
		{
			OutResult = FShidenExpressionValue(Result);
		}
		return true;
	}

	if (Operator == TEXT("**"))
	{
		const float Result = FMath::Pow(LeftValue, RightValue);
		// Power operation always returns float, even for integer operands
		// This is necessary because negative exponents produce non-integer results
		OutResult = FShidenExpressionValue(Result);
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Unknown operator: %s"), *Operator);
	return false;
}

bool FShidenExpressionEvaluator::TryApplyUnaryOperation(const FString& Operator, const FShidenExpressionValue& Value, FShidenExpressionValue& OutResult,
                                                        FString& ErrorMessage)
{
	if (Operator == TEXT("!"))
	{
		bool BoolValue;
		if (!Value.TryToBoolean(BoolValue, ErrorMessage))
		{
			return false;
		}
		OutResult = FShidenExpressionValue(!BoolValue);
		return true;
	}

	if (Operator == TEXT("-"))
	{
		if (Value.Type == EShidenExpressionValueType::Vector2)
		{
			OutResult = FShidenExpressionValue(-Value.Vector2Value);
			return true;
		}
		if (Value.Type == EShidenExpressionValueType::Vector3)
		{
			OutResult = FShidenExpressionValue(-Value.Vector3Value);
			return true;
		}

		float NumericValue;
		if (!Value.TryToNumeric(NumericValue, ErrorMessage))
		{
			return false;
		}

		if (Value.Type == EShidenExpressionValueType::Integer)
		{
			OutResult = FShidenExpressionValue(-Value.IntValue);
		}
		else
		{
			OutResult = FShidenExpressionValue(-NumericValue);
		}
		return true;
	}

	if (Operator == TEXT("+"))
	{
		if (Value.Type == EShidenExpressionValueType::Vector2)
		{
			OutResult = FShidenExpressionValue(Value.Vector2Value);
			return true;
		}
		if (Value.Type == EShidenExpressionValueType::Vector3)
		{
			OutResult = FShidenExpressionValue(Value.Vector3Value);
			return true;
		}

		float NumericValue;
		if (!Value.TryToNumeric(NumericValue, ErrorMessage))
		{
			return false;
		}

		if (Value.Type == EShidenExpressionValueType::Integer)
		{
			OutResult = FShidenExpressionValue(Value.IntValue);
		}
		else
		{
			OutResult = FShidenExpressionValue(NumericValue);
		}
		return true;
	}

	if (Operator == TEXT("~"))
	{
		int32 IntValue;
		if (!Value.TryToInteger(IntValue, ErrorMessage))
		{
			return false;
		}
		OutResult = FShidenExpressionValue(~IntValue);
		return true;
	}

	ErrorMessage = FString::Printf(TEXT("Unknown unary operator: %s"), *Operator);
	return false;
}

bool FShidenExpressionEvaluator::TryEvaluateFunction(const FString& FunctionName, const TArray<FShidenExpressionValue>& Args,
                                                     FShidenExpressionValue& OutResult, FString& ErrorMessage)
{
	auto CheckArgCount = [&](const int32 Expected) -> bool
	{
		if (Args.Num() != Expected)
		{
			ErrorMessage = FString::Printf(TEXT("Function '%s' expects %d argument(s), got %d"), *FunctionName, Expected, Args.Num());
			return false;
		}
		return true;
	};

	auto GetNumericArg = [&](const int32 Index, float& OutValue) -> bool
	{
		if (Index >= Args.Num())
		{
			ErrorMessage = FString::Printf(TEXT("Argument %d out of range"), Index);
			return false;
		}
		return Args[Index].TryToNumeric(OutValue, ErrorMessage);
	};

	// Helper macros to reduce code duplication
	// Note: EXPR parameters can be any expression using Arg1, Arg2, Arg3 (e.g., FMath::Sin(Arg1))
	// Note: CheckArgCount and GetNumericArg set ErrorMessage on failure
#define MATH_FUNC_1_ARG(NAME, EXPR) \
		if (FunctionName.Equals(TEXT(NAME), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1; \
			if (!CheckArgCount(1) || !GetNumericArg(0, Arg1)) { return false; } /* ErrorMessage set by helpers */ \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

#define MATH_FUNC_2_ARG(NAME, EXPR) \
		if (FunctionName.Equals(TEXT(NAME), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1, Arg2; \
			if (!CheckArgCount(2) || !GetNumericArg(0, Arg1) || !GetNumericArg(1, Arg2)) { return false; } /* ErrorMessage set by helpers */ \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

#define MATH_FUNC_3_ARG(NAME, EXPR) \
		if (FunctionName.Equals(TEXT(NAME), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1, Arg2, Arg3; \
			if (!CheckArgCount(3) || !GetNumericArg(0, Arg1) || !GetNumericArg(1, Arg2) || !GetNumericArg(2, Arg3)) { return false; } /* ErrorMessage set by helpers */ \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

#define MATH_FUNC_1_ARG_ALIAS(NAME1, NAME2, EXPR) \
		if (FunctionName.Equals(TEXT(NAME1), ESearchCase::IgnoreCase) || FunctionName.Equals(TEXT(NAME2), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1; \
			if (!CheckArgCount(1) || !GetNumericArg(0, Arg1)) { return false; } /* ErrorMessage set by helpers */ \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

	// Validation macros for functions that require argument validation
#define MATH_FUNC_1_ARG_VALIDATE(NAME, EXPR, VALIDATION, ERROR_MSG) \
		if (FunctionName.Equals(TEXT(NAME), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1; \
			if (!CheckArgCount(1) || !GetNumericArg(0, Arg1)) { return false; } /* ErrorMessage set by helpers */ \
			if (!(VALIDATION)) \
			{ \
				ErrorMessage = TEXT(ERROR_MSG); /* Custom validation error */ \
				return false; \
			} \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

#define MATH_FUNC_1_ARG_ALIAS_VALIDATE(NAME1, NAME2, EXPR, VALIDATION, ERROR_MSG) \
		if (FunctionName.Equals(TEXT(NAME1), ESearchCase::IgnoreCase) || FunctionName.Equals(TEXT(NAME2), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1; \
			if (!CheckArgCount(1) || !GetNumericArg(0, Arg1)) { return false; } /* ErrorMessage set by helpers */ \
			if (!(VALIDATION)) \
			{ \
				ErrorMessage = TEXT(ERROR_MSG); /* Custom validation error */ \
				return false; \
			} \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

#define MATH_FUNC_3_ARG_VALIDATE(NAME, EXPR, VALIDATION, ERROR_MSG) \
		if (FunctionName.Equals(TEXT(NAME), ESearchCase::IgnoreCase)) \
		{ \
			float Arg1, Arg2, Arg3; \
			if (!CheckArgCount(3) || !GetNumericArg(0, Arg1) || !GetNumericArg(1, Arg2) || !GetNumericArg(2, Arg3)) { return false; } /* ErrorMessage set by helpers */ \
			if (!(VALIDATION)) \
			{ \
				ErrorMessage = TEXT(ERROR_MSG); /* Custom validation error */ \
				return false; \
			} \
			OutResult = FShidenExpressionValue(EXPR); \
			return true; \
		}

	// Trigonometric functions
	MATH_FUNC_1_ARG("sin", FMath::Sin(Arg1))
	MATH_FUNC_1_ARG("cos", FMath::Cos(Arg1))
	MATH_FUNC_1_ARG("tan", FMath::Tan(Arg1))
	MATH_FUNC_1_ARG("asin", FMath::Asin(Arg1))
	MATH_FUNC_1_ARG("acos", FMath::Acos(Arg1))
	MATH_FUNC_1_ARG("atan", FMath::Atan(Arg1))
	MATH_FUNC_2_ARG("atan2", FMath::Atan2(Arg1, Arg2))

	// Square root with non-negative validation
	MATH_FUNC_1_ARG_VALIDATE("sqrt", FMath::Sqrt(Arg1), Arg1 >= 0, "sqrt: argument must be non-negative")

	// Power and exponential functions
	MATH_FUNC_2_ARG("pow", FMath::Pow(Arg1, Arg2))
	MATH_FUNC_1_ARG("exp", FMath::Exp(Arg1))

	// Logarithmic functions with positive validation
	MATH_FUNC_1_ARG_ALIAS_VALIDATE("log", "ln", FMath::Loge(Arg1), Arg1 > 0, "log: argument must be positive")
	MATH_FUNC_1_ARG_VALIDATE("log10", FMath::LogX(10.0, Arg1), Arg1 > 0, "log10: argument must be positive")

	// Rounding and absolute value functions
	MATH_FUNC_1_ARG("abs", FMath::Abs(Arg1))
	MATH_FUNC_1_ARG("floor", FMath::FloorToDouble(Arg1))
	MATH_FUNC_1_ARG("ceil", FMath::CeilToDouble(Arg1))
	MATH_FUNC_1_ARG("round", FMath::RoundToDouble(Arg1))
	MATH_FUNC_1_ARG("trunc", FMath::TruncToDouble(Arg1))
	MATH_FUNC_1_ARG("frac", FMath::Frac(Arg1))

	// Min/Max/Clamp functions
	MATH_FUNC_2_ARG("min", FMath::Min(Arg1, Arg2))
	MATH_FUNC_2_ARG("max", FMath::Max(Arg1, Arg2))
	MATH_FUNC_3_ARG("clamp", FMath::Clamp(Arg1, Arg2, Arg3))

	// Interpolation and utility functions
	MATH_FUNC_3_ARG("lerp", FMath::Lerp(Arg1, Arg2, Arg3))
	MATH_FUNC_3_ARG_VALIDATE("invlerp", (Arg3 - Arg1) / (Arg2 - Arg1), !FMath::IsNearlyEqual(Arg2, Arg1), "invlerp: min and max must be different")
	MATH_FUNC_1_ARG("sign", FMath::Sign(Arg1))

	// Angle conversion functions
	MATH_FUNC_1_ARG_ALIAS("radians", "deg2rad", FMath::DegreesToRadians(Arg1))
	MATH_FUNC_1_ARG_ALIAS("degrees", "rad2deg", FMath::RadiansToDegrees(Arg1))

	// Clean up macros
#undef MATH_FUNC_1_ARG
#undef MATH_FUNC_2_ARG
#undef MATH_FUNC_3_ARG
#undef MATH_FUNC_1_ARG_ALIAS
#undef MATH_FUNC_1_ARG_VALIDATE
#undef MATH_FUNC_1_ARG_ALIAS_VALIDATE
#undef MATH_FUNC_3_ARG_VALIDATE

	ErrorMessage = FString::Printf(TEXT("Unknown function: %s"), *FunctionName);
	return false;
}
