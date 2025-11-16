#include "Misc/AutomationTest.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(RegisterScenarioPropertyFromArrayTest, "ShidenScenarioBlueprintLibrary.RegisterScenarioPropertyFromArray", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool RegisterScenarioPropertyFromArrayTest::RunTest(const FString& Parameters)
{
	auto TestArrayToJsonString = [this](const TArray<FString>& TestArray)
	{
		// Arrange
		FString JsonString;
		UShidenScenarioBlueprintLibrary::ClearScenarioProperties(TEXT("TEST"));

		// Act
		UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromArray(TEXT("TEST"), TEXT("Key"), TestArray);
		FShidenScenarioProperty ResultProperty;
		bool bResult = UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(TEXT("TEST"), TEXT("Key"), ResultProperty);

		// Assert
		if (!bResult)
		{
			AddError(TEXT("Failed to convert array to JSON string."));
			return;
		}
		TArray<FString> ResultArray;
		bResult = ResultProperty.TryConvertToStringArray(ResultArray);
		if (!bResult)
		{
			AddError(TEXT("Failed to convert JSON string to array."));
			return;
		}
		if (ResultArray.Num() != TestArray.Num())
		{
			AddError(FString::Printf(TEXT("Expected array size: %d, but got: %d"), TestArray.Num(), ResultArray.Num()));
			return;
		}
		for (int32 i = 0; i < TestArray.Num(); ++i)
		{
			if (ResultArray[i] != TestArray[i])
			{
				AddError(FString::Printf(TEXT("Expected array element at index %d: %s, but got: %s"), i, *TestArray[i], *ResultArray[i]));
			}
		}
	};

	TestArrayToJsonString({TEXT("Hello"), TEXT("World"), TEXT("!")});
	TestArrayToJsonString({TEXT("newLine\nTest"), TEXT("newLine\r\nTest"), TEXT("!")});
	TestArrayToJsonString({});

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(RegisterScenarioPropertyFromMapTest, "ShidenScenarioBlueprintLibrary.RegisterScenarioPropertyFromMap", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool RegisterScenarioPropertyFromMapTest::RunTest(const FString& Parameters)
{
	auto TestMapToJsonString = [this](const TMap<FString, FString>& TestMap)
	{
		// Arrange
		FString JsonString;
		UShidenScenarioBlueprintLibrary::ClearScenarioProperties(TEXT("TEST"));

		// Act
		UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(TEXT("TEST"), TEXT("Key"), TestMap);
		FShidenScenarioProperty ResultProperty;
		bool bResult = UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(TEXT("TEST"), TEXT("Key"), ResultProperty);

		// Assert
		if (!bResult)
		{
			AddError(TEXT("Failed to convert array to JSON string."));
			return;
		}
		TMap<FString, FString> ResultMap;
		bResult = ResultProperty.TryConvertToStringMap(ResultMap);
		if (!bResult)
		{
			AddError(TEXT("Failed to convert JSON string to map."));
			return;
		}
		if (ResultMap.Num() != TestMap.Num())
		{
			AddError(FString::Printf(TEXT("Expected map size: %d, but got: %d"), ResultMap.Num(), TestMap.Num()));
			return;
		}
		for (const TPair<FString, FString>& Pair : TestMap)
		{
			if (!ResultMap.Contains(Pair.Key))
			{
				AddError(FString::Printf(TEXT("Expected map element: %s: %s, but not found in result map."), *Pair.Key, *Pair.Value));
				continue;
			}
			if (ResultMap[Pair.Key] != Pair.Value)
			{
				AddError(FString::Printf(TEXT("Expected map element: %s: %s, but got: %s"), *Pair.Key, *Pair.Value, *ResultMap[Pair.Key]));
			}
		}
	};

	TestMapToJsonString({{TEXT("Arg1"), TEXT("Hello")}, {TEXT("Arg2"), TEXT("World")}, {TEXT("Arg3"), TEXT("!")}});
	TestMapToJsonString({{TEXT("Arg1"), TEXT("newLine\nTest")}, {TEXT("Arg2"), TEXT("newLine\r\nTest")}, {TEXT("Arg3"), TEXT("!")}});
	TestMapToJsonString({});

	return true;
}
