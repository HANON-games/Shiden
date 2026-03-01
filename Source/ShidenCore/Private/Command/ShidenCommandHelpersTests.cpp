// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Misc/Base64.h"
#include "Command/ShidenCommandHelpers.h"

// =============================================================================
// Round-trip Tests (MakeScenarioPropertyKey -> ParseScenarioPropertyKey)
// =============================================================================

struct FScenarioPropertyKeyRoundTripTestParameters
{
	FString TargetType;
	FString TargetName;
	FString ParameterName;

	FString ToString() const
	{
		return FString::Printf(TEXT("%s\t%s\t%s"),
		                       *FBase64::Encode(TargetType),
		                       *FBase64::Encode(TargetName),
		                       *FBase64::Encode(ParameterName));
	}

	FScenarioPropertyKeyRoundTripTestParameters(const FString& InTargetType, const FString& InTargetName,
	                                            const FString& InParameterName)
		: TargetType(InTargetType), TargetName(InTargetName), ParameterName(InParameterName)
	{
	}

	explicit FScenarioPropertyKeyRoundTripTestParameters(const FString& Parameters)
		: TargetType(TEXT("")), TargetName(TEXT("")), ParameterName(TEXT(""))
	{
		TArray<FString> Parts;
		Parameters.ParseIntoArray(Parts, TEXT("\t"), false);
		if (Parts.Num() >= 3)
		{
			FBase64::Decode(Parts[0], TargetType);
			FBase64::Decode(Parts[1], TargetName);
			FBase64::Decode(Parts[2], ParameterName);
		}
	}
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(ScenarioPropertyKeyRoundTripTest, "ShidenMaterialParameterHelpers.RoundTrip",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void ScenarioPropertyKeyRoundTripTest::GetTests(TArray<FString>& OutBeautifiedNames,
                                                TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("NormalValues");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT("Image"), TEXT("MyTarget"), TEXT("MyParam")).ToString());

	OutBeautifiedNames.Add("TargetNameEndingWithColon");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT("Image"), TEXT("A:"), TEXT("B")).ToString());

	OutBeautifiedNames.Add("TargetTypeEndingWithColon");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT("Image:"), TEXT("MyTarget"), TEXT("MyParam")).ToString());

	OutBeautifiedNames.Add("ParameterNameEndingWithColon");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT("Image"), TEXT("MyTarget"), TEXT("Param:")).ToString());

	OutBeautifiedNames.Add("MultipleColonsInTargetName");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT("Image"), TEXT("A::B"), TEXT("Param")).ToString());

	OutBeautifiedNames.Add("AllFieldsWithColons");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT("Type:"), TEXT("Name:"), TEXT("Param:")).ToString());

	OutBeautifiedNames.Add("AllEmptyValues");
	OutTestCommands.Add(FScenarioPropertyKeyRoundTripTestParameters(
		TEXT(""), TEXT(""), TEXT("")).ToString());
}

bool ScenarioPropertyKeyRoundTripTest::RunTest(const FString& Parameters)
{
	const FScenarioPropertyKeyRoundTripTestParameters Params(Parameters);
	const FString Key = ShidenMaterialParameterHelpers::MakeScenarioPropertyKey(Params.TargetType, Params.TargetName, Params.ParameterName);
	const TTuple<FString, FString, FString> Result = ShidenMaterialParameterHelpers::ParseScenarioPropertyKey(Key);

	if (Result.Get<0>() != Params.TargetType
		|| Result.Get<1>() != Params.TargetName
		|| Result.Get<2>() != Params.ParameterName)
	{
		AddError(FString::Printf(
			TEXT("Round-trip failed for ('%s', '%s', '%s'): key='%s', got ('%s', '%s', '%s')"),
			*Params.TargetType, *Params.TargetName, *Params.ParameterName, *Key,
			*Result.Get<0>(), *Result.Get<1>(), *Result.Get<2>()));
		return false;
	}
	return true;
}
