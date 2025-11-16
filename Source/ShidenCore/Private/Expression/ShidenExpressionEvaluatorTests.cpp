// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Expression/ShidenExpressionEvaluator.h"
#include "Misc/DefaultValueHelper.h"
#include "Misc/Base64.h"

// Test parameter structure
struct FShidenExpressionTestParameters
{
	FString Expression;
	FString ExpectedResult;
	bool bShouldSucceed;

	FString ToString() const
	{
		const FString EncodedExpression = FBase64::Encode(Expression);
		const FString EncodedExpectedResult = FBase64::Encode(ExpectedResult);
		return FString::Printf(TEXT("%s\t%s\t%d"), *EncodedExpression, *EncodedExpectedResult, bShouldSucceed ? 1 : 0);
	}

	FShidenExpressionTestParameters(const FString& InExpression, const FString& InExpectedResult, const bool bInShouldSucceed = true)
		: Expression(InExpression), ExpectedResult(InExpectedResult), bShouldSucceed(bInShouldSucceed)
	{
	}

	explicit FShidenExpressionTestParameters(const FString& Parameters)
		: Expression(TEXT("")), ExpectedResult(TEXT("")), bShouldSucceed(true)
	{
		TArray<FString> Parts;
		Parameters.ParseIntoArray(Parts, TEXT("\t"), false);
		if (Parts.Num() >= 3)
		{
			FBase64::Decode(Parts[0], Expression);
			FBase64::Decode(Parts[1], ExpectedResult);
			bShouldSucceed = FCString::Atoi(*Parts[2]) != 0;
		}
	}
};

// Arithmetic expression tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorArithmeticTest, "ShidenExpressionEvaluator.Arithmetic",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorArithmeticTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Basic Addition tests
	OutBeautifiedNames.Add("SimpleAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("FloatAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2.5 + 3.5"), TEXT("6")).ToString());

	OutBeautifiedNames.Add("NegativeAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-5 + 3"), TEXT("-2")).ToString());

	OutBeautifiedNames.Add("AdditionWithZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 + 0"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("LargeNumberAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("999999 + 1"), TEXT("1000000")).ToString());

	OutBeautifiedNames.Add("SmallNumberAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0.001 + 0.002"), TEXT("0.003")).ToString());

	OutBeautifiedNames.Add("MultipleAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 + 2 + 3 + 4"), TEXT("10")).ToString());

	// Basic Subtraction tests
	OutBeautifiedNames.Add("SimpleSubtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 - 2"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("FloatSubtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("7.5 - 2.3"), TEXT("5.2")).ToString());

	OutBeautifiedNames.Add("NegativeSubtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 - 5"), TEXT("-2")).ToString());

	OutBeautifiedNames.Add("SubtractionWithZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 - 0"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("SubtractionFromZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 - 5"), TEXT("-5")).ToString());

	OutBeautifiedNames.Add("SubtractionNegativeNumber");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 - (-3)"), TEXT("8")).ToString());

	OutBeautifiedNames.Add("MultipleSubtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 - 3 - 2 - 1"), TEXT("4")).ToString());

	// Basic Multiplication tests
	OutBeautifiedNames.Add("SimpleMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * 3"), TEXT("6")).ToString());

	OutBeautifiedNames.Add("FloatMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2.5 * 4"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("NegativeMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-3 * 4"), TEXT("-12")).ToString());

	OutBeautifiedNames.Add("NegativeByNegativeMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-3 * -4"), TEXT("12")).ToString());

	OutBeautifiedNames.Add("MultiplicationByZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 * 0"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("MultiplicationByOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 * 1"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("MultipleMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * 3 * 4"), TEXT("24")).ToString());

	OutBeautifiedNames.Add("LargeNumberMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1000 * 1000"), TEXT("1000000")).ToString());

	OutBeautifiedNames.Add("SmallNumberMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0.1 * 0.2"), TEXT("0.02")).ToString());

	// Basic Division tests
	OutBeautifiedNames.Add("SimpleDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("6 / 2"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("FloatDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10.0 / 4.0"), TEXT("2.5")).ToString());

	OutBeautifiedNames.Add("DivisionResultingInFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("7 / 2"), TEXT("3.5")).ToString());

	OutBeautifiedNames.Add("NegativeDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-12 / 4"), TEXT("-3")).ToString());

	OutBeautifiedNames.Add("NegativeByNegativeDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-12 / -4"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("DivisionByOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 / 1"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("MultipleDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("24 / 4 / 2"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("SmallNumberDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0.1 / 0.5"), TEXT("0.2")).ToString());

	// Modulo tests
	OutBeautifiedNames.Add("SimpleModulo");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("7 % 3"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("ModuloResultZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("6 % 3"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("ModuloLargerDivisor");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 % 7"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("ModuloNegativeDividend");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-7 % 3"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("ModuloNegativeDivisor");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("7 % -3"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("MultipleModulo");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("17 % 5 % 2"), TEXT("0")).ToString());

	// Operator Precedence tests
	OutBeautifiedNames.Add("AdditionMultiplicationPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3 * 4"), TEXT("14")).ToString());

	OutBeautifiedNames.Add("SubtractionDivisionPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 - 8 / 2"), TEXT("6")).ToString());

	OutBeautifiedNames.Add("MixedPrecedence1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3 * 4 - 5"), TEXT("9")).ToString());

	OutBeautifiedNames.Add("MixedPrecedence2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 / 2 + 3 * 4"), TEXT("17")).ToString());

	OutBeautifiedNames.Add("ModuloPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 + 7 % 3"), TEXT("11")).ToString());

	// Parentheses tests
	OutBeautifiedNames.Add("SimpleParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(1 + 2) * 6"), TEXT("18")).ToString());

	OutBeautifiedNames.Add("ParenthesesOverridePrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(2 + 3) * (4 + 5)"), TEXT("45")).ToString());

	OutBeautifiedNames.Add("NestedParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("((2 + 3) * (4 + 5))"), TEXT("45")).ToString());

	OutBeautifiedNames.Add("DeeplyNestedParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(((1 + 2) * 3) + 4)"), TEXT("13")).ToString());

	OutBeautifiedNames.Add("MultipleParenthesesGroups");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(1 + 2) * (3 + 4) - (5 + 6)"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("ParenthesesWithNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-(3 + 4)"), TEXT("-7")).ToString());

	// Complex expression tests
	OutBeautifiedNames.Add("ComplexExpression1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3 * 4 - 5 / 2"), TEXT("11.5")).ToString());

	OutBeautifiedNames.Add("ComplexExpression2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(10 + 5) * 2 - 8 / 4"), TEXT("28")).ToString());

	OutBeautifiedNames.Add("ComplexExpression3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("100 / (2 + 3) * 4"), TEXT("80")).ToString());

	OutBeautifiedNames.Add("ComplexExpression4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("((5 + 3) * 2 - 6) / 2"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("ComplexExpression5");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 % 4 * 3 + 2"), TEXT("11")).ToString());

	// Edge cases
	OutBeautifiedNames.Add("ZeroPlusZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 + 0"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("ZeroMinusZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 - 0"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("ZeroTimesZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 * 0"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("VeryLongExpression");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 + 2 - 3 * 4 / 2 + 5 - 1"), TEXT("1")).ToString());
}

bool ShidenExpressionEvaluatorArithmeticTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);

	if (bSuccess != Params.bShouldSucceed)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluation returned %s, expected %s. Error: %s"),
		                         *Params.Expression,
		                         bSuccess ? TEXT("success") : TEXT("failure"),
		                         Params.bShouldSucceed ? TEXT("success") : TEXT("failure"),
		                         *ErrorMessage));
		return false;
	}

	if (bSuccess)
	{
		const FString ResultStr = Result.ToString();
		float ResultValue, ExpectedValue;

		if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
			FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
		{
			if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
			{
				AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
				                         *Params.Expression, ResultValue, ExpectedValue));
				return false;
			}
		}
		else if (ResultStr != Params.ExpectedResult)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to '%s', expected '%s'"),
			                         *Params.Expression, *ResultStr, *Params.ExpectedResult));
			return false;
		}
	}

	return true;
}

// Power operator tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorPowerTest, "ShidenExpressionEvaluator.Power",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorPowerTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Basic power tests
	OutBeautifiedNames.Add("SimplePower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 ** 3"), TEXT("8")).ToString());

	OutBeautifiedNames.Add("PowerOfZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 ** 0"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("PowerOfOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 ** 1"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("ZeroToThePower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 ** 5"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("OneToThePower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 ** 100"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("NegativeBase");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(-2) ** 3"), TEXT("-8")).ToString());

	OutBeautifiedNames.Add("NegativeBaseEvenExponent");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(-2) ** 2"), TEXT("4")).ToString());

	OutBeautifiedNames.Add("NegativeBaseOddExponent");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(-3) ** 3"), TEXT("-27")).ToString());

	OutBeautifiedNames.Add("FloatPower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2.5 ** 2"), TEXT("6.25")).ToString());

	OutBeautifiedNames.Add("PowerOfTwo");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 ** 10"), TEXT("1024")).ToString());

	OutBeautifiedNames.Add("PowerOfTen");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 ** 3"), TEXT("1000")).ToString());

	// Fractional exponents
	OutBeautifiedNames.Add("SquareRootAsPower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("4 ** 0.5"), TEXT("2")).ToString());

	OutBeautifiedNames.Add("CubeRootAsPower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("8 ** 0.333333"), TEXT("2")).ToString());

	OutBeautifiedNames.Add("FractionalExponent");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("16 ** 0.25"), TEXT("2")).ToString());

	// Negative exponents
	OutBeautifiedNames.Add("NegativeExponent");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 ** -1"), TEXT("0.5")).ToString());

	OutBeautifiedNames.Add("NegativeExponent2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 ** -2"), TEXT("0.01")).ToString());

	OutBeautifiedNames.Add("NegativeExponent3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("4 ** -2"), TEXT("0.0625")).ToString());

	// Precedence tests
	OutBeautifiedNames.Add("PowerMultiplicationPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * 3 ** 2"), TEXT("18")).ToString());

	OutBeautifiedNames.Add("PowerAdditionPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3 ** 2"), TEXT("11")).ToString());

	OutBeautifiedNames.Add("PowerSubtractionPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 - 2 ** 3"), TEXT("2")).ToString());

	OutBeautifiedNames.Add("PowerDivisionPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("100 / 10 ** 2"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("MultiplePowerOperators");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 ** 2 ** 2"), TEXT("16")).ToString());

	// Parentheses with power
	OutBeautifiedNames.Add("ParenthesesOverridePowerPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(2 * 3) ** 2"), TEXT("36")).ToString());

	OutBeautifiedNames.Add("PowerInParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * (3 ** 2)"), TEXT("18")).ToString());

	OutBeautifiedNames.Add("ComplexParenthesesPower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(2 + 3) ** (1 + 1)"), TEXT("25")).ToString());

	OutBeautifiedNames.Add("NestedParenthesesPower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("((2 + 1) ** 2) ** 2"), TEXT("81")).ToString());

	// Complex expressions
	OutBeautifiedNames.Add("PowerInComplexExpression1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 ** 3 + 3 ** 2"), TEXT("17")).ToString());

	OutBeautifiedNames.Add("PowerInComplexExpression2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 ** 2 / 5 + 2 ** 3"), TEXT("28")).ToString());

	OutBeautifiedNames.Add("PowerInComplexExpression3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 + 3) ** 2 - 4 ** 2"), TEXT("48")).ToString());

	OutBeautifiedNames.Add("PowerInComplexExpression4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 ** 3 / 3 + 2 ** 4"), TEXT("25")).ToString());

	// Large powers
	OutBeautifiedNames.Add("LargePower");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 ** 20"), TEXT("1048576")).ToString());

	OutBeautifiedNames.Add("PowerOfLargeNumber");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("100 ** 2"), TEXT("10000")).ToString());
}

bool ShidenExpressionEvaluatorPowerTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	float ResultValue, ExpectedValue;
	if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
		FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
	{
		if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
			                         *Params.Expression, ResultValue, ExpectedValue));
			return false;
		}
	}

	return true;
}

// String concatenation tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorStringTest, "ShidenExpressionEvaluator.String",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorStringTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Basic concatenation tests
	OutBeautifiedNames.Add("SimpleConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\" + \" World\""), TEXT("Hello World")).ToString());

	OutBeautifiedNames.Add("TwoStringsConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\" + \"World\""), TEXT("HelloWorld")).ToString());

	OutBeautifiedNames.Add("EmptyStringConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" + \"Hello\""), TEXT("Hello")).ToString());

	OutBeautifiedNames.Add("ConcatenationWithEmptyString");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\" + \"\""), TEXT("Hello")).ToString());

	OutBeautifiedNames.Add("EmptyStringsConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" + \"\""), TEXT("")).ToString());

	// Multiple concatenation tests
	OutBeautifiedNames.Add("MultipleConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"A\" + \"B\" + \"C\""), TEXT("ABC")).ToString());

	OutBeautifiedNames.Add("FourStringsConcatenation");
	// ReSharper disable once StringLiteralTypo
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"A\" + \"B\" + \"C\" + \"D\""), TEXT("ABCD")).ToString());

	OutBeautifiedNames.Add("LongConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\" + \" \" + \"beautiful\" + \" \" + \"world\""), TEXT("Hello beautiful world")).ToString());

	// String and number concatenation
	OutBeautifiedNames.Add("StringAndNumberConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\" + \" World\" + 2"), TEXT("Hello World2")).ToString());

	OutBeautifiedNames.Add("NumberAndStringConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Value: \" + 42"), TEXT("Value: 42")).ToString());

	OutBeautifiedNames.Add("StringAndFloatConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"PI: \" + 3.14"), TEXT("PI: 3.140000")).ToString());

	OutBeautifiedNames.Add("StringAndNegativeNumberConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Temperature: \" + -5"), TEXT("Temperature: -5")).ToString());

	OutBeautifiedNames.Add("StringAndZeroConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Count: \" + 0"), TEXT("Count: 0")).ToString());

	OutBeautifiedNames.Add("MixedStringAndNumbers");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Result: \" + 10 + \" + \" + 20 + \" = \" + 30"), TEXT("Result: 10 + 20 = 30")).ToString());

	// Special characters
	OutBeautifiedNames.Add("StringWithSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello   World\""), TEXT("Hello   World")).ToString());

	OutBeautifiedNames.Add("StringWithSpecialChars");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello!@#$%\""), TEXT("Hello!@#$%")).ToString());

	OutBeautifiedNames.Add("StringWithNumbers");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Test123\""), TEXT("Test123")).ToString());

	// Parentheses with strings
	OutBeautifiedNames.Add("ConcatenationWithParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(\"Hello\" + \" \") + \"World\""), TEXT("Hello World")).ToString());

	OutBeautifiedNames.Add("NestedStringParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Start \" + (\"Middle\" + \" End\")"), TEXT("Start Middle End")).ToString());

	// Long strings
	OutBeautifiedNames.Add("LongString");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"This is a very long string that should be handled correctly\""), TEXT("This is a very long string that should be handled correctly")).ToString());

	OutBeautifiedNames.Add("RepeatedConcatenation");
	// ReSharper disable once StringLiteralTypo
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"A\" + \"A\" + \"A\" + \"A\" + \"A\""), TEXT("AAAAA")).ToString());

	// Single character strings
	OutBeautifiedNames.Add("SingleChar");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"X\""), TEXT("X")).ToString());

	OutBeautifiedNames.Add("SingleCharConcatenation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"A\" + \"B\""), TEXT("AB")).ToString());
}

bool ShidenExpressionEvaluatorStringTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	if (Result.Type != EShidenExpressionValueType::String)
	{
		AddError(FString::Printf(TEXT("Expression '%s' did not return string type"), *Params.Expression));
		return false;
	}

	if (Result.StringValue != Params.ExpectedResult)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluated to '%s', expected '%s'"),
		                         *Params.Expression, *Result.StringValue, *Params.ExpectedResult));
		return false;
	}

	return true;
}

// Boolean and comparison tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorComparisonTest, "ShidenExpressionEvaluator.Comparison",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorComparisonTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Greater than tests
	OutBeautifiedNames.Add("GreaterThan_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 > 3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("GreaterThan_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 > 5"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("GreaterThan_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 > 5"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("GreaterThan_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-3 > -5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("GreaterThan_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5.5 > 5.1"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("GreaterThan_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 > 0"), TEXT("true")).ToString());

	// Less than tests
	OutBeautifiedNames.Add("LessThan_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 < 5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LessThan_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 < 3"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LessThan_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 < 5"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LessThan_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-5 < -3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LessThan_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5.1 < 5.5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LessThan_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 < 1"), TEXT("true")).ToString());

	// Greater or equal tests
	OutBeautifiedNames.Add("GreaterOrEqual_Greater");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 >= 3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("GreaterOrEqual_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 >= 5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("GreaterOrEqual_Less");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 >= 5"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("GreaterOrEqual_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-3 >= -5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("GreaterOrEqual_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5.5 >= 5.5"), TEXT("true")).ToString());

	// Less or equal tests
	OutBeautifiedNames.Add("LessOrEqual_Less");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 <= 5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LessOrEqual_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 <= 5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LessOrEqual_Greater");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 <= 3"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LessOrEqual_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-5 <= -3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LessOrEqual_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5.1 <= 5.1"), TEXT("true")).ToString());

	// Equal tests
	OutBeautifiedNames.Add("Equal_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 == 5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Equal_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 == 3"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("Equal_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-5 == -5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Equal_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5.5 == 5.5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Equal_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 == 0"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Equal_IntAndFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 == 5.0"), TEXT("true")).ToString());

	// Not equal tests
	OutBeautifiedNames.Add("NotEqual_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 != 3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("NotEqual_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 != 5"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("NotEqual_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-5 != -3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("NotEqual_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5.1 != 5.2"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("NotEqual_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 != 1"), TEXT("true")).ToString());

	// Logical AND tests
	OutBeautifiedNames.Add("LogicalAnd_TrueTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalAnd_TrueFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && false"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LogicalAnd_FalseTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false && true"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LogicalAnd_FalseFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false && false"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LogicalAnd_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && true && true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalAnd_WithComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 > 3 && 2 < 4"), TEXT("true")).ToString());

	// Logical OR tests
	OutBeautifiedNames.Add("LogicalOr_TrueTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true || true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalOr_TrueFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true || false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalOr_FalseTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false || true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalOr_FalseFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false || false"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LogicalOr_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false || false || true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalOr_WithComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 < 3 || 2 < 4"), TEXT("true")).ToString());

	// Logical NOT tests
	OutBeautifiedNames.Add("LogicalNot_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("LogicalNot_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!true"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LogicalNot_WithComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!(5 > 3)"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("LogicalNot_DoubleNegation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!!true"), TEXT("true")).ToString());

	// Complex conditions
	OutBeautifiedNames.Add("ComplexCondition1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && 3 < 5"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3) && (2 < 4)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3) || (2 > 4)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!(5 < 3) && (2 < 4)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition5");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3 && 2 < 4) || false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition6");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && (false || true)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition7");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 >= 5) && (3 <= 3)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexCondition8");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(10 > 5) && (20 > 15) && (30 > 25)"), TEXT("true")).ToString());
}

bool ShidenExpressionEvaluatorComparisonTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	bool bResultBool;
	if (!Result.TryToBoolean(bResultBool, ErrorMessage))
	{
		AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
		                         *Params.Expression, *ErrorMessage));
		return false;
	}

	const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
	if (bResultBool != bExpected)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
		                         *Params.Expression,
		                         bResultBool ? TEXT("true") : TEXT("false"),
		                         bExpected ? TEXT("true") : TEXT("false")));
		return false;
	}

	return true;
}

// Bitwise operation tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorBitwiseTest, "ShidenExpressionEvaluator.Bitwise",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorBitwiseTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Bitwise AND tests
	OutBeautifiedNames.Add("BitwiseAnd_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 & 10"), TEXT("8")).ToString()); // 1100 & 1010 = 1000

	OutBeautifiedNames.Add("BitwiseAnd_AllBits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 & 15"), TEXT("15")).ToString()); // 1111 & 1111 = 1111

	OutBeautifiedNames.Add("BitwiseAnd_ZeroBits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 & 3"), TEXT("0")).ToString()); // 1100 & 0011 = 0000

	OutBeautifiedNames.Add("BitwiseAnd_WithZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 & 0"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("BitwiseAnd_LargeBits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("255 & 127"), TEXT("127")).ToString()); // 11111111 & 01111111 = 01111111

	OutBeautifiedNames.Add("BitwiseAnd_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 & 7 & 3"), TEXT("3")).ToString());

	// Bitwise OR tests
	OutBeautifiedNames.Add("BitwiseOr_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 | 10"), TEXT("14")).ToString()); // 1100 | 1010 = 1110

	OutBeautifiedNames.Add("BitwiseOr_AllBits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 | 15"), TEXT("15")).ToString());

	OutBeautifiedNames.Add("BitwiseOr_WithZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 | 0"), TEXT("15")).ToString());

	OutBeautifiedNames.Add("BitwiseOr_NoBitsOverlap");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 | 3"), TEXT("15")).ToString()); // 1100 | 0011 = 1111

	OutBeautifiedNames.Add("BitwiseOr_LargeBits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("128 | 127"), TEXT("255")).ToString()); // 10000000 | 01111111 = 11111111

	OutBeautifiedNames.Add("BitwiseOr_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 | 2 | 4"), TEXT("7")).ToString());

	// Bitwise XOR tests
	OutBeautifiedNames.Add("BitwiseXor_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 ^ 10"), TEXT("6")).ToString()); // 1100 ^ 1010 = 0110

	OutBeautifiedNames.Add("BitwiseXor_Same");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 ^ 15"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("BitwiseXor_WithZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("15 ^ 0"), TEXT("15")).ToString());

	OutBeautifiedNames.Add("BitwiseXor_AllBits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("255 ^ 255"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("BitwiseXor_OppositeValues");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 ^ 3"), TEXT("15")).ToString()); // 1100 ^ 0011 = 1111

	OutBeautifiedNames.Add("BitwiseXor_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 ^ 3 ^ 1"), TEXT("7")).ToString());

	// Left shift tests
	OutBeautifiedNames.Add("LeftShift_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 << 2"), TEXT("12")).ToString()); // 11 << 2 = 1100

	OutBeautifiedNames.Add("LeftShift_ByZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 << 0"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("LeftShift_ByOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 << 1"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("LeftShift_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 << 8"), TEXT("256")).ToString());

	OutBeautifiedNames.Add("LeftShift_PowerOfTwo");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 << 10"), TEXT("1024")).ToString());

	OutBeautifiedNames.Add("LeftShift_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 << 1 << 1"), TEXT("8")).ToString());

	// Right shift tests
	OutBeautifiedNames.Add("RightShift_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("12 >> 2"), TEXT("3")).ToString()); // 1100 >> 2 = 11

	OutBeautifiedNames.Add("RightShift_ByZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 >> 0"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("RightShift_ByOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10 >> 1"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("RightShift_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("256 >> 8"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("RightShift_ToZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("7 >> 3"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("RightShift_Multiple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("32 >> 2 >> 1"), TEXT("4")).ToString());

	// Bitwise NOT tests
	OutBeautifiedNames.Add("BitwiseNot_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("~0"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("BitwiseNot_MinusOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("~-1"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("BitwiseNot_PositiveNumber");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("~5"), TEXT("-6")).ToString());

	OutBeautifiedNames.Add("BitwiseNot_DoubleNot");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("~~5"), TEXT("5")).ToString());

	// Combined operations
	OutBeautifiedNames.Add("BitwiseAndOr");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(12 & 10) | (3 & 1)"), TEXT("9")).ToString());

	OutBeautifiedNames.Add("BitwiseAndXor");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(15 & 7) ^ (8 & 4)"), TEXT("7")).ToString());

	OutBeautifiedNames.Add("BitwiseShiftAndAnd");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(8 << 2) & 31"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("BitwiseShiftAndOr");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(4 << 2) | 3"), TEXT("19")).ToString());

	OutBeautifiedNames.Add("ComplexBitwise1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 & 3) | (2 << 1)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("ComplexBitwise2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("((8 >> 1) | 3) & 7"), TEXT("7")).ToString());

	OutBeautifiedNames.Add("ComplexBitwise3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("~(12 & 10)"), TEXT("-9")).ToString());
}

bool ShidenExpressionEvaluatorBitwiseTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	int32 ResultInt, ExpectedInt;
	if (Result.TryToInteger(ResultInt, ErrorMessage) &&
		FDefaultValueHelper::ParseInt(Params.ExpectedResult, ExpectedInt))
	{
		if (ResultInt != ExpectedInt)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %d, expected %d"),
			                         *Params.Expression, ResultInt, ExpectedInt));
			return false;
		}
	}

	return true;
}

// Vector operation tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorVectorTest, "ShidenExpressionEvaluator.Vector",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorVectorTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Vector2 Literal tests
	OutBeautifiedNames.Add("Vector2Literal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2]"), TEXT("X=1.000000 Y=2.000000")).ToString());

	OutBeautifiedNames.Add("Vector2LiteralFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1.5,2.5]"), TEXT("X=1.500000 Y=2.500000")).ToString());

	OutBeautifiedNames.Add("Vector2LiteralNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[-1,-2]"), TEXT("X=-1.000000 Y=-2.000000")).ToString());

	OutBeautifiedNames.Add("Vector2LiteralZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0,0]"), TEXT("X=0.000000 Y=0.000000")).ToString());

	OutBeautifiedNames.Add("Vector2LiteralMixed");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[-1.5,2.5]"), TEXT("X=-1.500000 Y=2.500000")).ToString());

	// Vector3 Literal tests
	OutBeautifiedNames.Add("Vector3Literal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3]"), TEXT("X=1.000000 Y=2.000000 Z=3.000000")).ToString());

	OutBeautifiedNames.Add("Vector3LiteralFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1.5,2.5,3.5]"), TEXT("X=1.500000 Y=2.500000 Z=3.500000")).ToString());

	OutBeautifiedNames.Add("Vector3LiteralNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[-1,-2,-3]"), TEXT("X=-1.000000 Y=-2.000000 Z=-3.000000")).ToString());

	OutBeautifiedNames.Add("Vector3LiteralZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0,0,0]"), TEXT("X=0.000000 Y=0.000000 Z=0.000000")).ToString());

	OutBeautifiedNames.Add("Vector3LiteralMixed");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[-1.5,2.5,-3.5]"), TEXT("X=-1.500000 Y=2.500000 Z=-3.500000")).ToString());

	// Vector2 Addition tests
	OutBeautifiedNames.Add("Vector2Addition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] + [3,4]"), TEXT("X=4.000000 Y=6.000000")).ToString());

	OutBeautifiedNames.Add("Vector2AdditionFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2.1] + [3.1,4]"), TEXT("X=4.100000 Y=6.100000")).ToString());

	OutBeautifiedNames.Add("Vector2AdditionNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6] + [-2,-3]"), TEXT("X=3.000000 Y=3.000000")).ToString());

	OutBeautifiedNames.Add("Vector2AdditionZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6] + [0,0]"), TEXT("X=5.000000 Y=6.000000")).ToString());

	OutBeautifiedNames.Add("Vector2AdditionLarge");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1000,2000] + [3000,4000]"), TEXT("X=4000.000000 Y=6000.000000")).ToString());

	OutBeautifiedNames.Add("Vector2AdditionSmall");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0.001,0.002] + [0.003,0.004]"), TEXT("X=0.004000 Y=0.006000")).ToString());

	// Vector3 Addition tests
	OutBeautifiedNames.Add("Vector3Addition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] + [4,5,6]"), TEXT("X=5.000000 Y=7.000000 Z=9.000000")).ToString());

	OutBeautifiedNames.Add("Vector3AdditionFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1.1,2,3.1] + [4,5.1,6.1]"), TEXT("X=5.100000 Y=7.100000 Z=9.200000")).ToString());

	OutBeautifiedNames.Add("Vector3AdditionNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6,7] + [-2,-3,-4]"), TEXT("X=3.000000 Y=3.000000 Z=3.000000")).ToString());

	OutBeautifiedNames.Add("Vector3AdditionZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6,7] + [0,0,0]"), TEXT("X=5.000000 Y=6.000000 Z=7.000000")).ToString());

	OutBeautifiedNames.Add("Vector3AdditionLarge");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1000,2000,3000] + [4000,5000,6000]"), TEXT("X=5000.000000 Y=7000.000000 Z=9000.000000")).ToString());

	OutBeautifiedNames.Add("Vector3AdditionSmall");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0.001,0.002,0.003] + [0.004,0.005,0.006]"), TEXT("X=0.005000 Y=0.007000 Z=0.009000")).ToString());

	// Vector2 Subtraction tests
	OutBeautifiedNames.Add("Vector2Subtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,7] - [2,3]"), TEXT("X=3.000000 Y=4.000000")).ToString());

	OutBeautifiedNames.Add("Vector2SubtractionFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5.5,7.5] - [2.2,3.3]"), TEXT("X=3.300000 Y=4.200000")).ToString());

	OutBeautifiedNames.Add("Vector2SubtractionNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[2,3] - [5,7]"), TEXT("X=-3.000000 Y=-4.000000")).ToString());

	OutBeautifiedNames.Add("Vector2SubtractionZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6] - [0,0]"), TEXT("X=5.000000 Y=6.000000")).ToString());

	OutBeautifiedNames.Add("Vector2SubtractionFromZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0,0] - [3,4]"), TEXT("X=-3.000000 Y=-4.000000")).ToString());

	// Vector3 Subtraction tests
	OutBeautifiedNames.Add("Vector3Subtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20,30] - [5,10,15]"), TEXT("X=5.000000 Y=10.000000 Z=15.000000")).ToString());

	OutBeautifiedNames.Add("Vector3SubtractionFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5.5,7.5,9.5] - [2.2,3.3,4.4]"), TEXT("X=3.300000 Y=4.200000 Z=5.100000")).ToString());

	OutBeautifiedNames.Add("Vector3SubtractionNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[2,3,4] - [5,7,9]"), TEXT("X=-3.000000 Y=-4.000000 Z=-5.000000")).ToString());

	OutBeautifiedNames.Add("Vector3SubtractionZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6,7] - [0,0,0]"), TEXT("X=5.000000 Y=6.000000 Z=7.000000")).ToString());

	OutBeautifiedNames.Add("Vector3SubtractionFromZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0,0,0] - [3,4,5]"), TEXT("X=-3.000000 Y=-4.000000 Z=-5.000000")).ToString());

	// Vector2 Scalar Multiplication tests
	OutBeautifiedNames.Add("Vector2ScalarMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] * 2"), TEXT("X=2.000000 Y=4.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ScalarMultiplicationFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1.5,2.5] * 2.5"), TEXT("X=3.750000 Y=6.250000")).ToString());

	OutBeautifiedNames.Add("Vector2ScalarMultiplicationNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] * -2"), TEXT("X=-2.000000 Y=-4.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ScalarMultiplicationZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6] * 0"), TEXT("X=0.000000 Y=0.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ScalarMultiplicationLeftSide");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * [1,2]"), TEXT("X=2.000000 Y=4.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ScalarMultiplicationLeftSideFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2.5 * [1.5,2.5]"), TEXT("X=3.750000 Y=6.250000")).ToString());

	OutBeautifiedNames.Add("Vector2ScalarMultiplicationLeftSideNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-2 * [1,2]"), TEXT("X=-2.000000 Y=-4.000000")).ToString());

	// Vector3 Scalar Multiplication tests
	OutBeautifiedNames.Add("Vector3ScalarMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] * 2"), TEXT("X=2.000000 Y=4.000000 Z=6.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ScalarMultiplicationFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1.5,2.5,3.5] * 2.5"), TEXT("X=3.750000 Y=6.250000 Z=8.750000")).ToString());

	OutBeautifiedNames.Add("Vector3ScalarMultiplicationNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] * -2"), TEXT("X=-2.000000 Y=-4.000000 Z=-6.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ScalarMultiplicationZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6,7] * 0"), TEXT("X=0.000000 Y=0.000000 Z=0.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ScalarMultiplicationLeftSide");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 * [1,2,3]"), TEXT("X=3.000000 Y=6.000000 Z=9.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ScalarMultiplicationLeftSideFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2.5 * [1.5,2.5,3.5]"), TEXT("X=3.750000 Y=6.250000 Z=8.750000")).ToString());

	OutBeautifiedNames.Add("Vector3ScalarMultiplicationLeftSideNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-2 * [1,2,3]"), TEXT("X=-2.000000 Y=-4.000000 Z=-6.000000")).ToString());

	// Vector2 Division tests
	OutBeautifiedNames.Add("Vector2Division");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20] / 2"), TEXT("X=5.000000 Y=10.000000")).ToString());

	OutBeautifiedNames.Add("Vector2DivisionFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10.5,21] / 2.5"), TEXT("X=4.200000 Y=8.400000")).ToString());

	OutBeautifiedNames.Add("Vector2DivisionNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20] / -2"), TEXT("X=-5.000000 Y=-10.000000")).ToString());

	OutBeautifiedNames.Add("Vector2DivisionByOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6] / 1"), TEXT("X=5.000000 Y=6.000000")).ToString());

	// Vector3 Division tests
	OutBeautifiedNames.Add("Vector3Division");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[9,18,27] / 3"), TEXT("X=3.000000 Y=6.000000 Z=9.000000")).ToString());

	OutBeautifiedNames.Add("Vector3DivisionFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10.5,21,31.5] / 2.5"), TEXT("X=4.200000 Y=8.400000 Z=12.600000")).ToString());

	OutBeautifiedNames.Add("Vector3DivisionNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[9,18,27] / -3"), TEXT("X=-3.000000 Y=-6.000000 Z=-9.000000")).ToString());

	OutBeautifiedNames.Add("Vector3DivisionByOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,6,7] / 1"), TEXT("X=5.000000 Y=6.000000 Z=7.000000")).ToString());

	// Complex Vector2 expressions
	OutBeautifiedNames.Add("Vector2ComplexExpression1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("([1,2] + [3,4]) * 2"), TEXT("X=8.000000 Y=12.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ComplexExpression2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("([10,20] - [2,4]) / 2"), TEXT("X=4.000000 Y=8.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ComplexExpression3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] * 3 + [2,3]"), TEXT("X=5.000000 Y=9.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ComplexExpression4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * [1,2] - [1,1]"), TEXT("X=1.000000 Y=3.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ComplexExpression5");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("([1,2] + [3,4]) * 2 - [2,2]"), TEXT("X=6.000000 Y=10.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ComplexExpression6");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20] / 2 + [5,5]"), TEXT("X=10.000000 Y=15.000000")).ToString());

	// Complex Vector3 expressions
	OutBeautifiedNames.Add("Vector3ComplexExpression1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 * ([1,2,3] + [4,5,6])"), TEXT("X=10.000000 Y=14.000000 Z=18.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ComplexExpression2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("([10,20,30] - [2,4,6]) / 2"), TEXT("X=4.000000 Y=8.000000 Z=12.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ComplexExpression3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] * 3 + [2,3,4]"), TEXT("X=5.000000 Y=9.000000 Z=13.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ComplexExpression4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("3 * [1,2,3] - [1,1,1]"), TEXT("X=2.000000 Y=5.000000 Z=8.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ComplexExpression5");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("([1,2,3] + [4,5,6]) * 2 - [2,2,2]"), TEXT("X=8.000000 Y=12.000000 Z=16.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ComplexExpression6");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[9,18,27] / 3 + [1,2,3]"), TEXT("X=4.000000 Y=8.000000 Z=12.000000")).ToString());

	// Edge case tests
	OutBeautifiedNames.Add("Vector2VeryLargeValues");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[999999,999999] + [1,1]"), TEXT("X=1000000.000000 Y=1000000.000000")).ToString());

	OutBeautifiedNames.Add("Vector3VeryLargeValues");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[999999,999999,999999] + [1,1,1]"), TEXT("X=1000000.000000 Y=1000000.000000 Z=1000000.000000")).ToString());

	OutBeautifiedNames.Add("Vector2VerySmallValues");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0.0001,0.0002] + [0.0003,0.0004]"), TEXT("X=0.000400 Y=0.000600")).ToString());

	OutBeautifiedNames.Add("Vector3VerySmallValues");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[0.0001,0.0002,0.0003] + [0.0004,0.0005,0.0006]"), TEXT("X=0.000500 Y=0.000700 Z=0.000900")).ToString());

	OutBeautifiedNames.Add("Vector2MixedPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] + [3,4] * 2"), TEXT("X=7.000000 Y=10.000000")).ToString());

	OutBeautifiedNames.Add("Vector3MixedPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] + [4,5,6] * 2"), TEXT("X=9.000000 Y=12.000000 Z=15.000000")).ToString());

	OutBeautifiedNames.Add("Vector2NegativeVector");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-[1,2]"), TEXT("X=-1.000000 Y=-2.000000")).ToString());

	OutBeautifiedNames.Add("Vector3NegativeVector");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-[1,2,3]"), TEXT("X=-1.000000 Y=-2.000000 Z=-3.000000")).ToString());

	OutBeautifiedNames.Add("Vector2DoubleNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-[-1,-2]"), TEXT("X=1.000000 Y=2.000000")).ToString());

	OutBeautifiedNames.Add("Vector3DoubleNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-[-1,-2,-3]"), TEXT("X=1.000000 Y=2.000000 Z=3.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ChainedAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] + [3,4] + [5,6]"), TEXT("X=9.000000 Y=12.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ChainedAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] + [4,5,6] + [7,8,9]"), TEXT("X=12.000000 Y=15.000000 Z=18.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ChainedSubtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20] - [3,4] - [2,3]"), TEXT("X=5.000000 Y=13.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ChainedSubtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[20,30,40] - [5,6,7] - [3,4,5]"), TEXT("X=12.000000 Y=20.000000 Z=28.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ChainedMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] * 2 * 3"), TEXT("X=6.000000 Y=12.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ChainedMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] * 2 * 3"), TEXT("X=6.000000 Y=12.000000 Z=18.000000")).ToString());

	OutBeautifiedNames.Add("Vector2ChainedDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[24,48] / 2 / 3"), TEXT("X=4.000000 Y=8.000000")).ToString());

	OutBeautifiedNames.Add("Vector3ChainedDivision");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[36,72,108] / 2 / 3"), TEXT("X=6.000000 Y=12.000000 Z=18.000000")).ToString());
}

bool ShidenExpressionEvaluatorVectorTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	const FString ResultStr = Result.ToString();
	if (ResultStr != Params.ExpectedResult)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluated to '%s', expected '%s'"),
		                         *Params.Expression, *ResultStr, *Params.ExpectedResult));
		return false;
	}

	return true;
}

// Math function tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorMathFunctionTest, "ShidenExpressionEvaluator.MathFunction",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorMathFunctionTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Abs tests
	OutBeautifiedNames.Add("Abs_Positive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Abs_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(-5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Abs_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Abs_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(-3.5)"), TEXT("3.5")).ToString());

	OutBeautifiedNames.Add("Abs_LargeNumber");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(-10000)"), TEXT("10000")).ToString());

	// Sqrt tests
	OutBeautifiedNames.Add("Sqrt_Perfect");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(16)"), TEXT("4")).ToString());

	OutBeautifiedNames.Add("Sqrt_NonPerfect");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(2)"), TEXT("1.414214")).ToString());

	OutBeautifiedNames.Add("Sqrt_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Sqrt_One");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(1)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Sqrt_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(100)"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("Sqrt_Decimal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(0.25)"), TEXT("0.5")).ToString());

	// Pow tests
	OutBeautifiedNames.Add("Pow_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(2, 3)"), TEXT("8")).ToString());

	OutBeautifiedNames.Add("Pow_ZeroExponent");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(5, 0)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Pow_OneExponent");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(5, 1)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Pow_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(2, -1)"), TEXT("0.5")).ToString());

	OutBeautifiedNames.Add("Pow_Fractional");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(4, 0.5)"), TEXT("2")).ToString());

	OutBeautifiedNames.Add("Pow_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(10, 3)"), TEXT("1000")).ToString());

	// Min tests
	OutBeautifiedNames.Add("Min_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("min(5, 3)"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("Min_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("min(5, 5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Min_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("min(-5, -3)"), TEXT("-5")).ToString());

	OutBeautifiedNames.Add("Min_Mixed");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("min(-5, 3)"), TEXT("-5")).ToString());

	OutBeautifiedNames.Add("Min_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("min(3.5, 3.2)"), TEXT("3.2")).ToString());

	OutBeautifiedNames.Add("Min_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("min(0, 5)"), TEXT("0")).ToString());

	// Max tests
	OutBeautifiedNames.Add("Max_Basic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(5, 3)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Max_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(5, 5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Max_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(-5, -3)"), TEXT("-3")).ToString());

	OutBeautifiedNames.Add("Max_Mixed");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(-5, 3)"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("Max_Float");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(3.5, 3.2)"), TEXT("3.5")).ToString());

	OutBeautifiedNames.Add("Max_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(0, 5)"), TEXT("5")).ToString());

	// Floor tests
	OutBeautifiedNames.Add("Floor_Positive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(3.7)"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("Floor_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(-3.2)"), TEXT("-4")).ToString());

	OutBeautifiedNames.Add("Floor_Integer");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Floor_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(0.5)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Floor_NearInteger");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(3.99)"), TEXT("3")).ToString());

	// Ceil tests
	OutBeautifiedNames.Add("Ceil_Positive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("ceil(3.2)"), TEXT("4")).ToString());

	OutBeautifiedNames.Add("Ceil_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("ceil(-3.7)"), TEXT("-3")).ToString());

	OutBeautifiedNames.Add("Ceil_Integer");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("ceil(5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Ceil_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("ceil(0.1)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Ceil_NearInteger");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("ceil(3.01)"), TEXT("4")).ToString());

	// Round tests
	OutBeautifiedNames.Add("Round_Up");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("round(3.5)"), TEXT("4")).ToString());

	OutBeautifiedNames.Add("Round_Down");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("round(3.4)"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("Round_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("round(-3.5)"), TEXT("-3")).ToString());

	OutBeautifiedNames.Add("Round_Integer");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("round(5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Round_Exact");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("round(2.5)"), TEXT("3")).ToString());

	// Clamp tests
	OutBeautifiedNames.Add("Clamp_Above");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("clamp(15, 0, 10)"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("Clamp_Below");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("clamp(-5, 0, 10)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Clamp_Within");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("clamp(5, 0, 10)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Clamp_AtMin");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("clamp(0, 0, 10)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Clamp_AtMax");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("clamp(10, 0, 10)"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("Clamp_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("clamp(-10, -5, 5)"), TEXT("-5")).ToString());

	// Combined function tests
	OutBeautifiedNames.Add("CombinedFunctions1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(min(-5, -3))"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("CombinedFunctions2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(sqrt(10))"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("CombinedFunctions3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(abs(-5), abs(-3))"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("CombinedFunctions4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("round(pow(2, 3.5))"), TEXT("11")).ToString());
}

bool ShidenExpressionEvaluatorMathFunctionTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	float ResultValue, ExpectedValue;
	if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
		FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
	{
		if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
			                         *Params.Expression, ResultValue, ExpectedValue));
			return false;
		}
	}

	return true;
}

// String comparison tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorStringComparisonTest, "ShidenExpressionEvaluator.StringComparison",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorStringComparisonTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Equality tests
	OutBeautifiedNames.Add("StringEqual_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"test\" == \"test\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringEqual_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"test\" == \"other\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringEqual_CaseSensitive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Test\" == \"test\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringEqual_EmptyBoth");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" == \"\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringEqual_EmptyVsNonEmpty");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" == \"a\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringEqual_Long");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"this is a long string\" == \"this is a long string\""), TEXT("true")).ToString());

	// Not equal tests
	OutBeautifiedNames.Add("StringNotEqual_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"test\" != \"other\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringNotEqual_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"test\" != \"test\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringNotEqual_CaseSensitive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Test\" != \"test\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringNotEqual_EmptyVsNonEmpty");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" != \"a\""), TEXT("true")).ToString());

	// Greater than tests
	OutBeautifiedNames.Add("StringGreaterThan_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"b\" > \"a\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringGreaterThan_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" > \"b\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringGreaterThan_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" > \"a\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringGreaterThan_Uppercase");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Z\" > \"a\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringGreaterThan_Digits");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"9\" > \"1\""), TEXT("true")).ToString());

	// Less than tests
	OutBeautifiedNames.Add("StringLessThan_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" < \"b\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringLessThan_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"b\" < \"a\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringLessThan_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" < \"a\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringLessThan_Length");
	// ReSharper disable once StringLiteralTypo
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"abc\" < \"abcd\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringLessThan_Prefix");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"test\" < \"testing\""), TEXT("true")).ToString());

	// Greater or equal tests
	OutBeautifiedNames.Add("StringGreaterOrEqual_Greater");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"b\" >= \"a\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringGreaterOrEqual_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"b\" >= \"b\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringGreaterOrEqual_Less");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" >= \"b\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringGreaterOrEqual_Empty");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" >= \"\""), TEXT("true")).ToString());

	// Less or equal tests
	OutBeautifiedNames.Add("StringLessOrEqual_Less");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" <= \"b\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringLessOrEqual_Equal");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" <= \"a\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringLessOrEqual_Greater");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"b\" <= \"a\""), TEXT("false")).ToString());

	OutBeautifiedNames.Add("StringLessOrEqual_Empty");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\" <= \"a\""), TEXT("true")).ToString());

	// Special character tests
	OutBeautifiedNames.Add("StringWithSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"hello world\" == \"hello world\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringWithNumbers");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"test123\" == \"test123\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringWithSpecialChars");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"!@#$%\" == \"!@#$%\""), TEXT("true")).ToString());

	// Combined comparison tests
	OutBeautifiedNames.Add("StringComparisonAnd");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" < \"b\" && \"c\" > \"b\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringComparisonOr");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" == \"b\" || \"a\" == \"a\""), TEXT("true")).ToString());

	OutBeautifiedNames.Add("StringComparisonNot");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!(\"a\" == \"b\")"), TEXT("true")).ToString());
}

bool ShidenExpressionEvaluatorStringComparisonTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	bool bResultBool;
	if (!Result.TryToBoolean(bResultBool, ErrorMessage))
	{
		AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
		                         *Params.Expression, *ErrorMessage));
		return false;
	}

	const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
	if (bResultBool != bExpected)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
		                         *Params.Expression,
		                         bResultBool ? TEXT("true") : TEXT("false"),
		                         bExpected ? TEXT("true") : TEXT("false")));
		return false;
	}

	return true;
}

// Boolean comparison tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorBooleanComparisonTest, "ShidenExpressionEvaluator.BooleanComparison",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorBooleanComparisonTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Boolean equality tests
	OutBeautifiedNames.Add("BooleanEqual_TrueTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true == true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanEqual_FalseFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false == false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanEqual_TrueFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true == false"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("BooleanEqual_FalseTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false == true"), TEXT("false")).ToString());

	// Boolean not equal tests
	OutBeautifiedNames.Add("BooleanNotEqual_TrueFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true != false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanNotEqual_FalseTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false != true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanNotEqual_TrueTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true != true"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("BooleanNotEqual_FalseFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false != false"), TEXT("false")).ToString());

	// Boolean AND combinations
	OutBeautifiedNames.Add("BooleanAnd_TrueAndComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && (5 > 3)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanAnd_FalseAndComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false && (5 > 3)"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("BooleanAnd_ComparisonAndTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3) && true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanAnd_ComparisonAndFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3) && false"), TEXT("false")).ToString());

	// Boolean OR combinations
	OutBeautifiedNames.Add("BooleanOr_TrueOrComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true || (5 < 3)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanOr_FalseOrComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("false || (5 > 3)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanOr_ComparisonOrTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 < 3) || true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanOr_ComparisonOrFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 < 3) || false"), TEXT("false")).ToString());

	// Boolean NOT combinations
	OutBeautifiedNames.Add("BooleanNot_NotTrueEquality");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!true == false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanNot_NotFalseEquality");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!false == true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanNot_DoubleNegationTrue");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!!true == true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BooleanNot_DoubleNegationFalse");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!!false == false"), TEXT("true")).ToString());

	// Complex boolean expressions
	OutBeautifiedNames.Add("ComplexBoolean_OrAndPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(true || false) && true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_AndOrPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && false || true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_NotOr");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!(false || false)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_NotAnd");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!(true && false)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_MultipleAnd");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(true && false) || (false && true)"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_MultipleOr");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(false || false) && (true || false)"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_Nested1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("true && (false || (true && true))"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_Nested2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!(true && false) && (false || true)"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_WithComparisons1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3) == true"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_WithComparisons2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 < 3) == false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexBoolean_MixedOperators");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(true || false) && !(false && true) || false"), TEXT("true")).ToString());
}

bool ShidenExpressionEvaluatorBooleanComparisonTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	bool bResultBool;
	if (!Result.TryToBoolean(bResultBool, ErrorMessage))
	{
		AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
		                         *Params.Expression, *ErrorMessage));
		return false;
	}

	const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
	if (bResultBool != bExpected)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
		                         *Params.Expression,
		                         bResultBool ? TEXT("true") : TEXT("false"),
		                         bExpected ? TEXT("true") : TEXT("false")));
		return false;
	}

	return true;
}

// Vector comparison tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorVectorComparisonTest, "ShidenExpressionEvaluator.VectorComparison",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorVectorComparisonTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("Vector2Equal_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] == [1,2]"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Vector2Equal_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] == [3,4]"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("Vector2NotEqual_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] != [3,4]"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Vector3Equal_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] == [1,2,3]"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Vector3Equal_False");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] == [4,5,6]"), TEXT("false")).ToString());

	OutBeautifiedNames.Add("Vector3NotEqual_True");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3] != [4,5,6]"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("Vector2Subtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[5,7] - [2,3]"), TEXT("X=3.000000 Y=4.000000")).ToString());

	OutBeautifiedNames.Add("Vector3Subtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20,30] - [5,10,15]"), TEXT("X=5.000000 Y=10.000000 Z=15.000000")).ToString());

	OutBeautifiedNames.Add("Vector2Division");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[10,20] / 2"), TEXT("X=5.000000 Y=10.000000")).ToString());

	OutBeautifiedNames.Add("Vector3Division");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[9,18,27] / 3"), TEXT("X=3.000000 Y=6.000000 Z=9.000000")).ToString());
}

bool ShidenExpressionEvaluatorVectorComparisonTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	if (Result.Type == EShidenExpressionValueType::Boolean)
	{
		bool bResultBool;
		if (!Result.TryToBoolean(bResultBool, ErrorMessage))
		{
			AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
			                         *Params.Expression, *ErrorMessage));
			return false;
		}

		const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
		if (bResultBool != bExpected)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
			                         *Params.Expression,
			                         bResultBool ? TEXT("true") : TEXT("false"),
			                         bExpected ? TEXT("true") : TEXT("false")));
			return false;
		}
	}
	else
	{
		const FString ResultStr = Result.ToString();
		if (ResultStr != Params.ExpectedResult)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to '%s', expected '%s'"),
			                         *Params.Expression, *ResultStr, *Params.ExpectedResult));
			return false;
		}
	}

	return true;
}

// Unary operator tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorUnaryOperatorTest, "ShidenExpressionEvaluator.UnaryOperator",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorUnaryOperatorTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("UnaryMinus_Simple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-5"), TEXT("-5")).ToString());

	OutBeautifiedNames.Add("UnaryMinus_Expression");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-(3 + 2)"), TEXT("-5")).ToString());

	OutBeautifiedNames.Add("UnaryPlus_Simple");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("+5"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("UnaryPlus_Expression");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("+(3 + 2)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("DoubleNegative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("--5"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("NegativeMultiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-3 * 4"), TEXT("-12")).ToString());

	OutBeautifiedNames.Add("NegativeAddition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-3 + 5"), TEXT("2")).ToString());

	OutBeautifiedNames.Add("LogicalNot_DoubleNegation");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("!!true"), TEXT("true")).ToString());
}

bool ShidenExpressionEvaluatorUnaryOperatorTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	if (Result.Type == EShidenExpressionValueType::Boolean)
	{
		bool bResultBool;
		if (!Result.TryToBoolean(bResultBool, ErrorMessage))
		{
			AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
			                         *Params.Expression, *ErrorMessage));
			return false;
		}

		const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
		if (bResultBool != bExpected)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
			                         *Params.Expression,
			                         bResultBool ? TEXT("true") : TEXT("false"),
			                         bExpected ? TEXT("true") : TEXT("false")));
			return false;
		}
	}
	else
	{
		float ResultValue, ExpectedValue;
		if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
			FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
		{
			if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
			{
				AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
				                         *Params.Expression, ResultValue, ExpectedValue));
				return false;
			}
		}
	}

	return true;
}

// Nested function call tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorNestedFunctionTest, "ShidenExpressionEvaluator.NestedFunction",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorNestedFunctionTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("NestedAbs");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(abs(-5))"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("NestedMinMax");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("max(min(10, 5), 3)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("NestedSqrtPow");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(pow(4, 2))"), TEXT("4")).ToString());

	OutBeautifiedNames.Add("ComplexNested");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("floor(abs(min(-10.5, -5.3)))"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("NestedWithArithmetic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(-5) + max(3, 7)"), TEXT("12")).ToString());

	OutBeautifiedNames.Add("TripleNested");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(min(max(1, 2), 3))"), TEXT("2")).ToString());
}

bool ShidenExpressionEvaluatorNestedFunctionTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	float ResultValue, ExpectedValue;
	if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
		FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
	{
		if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
			                         *Params.Expression, ResultValue, ExpectedValue));
			return false;
		}
	}

	return true;
}

// Trigonometric function tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorTrigonometricTest, "ShidenExpressionEvaluator.Trigonometric",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorTrigonometricTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Sin tests
	OutBeautifiedNames.Add("Sin_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Sin_Pi");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(3.14159)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Sin_PiOver2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(1.5708)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Sin_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(-1.5708)"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("Sin_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(0.5)"), TEXT("0.479426")).ToString());

	// Cos tests
	OutBeautifiedNames.Add("Cos_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("cos(0)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Cos_Pi");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("cos(3.14159)"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("Cos_PiOver2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("cos(1.5708)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Cos_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("cos(-1.5708)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Cos_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("cos(0.5)"), TEXT("0.877583")).ToString());

	// Tan tests
	OutBeautifiedNames.Add("Tan_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("tan(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Tan_PiOver4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("tan(0.7854)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Tan_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("tan(-0.7854)"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("Tan_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("tan(0.5)"), TEXT("0.546302")).ToString());

	// Asin tests
	OutBeautifiedNames.Add("Asin_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("asin(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Asin_One");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("asin(1)"), TEXT("1.5708")).ToString());

	OutBeautifiedNames.Add("Asin_MinusOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("asin(-1)"), TEXT("-1.5708")).ToString());

	OutBeautifiedNames.Add("Asin_Half");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("asin(0.5)"), TEXT("0.523599")).ToString());

	// Acos tests
	OutBeautifiedNames.Add("Acos_One");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("acos(1)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Acos_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("acos(0)"), TEXT("1.5708")).ToString());

	OutBeautifiedNames.Add("Acos_MinusOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("acos(-1)"), TEXT("3.14159")).ToString());

	OutBeautifiedNames.Add("Acos_Half");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("acos(0.5)"), TEXT("1.0472")).ToString());

	// Atan tests
	OutBeautifiedNames.Add("Atan_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Atan_One");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan(1)"), TEXT("0.785398")).ToString());

	OutBeautifiedNames.Add("Atan_MinusOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan(-1)"), TEXT("-0.785398")).ToString());

	OutBeautifiedNames.Add("Atan_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan(100)"), TEXT("1.5608")).ToString());

	// Atan2 tests
	OutBeautifiedNames.Add("Atan2_ZeroOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan2(0, 1)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Atan2_OneZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan2(1, 0)"), TEXT("1.5708")).ToString());

	OutBeautifiedNames.Add("Atan2_OneOne");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan2(1, 1)"), TEXT("0.785398")).ToString());

	OutBeautifiedNames.Add("Atan2_NegativeY");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan2(-1, 1)"), TEXT("-0.785398")).ToString());

	OutBeautifiedNames.Add("Atan2_NegativeX");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("atan2(1, -1)"), TEXT("2.35619")).ToString());

	// Combined trig functions
	OutBeautifiedNames.Add("TrigCombined1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(asin(0.5))"), TEXT("0.5")).ToString());

	OutBeautifiedNames.Add("TrigCombined2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("cos(acos(0.5))"), TEXT("0.5")).ToString());

	OutBeautifiedNames.Add("TrigCombined3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("tan(atan(1))"), TEXT("1")).ToString());
}

bool ShidenExpressionEvaluatorTrigonometricTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	float ResultValue, ExpectedValue;
	if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
		FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
	{
		if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.01))
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
			                         *Params.Expression, ResultValue, ExpectedValue));
			return false;
		}
	}

	return true;
}

// Complex expression tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorComplexExpressionTest, "ShidenExpressionEvaluator.ComplexExpression",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorComplexExpressionTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("ArithmeticAndComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 + 3) * 2 > 10"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("ComplexLogicalExpression");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 > 3) && (10 < 20) || false"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("MixedOperations");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(-10) + max(5, 3) * 2 - min(2, 1)"), TEXT("19")).ToString());

	OutBeautifiedNames.Add("StringAndNumberComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"a\" < \"b\" && 5 > 3"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("VectorAndScalarMixed");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("([1,2] + [3,4]) * 2 == [8,12]"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("BitwiseAndArithmetic");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 & 3) + (10 | 2)"), TEXT("11")).ToString());

	OutBeautifiedNames.Add("PowerAndRoot");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(pow(3, 2) + pow(4, 2))"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("ComplexPrecedence");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3 * 4 ** 2 / 8 - 1"), TEXT("7")).ToString());
}

bool ShidenExpressionEvaluatorComplexExpressionTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	if (Result.Type == EShidenExpressionValueType::Boolean)
	{
		bool bResultBool;
		if (!Result.TryToBoolean(bResultBool, ErrorMessage))
		{
			AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
			                         *Params.Expression, *ErrorMessage));
			return false;
		}

		const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
		if (bResultBool != bExpected)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
			                         *Params.Expression,
			                         bResultBool ? TEXT("true") : TEXT("false"),
			                         bExpected ? TEXT("true") : TEXT("false")));
			return false;
		}
	}
	else
	{
		float ResultValue, ExpectedValue;
		if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
			FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
		{
			if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
			{
				AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
				                         *Params.Expression, ResultValue, ExpectedValue));
				return false;
			}
		}
	}

	return true;
}

// Edge case and error tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorEdgeCaseTest, "ShidenExpressionEvaluator.EdgeCase",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorEdgeCaseTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("DivisionByZero_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 / 0"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("SqrtNegative_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(-1)"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("LogNegative_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(-1)"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("LogZero_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(0)"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("AsinOutOfRange");
	// Asin input must be in the range [-1, 1], asin(2) is treated as asin(1)
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("asin(2)"), TEXT("1.570796")).ToString());

	OutBeautifiedNames.Add("AcosOutOfRange");
	// Acos input must be in the range [-1, 1], acos(2) is treated as acos(1)
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("acos(2)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("VeryLargeNumber");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("999999999 + 1"), TEXT("1000000000")).ToString());

	OutBeautifiedNames.Add("VerySmallNumber");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0.0001 + 0.0001"), TEXT("0.0002")).ToString());

	OutBeautifiedNames.Add("ZeroComparison");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("0 == 0.0"), TEXT("true")).ToString());

	OutBeautifiedNames.Add("NegativeZero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-0"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("EmptyParentheses_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("()"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("MismatchedParentheses_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(5 + 3"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("InvalidVectorDimension_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1]"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("InvalidVectorDimension4_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2,3,4]"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("MixedVector2And3_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("[1,2] + [1,2,3]"), TEXT(""), false).ToString());

	// Shift operation boundary tests
	OutBeautifiedNames.Add("LeftShiftNegativeAmount_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 << -1"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("RightShiftNegativeAmount_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 >> -1"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("LeftShiftOverBitWidth_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 << 32"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("RightShiftOverBitWidth_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5 >> 32"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("LeftShiftLargeAmount_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 << 100"), TEXT(""), false).ToString());

	OutBeautifiedNames.Add("RightShiftLargeAmount_ShouldFail");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 >> 100"), TEXT(""), false).ToString());

	// Edge cases: shift by 31 (maximum valid shift for int32) should succeed
	OutBeautifiedNames.Add("LeftShift_MaxValidAmount");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("1 << 31"), TEXT("-2147483648")).ToString());

	OutBeautifiedNames.Add("RightShift_MaxValidAmount");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("-2147483648 >> 31"), TEXT("-1")).ToString());
}

bool ShidenExpressionEvaluatorEdgeCaseTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);

	if (bSuccess != Params.bShouldSucceed)
	{
		AddError(FString::Printf(TEXT("Expression '%s' evaluation returned %s, expected %s. Error: %s"),
		                         *Params.Expression,
		                         bSuccess ? TEXT("success") : TEXT("failure"),
		                         Params.bShouldSucceed ? TEXT("success") : TEXT("failure"),
		                         *ErrorMessage));
		return false;
	}

	if (bSuccess && !Params.ExpectedResult.IsEmpty())
	{
		float ResultValue, ExpectedValue;
		if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
			FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
		{
			if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.0001))
			{
				AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
				                         *Params.Expression, ResultValue, ExpectedValue));
				return false;
			}
		}
		else if (Result.Type == EShidenExpressionValueType::Boolean)
		{
			bool bResultBool;
			if (!Result.TryToBoolean(bResultBool, ErrorMessage))
			{
				AddError(FString::Printf(TEXT("Expression '%s' could not convert to boolean: %s"),
				                         *Params.Expression, *ErrorMessage));
				return false;
			}

			const bool bExpected = Params.ExpectedResult.Equals(TEXT("true"), ESearchCase::IgnoreCase);
			if (bResultBool != bExpected)
			{
				AddError(FString::Printf(TEXT("Expression '%s' evaluated to %s, expected %s"),
				                         *Params.Expression,
				                         bResultBool ? TEXT("true") : TEXT("false"),
				                         bExpected ? TEXT("true") : TEXT("false")));
				return false;
			}
		}
	}

	return true;
}

// Additional math function tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorAdditionalMathTest, "ShidenExpressionEvaluator.AdditionalMath",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorAdditionalMathTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Exp tests
	OutBeautifiedNames.Add("Exp_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("exp(0)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Exp_One");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("exp(1)"), TEXT("2.71828")).ToString());

	OutBeautifiedNames.Add("Exp_Two");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("exp(2)"), TEXT("7.38906")).ToString());

	OutBeautifiedNames.Add("Exp_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("exp(-1)"), TEXT("0.367879")).ToString());

	OutBeautifiedNames.Add("Exp_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("exp(0.5)"), TEXT("1.64872")).ToString());

	// Log tests
	OutBeautifiedNames.Add("Log_e");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(2.71828)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Log_One");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(1)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Log_Ten");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(10)"), TEXT("2.30259")).ToString());

	OutBeautifiedNames.Add("Log_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(100)"), TEXT("4.60517")).ToString());

	OutBeautifiedNames.Add("Log_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("log(0.5)"), TEXT("-0.693147")).ToString());

	// Sign tests
	OutBeautifiedNames.Add("Sign_Positive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(5)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Sign_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(-5)"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("Sign_Zero");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Sign_PositiveFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(3.5)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("Sign_NegativeFloat");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(-3.5)"), TEXT("-1")).ToString());

	OutBeautifiedNames.Add("Sign_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(0.001)"), TEXT("1")).ToString());

	// Frac tests
	OutBeautifiedNames.Add("Frac_Positive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("frac(3.7)"), TEXT("0.7")).ToString());

	OutBeautifiedNames.Add("Frac_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("frac(-3.7)"), TEXT("0.3")).ToString());

	OutBeautifiedNames.Add("Frac_Integer");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("frac(5)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Frac_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("frac(0.123)"), TEXT("0.123")).ToString());

	OutBeautifiedNames.Add("Frac_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("frac(100.999)"), TEXT("0.999")).ToString());

	// Trunc tests
	OutBeautifiedNames.Add("Trunc_Positive");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("trunc(3.7)"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("Trunc_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("trunc(-3.7)"), TEXT("-3")).ToString());

	OutBeautifiedNames.Add("Trunc_Integer");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("trunc(5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Trunc_Small");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("trunc(0.999)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Trunc_Large");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("trunc(999.1)"), TEXT("999")).ToString());

	// Lerp tests
	OutBeautifiedNames.Add("Lerp_Middle");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("lerp(0, 10, 0.5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("Lerp_Start");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("lerp(0, 10, 0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Lerp_End");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("lerp(0, 10, 1)"), TEXT("10")).ToString());

	OutBeautifiedNames.Add("Lerp_Quarter");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("lerp(0, 10, 0.25)"), TEXT("2.5")).ToString());

	OutBeautifiedNames.Add("Lerp_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("lerp(-10, 10, 0.5)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("Lerp_Extrapolate");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("lerp(0, 10, 2)"), TEXT("20")).ToString());

	// InvLerp tests
	OutBeautifiedNames.Add("InvLerp_Middle");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("invlerp(0, 10, 5)"), TEXT("0.5")).ToString());

	OutBeautifiedNames.Add("InvLerp_Start");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("invlerp(0, 10, 0)"), TEXT("0")).ToString());

	OutBeautifiedNames.Add("InvLerp_End");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("invlerp(0, 10, 10)"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("InvLerp_Quarter");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("invlerp(0, 10, 2.5)"), TEXT("0.25")).ToString());

	OutBeautifiedNames.Add("InvLerp_Negative");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("invlerp(-10, 10, 0)"), TEXT("0.5")).ToString());

	OutBeautifiedNames.Add("InvLerp_Outside");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("invlerp(0, 10, 20)"), TEXT("2")).ToString());

	// Combined additional math functions
	OutBeautifiedNames.Add("CombinedAdditional1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sign(frac(3.7))"), TEXT("1")).ToString());

	OutBeautifiedNames.Add("CombinedAdditional2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("trunc(lerp(0, 100, 0.756))"), TEXT("75")).ToString());

	OutBeautifiedNames.Add("CombinedAdditional3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("exp(log(5))"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("CombinedAdditional4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(sign(-5))"), TEXT("1")).ToString());
}

bool ShidenExpressionEvaluatorAdditionalMathTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	float ResultValue, ExpectedValue;
	if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
		FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
	{
		if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.01))
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
			                         *Params.Expression, ResultValue, ExpectedValue));
			return false;
		}
	}

	return true;
}

// Unformatted string tests (whitespace handling)
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorUnformattedStringTest, "ShidenExpressionEvaluator.UnformattedString",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorUnformattedStringTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// No spaces around operators
	OutBeautifiedNames.Add("NoSpaces_Addition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2+3"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("NoSpaces_Subtraction");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("5-2"), TEXT("3")).ToString());

	OutBeautifiedNames.Add("NoSpaces_Multiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2*3"), TEXT("6")).ToString());

	OutBeautifiedNames.Add("NoSpaces_Division");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("10/2"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("NoSpaces_ComplexExpression");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2+3*4-1"), TEXT("13")).ToString());

	// Extra spaces around operators
	OutBeautifiedNames.Add("ExtraSpaces_Addition");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2  +  3"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("ExtraSpaces_Multiplication");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2   *   3"), TEXT("6")).ToString());

	OutBeautifiedNames.Add("ExtraSpaces_ComplexExpression");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2  +  3  *  4  -  1"), TEXT("13")).ToString());

	// Leading and trailing spaces
	OutBeautifiedNames.Add("LeadingSpace");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT(" 2 + 3"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("TrailingSpace");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 + 3 "), TEXT("5")).ToString());

	OutBeautifiedNames.Add("BothLeadingAndTrailing");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("  2 + 3  "), TEXT("5")).ToString());

	OutBeautifiedNames.Add("MultipleLeadingTrailing");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("    2 + 3    "), TEXT("5")).ToString());

	// Spaces around parentheses
	OutBeautifiedNames.Add("NoSpaceBeforeParenthesis");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(2+3)*4"), TEXT("20")).ToString());

	OutBeautifiedNames.Add("SpaceBeforeParenthesis");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT(" ( 2 + 3 ) * 4"), TEXT("20")).ToString());

	OutBeautifiedNames.Add("ExtraSpacesAroundParentheses");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(  2  +  3  )  *  4"), TEXT("20")).ToString());

	OutBeautifiedNames.Add("NestedParenthesesWithSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("( ( 2 + 3 ) * 4 )"), TEXT("20")).ToString());

	// Spaces in function calls
	OutBeautifiedNames.Add("FunctionNoSpace");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(-5)"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("FunctionWithSpace");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs( -5 )"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("FunctionWithExtraSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("abs(  -5  )"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("FunctionMultipleArgsNoSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(2,3)"), TEXT("8")).ToString());

	OutBeautifiedNames.Add("FunctionMultipleArgsWithSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow( 2 , 3 )"), TEXT("8")).ToString());

	OutBeautifiedNames.Add("FunctionMultipleArgsExtraSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("pow(  2  ,  3  )"), TEXT("8")).ToString());

	// Mixed formatting
	OutBeautifiedNames.Add("MixedFormatting1");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("  2+3  *  4"), TEXT("14")).ToString());

	OutBeautifiedNames.Add("MixedFormatting2");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("(2+3)*4-1  "), TEXT("19")).ToString());

	OutBeautifiedNames.Add("MixedFormatting3");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("  abs(  -5  )+pow(2,3)  "), TEXT("13")).ToString());

	OutBeautifiedNames.Add("MixedFormatting4");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2  +3*  4-(  5  /2)"), TEXT("11.5")).ToString());

	// Tabs and mixed whitespace (if supported)
	OutBeautifiedNames.Add("TabsInsteadOfSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2\t+\t3"), TEXT("5")).ToString());

	OutBeautifiedNames.Add("MixedTabsAndSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("2 \t+ \t3"), TEXT("5")).ToString());

	// Unformatted complex expressions
	OutBeautifiedNames.Add("ComplexNoSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sin(3.14159/2)+cos(0)"), TEXT("2")).ToString());

	OutBeautifiedNames.Add("ComplexExtraSpaces");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("  sin(  3.14159  /  2  )  +  cos(  0  )  "), TEXT("2")).ToString());

	OutBeautifiedNames.Add("ComplexMixedFormatting");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("sqrt(16)+abs(-4)  *2"), TEXT("12")).ToString());
}

bool ShidenExpressionEvaluatorUnformattedStringTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	float ResultValue, ExpectedValue;
	if (Result.TryToNumeric(ResultValue, ErrorMessage) &&
		FDefaultValueHelper::ParseFloat(Params.ExpectedResult, ExpectedValue))
	{
		if (!FMath::IsNearlyEqual(ResultValue, ExpectedValue, 0.01))
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to %f, expected %f"),
			                         *Params.Expression, ResultValue, ExpectedValue));
			return false;
		}
	}

	return true;
}

// String escape sequence tests
IMPLEMENT_COMPLEX_AUTOMATION_TEST(ShidenExpressionEvaluatorEscapeSequenceTest, "ShidenExpressionEvaluator.EscapeSequence",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ShidenExpressionEvaluatorEscapeSequenceTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// Newline escape sequence
	OutBeautifiedNames.Add("Newline_Escape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\\nWorld\""), TEXT("Hello\nWorld")).ToString());

	OutBeautifiedNames.Add("Multiple_Newlines");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Line1\\nLine2\\nLine3\""), TEXT("Line1\nLine2\nLine3")).ToString());

	// Tab escape sequence
	OutBeautifiedNames.Add("Tab_Escape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\\tWorld\""), TEXT("Hello\tWorld")).ToString());

	OutBeautifiedNames.Add("Multiple_Tabs");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Col1\\tCol2\\tCol3\""), TEXT("Col1\tCol2\tCol3")).ToString());

	// Carriage return escape sequence
	OutBeautifiedNames.Add("CarriageReturn_Escape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Hello\\rWorld\""), TEXT("Hello\rWorld")).ToString());

	OutBeautifiedNames.Add("Multiple_CarriageReturns");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Line1\\rLine2\\rLine3\""), TEXT("Line1\rLine2\rLine3")).ToString());

	// Backslash escape sequence
	OutBeautifiedNames.Add("Backslash_Escape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"C:\\\\Users\\\\Name\""), TEXT("C:\\Users\\Name")).ToString());

	OutBeautifiedNames.Add("Multiple_Backslashes");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\\\\\\\Server\\\\Share\""), TEXT("\\\\Server\\Share")).ToString());

	OutBeautifiedNames.Add("SingleBackslash");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Path\\\\File\""), TEXT("Path\\File")).ToString());

	// Double quote escape sequence
	OutBeautifiedNames.Add("DoubleQuote_Escape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Say \\\"Hello\\\"\""), TEXT("Say \"Hello\"")).ToString());

	OutBeautifiedNames.Add("Multiple_DoubleQuotes");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\\"Quote1\\\" and \\\"Quote2\\\"\""), TEXT("\"Quote1\" and \"Quote2\"")).ToString());

	OutBeautifiedNames.Add("NestedQuotes");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"She said \\\"It's fine\\\"\""), TEXT("She said \"It's fine\"")).ToString());

	// Mixed escape sequences
	OutBeautifiedNames.Add("Mixed_NewlineAndTab");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Line1\\n\\tIndented\""), TEXT("Line1\n\tIndented")).ToString());

	OutBeautifiedNames.Add("Mixed_AllEscapes");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Path: C:\\\\Folder\\n\\tFile: \\\"test.txt\\\"\""), TEXT("Path: C:\\Folder\n\tFile: \"test.txt\"")).ToString());

	OutBeautifiedNames.Add("Mixed_BackslashAndQuote");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Path\\\\\\\"file.txt\\\"\""), TEXT("Path\\\"file.txt\"")).ToString());

	OutBeautifiedNames.Add("Mixed_TabAndCarriageReturn");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"Col1\\tCol2\\rNew\""), TEXT("Col1\tCol2\rNew")).ToString());

	// Edge cases
	OutBeautifiedNames.Add("EmptyString");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\""), TEXT("")).ToString());

	OutBeautifiedNames.Add("OnlyEscapeSequence_Newline");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\n\""), TEXT("\n")).ToString());

	OutBeautifiedNames.Add("OnlyEscapeSequence_Tab");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\t\""), TEXT("\t")).ToString());

	OutBeautifiedNames.Add("OnlyEscapeSequence_Backslash");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\\\\""), TEXT("\\")).ToString());

	OutBeautifiedNames.Add("OnlyEscapeSequence_Quote");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\\"\""), TEXT("\"")).ToString());

	OutBeautifiedNames.Add("StartWithEscape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\nStart with newline\""), TEXT("\nStart with newline")).ToString());

	OutBeautifiedNames.Add("EndWithEscape");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"End with tab\\t\""), TEXT("End with tab\t")).ToString());

	OutBeautifiedNames.Add("ConsecutiveEscapes");
	OutTestCommands.Add(FShidenExpressionTestParameters(TEXT("\"\\n\\n\\n\""), TEXT("\n\n\n")).ToString());
}

bool ShidenExpressionEvaluatorEscapeSequenceTest::RunTest(const FString& Parameters)
{
	const FShidenExpressionTestParameters Params(Parameters);
	FShidenExpressionValue Result;
	FString ErrorMessage;

	const bool bSuccess = FShidenExpressionEvaluator::TryEvaluate(Params.Expression, Result, ErrorMessage);
	if (!bSuccess)
	{
		AddError(FString::Printf(TEXT("Expression '%s' failed: %s"), *Params.Expression, *ErrorMessage));
		return false;
	}

	// For string comparisons
	if (Result.Type == EShidenExpressionValueType::String)
	{
		if (Result.StringValue != Params.ExpectedResult)
		{
			AddError(FString::Printf(TEXT("Expression '%s' evaluated to '%s', expected '%s'"),
			                         *Params.Expression, *Result.StringValue, *Params.ExpectedResult));
			return false;
		}
		return true;
	}

	AddError(FString::Printf(TEXT("Expression '%s' did not evaluate to a string type"), *Params.Expression));
	return false;
}
