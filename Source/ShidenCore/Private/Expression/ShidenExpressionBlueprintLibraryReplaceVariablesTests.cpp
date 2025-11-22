// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Misc/AutomationTest.h"
#include "Misc/Base64.h"
#include "Expression/ShidenExpressionBlueprintLibrary.h"
#include "System/ShidenSubsystem.h"

// =============================================================================
// ReplaceVariablesInExpression Tests
// =============================================================================

struct FReplaceVariablesInExpressionTestParameters
{
	FString InputText;
	FString ExpectedResult;
	TArray<FShidenVariableDefinition> UserVariableDefinitions;
	TArray<FShidenVariableDefinition> SystemVariableDefinitions;
	TArray<FShidenVariableDefinition> LocalVariableDefinitions;

	FString ToString() const
	{
		const FString EncodedInputText = FBase64::Encode(InputText);
		const FString EncodedExpectedResult = FBase64::Encode(ExpectedResult);
		const FString EncodedUserDefs = FBase64::Encode(SerializeDefinitions(UserVariableDefinitions));
		const FString EncodedSystemDefs = FBase64::Encode(SerializeDefinitions(SystemVariableDefinitions));
		const FString EncodedLocalDefs = FBase64::Encode(SerializeDefinitions(LocalVariableDefinitions));
		return FString::Printf(TEXT("%s\t%s\t%s\t%s\t%s"), *EncodedInputText, *EncodedExpectedResult, *EncodedUserDefs, *EncodedSystemDefs, *EncodedLocalDefs);
	}

	FReplaceVariablesInExpressionTestParameters() = default;

	explicit FReplaceVariablesInExpressionTestParameters(const FString& Parameters)
	{
		TArray<FString> Parts;
		Parameters.ParseIntoArray(Parts, TEXT("\t"), false);
		if (Parts.Num() >= 5)
		{
			FBase64::Decode(Parts[0], InputText);
			FBase64::Decode(Parts[1], ExpectedResult);
			FString UserDefsStr, SystemDefsStr, LocalDefsStr;
			FBase64::Decode(Parts[2], UserDefsStr);
			FBase64::Decode(Parts[3], SystemDefsStr);
			FBase64::Decode(Parts[4], LocalDefsStr);
			UserVariableDefinitions = DeserializeDefinitions(UserDefsStr);
			SystemVariableDefinitions = DeserializeDefinitions(SystemDefsStr);
			LocalVariableDefinitions = DeserializeDefinitions(LocalDefsStr);
		}
	}

	FReplaceVariablesInExpressionTestParameters& SetInput(const FString& Text, const FString& Expected)
	{
		InputText = Text;
		ExpectedResult = Expected;
		return *this;
	}

	FReplaceVariablesInExpressionTestParameters& AddUserVariable(const FString& Name, EShidenVariableType Type, const FString& DefaultValue)
	{
		FShidenVariableDefinition Def;
		Def.Name = Name;
		Def.Type = Type;
		Def.DefaultValue = DefaultValue;
		Def.bIsReadOnly = false;
		UserVariableDefinitions.Add(Def);
		return *this;
	}

	FReplaceVariablesInExpressionTestParameters& AddSystemVariable(const FString& Name, EShidenVariableType Type, const FString& DefaultValue)
	{
		FShidenVariableDefinition Def;
		Def.Name = Name;
		Def.Type = Type;
		Def.DefaultValue = DefaultValue;
		Def.bIsReadOnly = false;
		SystemVariableDefinitions.Add(Def);
		return *this;
	}

	FReplaceVariablesInExpressionTestParameters& AddLocalVariable(const FString& Name, EShidenVariableType Type, const FString& DefaultValue)
	{
		FShidenVariableDefinition Def;
		Def.Name = Name;
		Def.Type = Type;
		Def.DefaultValue = DefaultValue;
		Def.bIsReadOnly = false;
		LocalVariableDefinitions.Add(Def);
		return *this;
	}

private:
	static FString SerializeDefinitions(const TArray<FShidenVariableDefinition>& Definitions)
	{
		TArray<FString> Items;
		for (const auto& Def : Definitions)
		{
			Items.Add(FString::Printf(TEXT("%s:%d:%s"), *Def.Name, static_cast<int32>(Def.Type), *Def.DefaultValue));
		}
		return FString::Join(Items, TEXT(";"));
	}

	static TArray<FShidenVariableDefinition> DeserializeDefinitions(const FString& Str)
	{
		TArray<FShidenVariableDefinition> Result;
		if (Str.IsEmpty()) return Result;

		TArray<FString> Items;
		Str.ParseIntoArray(Items, TEXT(";"));
		for (const FString& Item : Items)
		{
			TArray<FString> Parts;
			Item.ParseIntoArray(Parts, TEXT(":"));
			if (Parts.Num() >= 3)
			{
				FShidenVariableDefinition Def;
				Def.Name = Parts[0];
				Def.Type = static_cast<EShidenVariableType>(FCString::Atoi(*Parts[1]));
				Def.DefaultValue = Parts[2];
				Def.bIsReadOnly = false;
				Result.Add(Def);
			}
		}
		return Result;
	}
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FReplaceVariablesInExpressionTest, "ShidenExpressionBlueprintLibrary.ReplaceVariablesInExpression",
                                  EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

void FReplaceVariablesInExpressionTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	// UserVariable_String - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("UserVariable_String"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Hello {PlayerName}!"), TEXT("Hello \"Hero\"!"))
	                    .AddUserVariable(TEXT("PlayerName"), EShidenVariableType::String, TEXT("Hero"))
	                    .ToString());

	// UserVariable_Integer
	OutBeautifiedNames.Add(TEXT("UserVariable_Integer"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Score: {Score}"), TEXT("Score: 100"))
	                    .AddUserVariable(TEXT("Score"), EShidenVariableType::Integer, TEXT("100"))
	                    .ToString());

	// UserVariable_Float
	OutBeautifiedNames.Add(TEXT("UserVariable_Float"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Health: {Health}"), TEXT("Health: 75.5"))
	                    .AddUserVariable(TEXT("Health"), EShidenVariableType::Float, TEXT("75.5"))
	                    .ToString());

	// UserVariable_Boolean
	OutBeautifiedNames.Add(TEXT("UserVariable_Boolean"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Alive: {IsAlive}"), TEXT("Alive: true"))
	                    .AddUserVariable(TEXT("IsAlive"), EShidenVariableType::Boolean, TEXT("true"))
	                    .ToString());

	// UserVariable_Multiple - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("UserVariable_Multiple"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("{PlayerName} scored {Score} points"), TEXT("\"Hero\" scored 100 points"))
	                    .AddUserVariable(TEXT("PlayerName"), EShidenVariableType::String, TEXT("Hero"))
	                    .AddUserVariable(TEXT("Score"), EShidenVariableType::Integer, TEXT("100"))
	                    .ToString());

	// SystemVariable_String - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("SystemVariable_String"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Welcome to {System::GameTitle}"), TEXT("Welcome to \"My Visual Novel\""))
	                    .AddSystemVariable(TEXT("GameTitle"), EShidenVariableType::String, TEXT("My Visual Novel"))
	                    .ToString());

	// SystemVariable_Integer
	OutBeautifiedNames.Add(TEXT("SystemVariable_Integer"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Chapter {System::ChapterNumber}"), TEXT("Chapter 5"))
	                    .AddSystemVariable(TEXT("ChapterNumber"), EShidenVariableType::Integer, TEXT("5"))
	                    .ToString());

	// LocalVariable_String - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("LocalVariable_String"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Local: {Local::LocalName}"), TEXT("Local: \"LocalValue\""))
	                    .AddLocalVariable(TEXT("LocalName"), EShidenVariableType::String, TEXT("LocalValue"))
	                    .ToString());

	// LocalVariable_Integer
	OutBeautifiedNames.Add(TEXT("LocalVariable_Integer"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Local count: {Local::Count}"), TEXT("Local count: 42"))
	                    .AddLocalVariable(TEXT("Count"), EShidenVariableType::Integer, TEXT("42"))
	                    .ToString());

	// PredefinedVariable
	OutBeautifiedNames.Add(TEXT("PredefinedVariable"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("MasterVolume: {Predefined::MasterVolume}"), TEXT("MasterVolume: 1.0"))
	                    .ToString());

	// Mixed_UserAndSystem - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("Mixed_UserAndSystem"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("{UserName} is playing version {System::Version}"), TEXT("\"Player1\" is playing version \"1.0.0\""))
	                    .AddUserVariable(TEXT("UserName"), EShidenVariableType::String, TEXT("Player1"))
	                    .AddSystemVariable(TEXT("Version"), EShidenVariableType::String, TEXT("1.0.0"))
	                    .ToString());

	// Mixed_AllTypes - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("Mixed_AllTypes"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("{User} / {System::Sys} / {Local::Loc}"), TEXT("\"U\" / \"S\" / \"L\""))
	                    .AddUserVariable(TEXT("User"), EShidenVariableType::String, TEXT("U"))
	                    .AddSystemVariable(TEXT("Sys"), EShidenVariableType::String, TEXT("S"))
	                    .AddLocalVariable(TEXT("Loc"), EShidenVariableType::String, TEXT("L"))
	                    .ToString());

	// Undefined - Note: undefined variables become empty string in expression context
	OutBeautifiedNames.Add(TEXT("Undefined"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Hello {UndefinedVar}!"), TEXT("Hello !"))
	                    .ToString());

	// NoPlaceholders_PlainText
	OutBeautifiedNames.Add(TEXT("NoPlaceholders_PlainText"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Hello World!"), TEXT("Hello World!"))
	                    .ToString());

	// NoPlaceholders_Empty
	OutBeautifiedNames.Add(TEXT("NoPlaceholders_Empty"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT(""), TEXT(""))
	                    .ToString());

	// NoPlaceholders_SpecialChars
	OutBeautifiedNames.Add(TEXT("NoPlaceholders_SpecialChars"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Special chars: !@#$%^&*()"), TEXT("Special chars: !@#$%^&*()"))
	                    .ToString());

	// SpacesInPlaceholder - Note: strings are wrapped in double quotes
	OutBeautifiedNames.Add(TEXT("SpacesInPlaceholder"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Value: { TestVar }"), TEXT("Value: \"TestValue\""))
	                    .AddUserVariable(TEXT("TestVar"), EShidenVariableType::String, TEXT("TestValue"))
	                    .ToString());

	// Vector2 - Note: uses [X, Y] format for expression
	OutBeautifiedNames.Add(TEXT("Vector2"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Position: {Position2D}"), TEXT("Position: [100, 200]"))
	                    .AddUserVariable(TEXT("Position2D"), EShidenVariableType::Vector2, TEXT("X=100.0 Y=200.0"))
	                    .ToString());

	// Vector3 - Note: uses [X, Y, Z] format for expression
	OutBeautifiedNames.Add(TEXT("Vector3"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("Position: {Position3D}"), TEXT("Position: [10, 20, 30]"))
	                    .AddUserVariable(TEXT("Position3D"), EShidenVariableType::Vector3, TEXT("X=10.0 Y=20.0 Z=30.0"))
	                    .ToString());

	// VariableValueContainingVariablePattern - Verify no recursive replacement, strings wrapped in quotes
	OutBeautifiedNames.Add(TEXT("VariableValueContainingVariablePattern_NoRecursiveReplacement"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("{Text1} {Text2}"), TEXT("\"{Text2}\" \"test\""))
	                    .AddUserVariable(TEXT("Text1"), EShidenVariableType::String, TEXT("{Text2}"))
	                    .AddUserVariable(TEXT("Text2"), EShidenVariableType::String, TEXT("test"))
	                    .ToString());

	// MultipleOccurrencesOfSameVariable - Verify that all occurrences of the same variable are replaced
	OutBeautifiedNames.Add(TEXT("MultipleOccurrencesOfSameVariable"));
	OutTestCommands.Add(FReplaceVariablesInExpressionTestParameters()
	                    .SetInput(TEXT("{Text} {Text}"), TEXT("\"test\" \"test\""))
	                    .AddUserVariable(TEXT("Text"), EShidenVariableType::String, TEXT("test"))
	                    .ToString());
}

bool FReplaceVariablesInExpressionTest::RunTest(const FString& Parameters)
{
	const FReplaceVariablesInExpressionTestParameters Params(Parameters);
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	// Save original state
	const FShidenVariable OriginalUserVariable = ShidenSubsystem->UserVariable;
	const FShidenVariable OriginalSystemVariable = ShidenSubsystem->SystemVariable;
	const FShidenLocalVariable OriginalLocalVariable = ShidenSubsystem->LocalVariable;
	const TMap<FString, FShidenScenarioProgressStack> OriginalScenarioProgressStack = ShidenSubsystem->ScenarioProgressStack;

	// Setup user variables
	ShidenSubsystem->UserVariable = FShidenVariable(Params.UserVariableDefinitions);

	// Setup system variables
	if (Params.SystemVariableDefinitions.Num() > 0)
	{
		ShidenSubsystem->SystemVariable = FShidenVariable(Params.SystemVariableDefinitions);
	}

	// Setup local variables
	if (Params.LocalVariableDefinitions.Num() > 0)
	{
		// Setup ScenarioProgressStack for "Default" process
		FShidenScenarioProgressStack Stack;
		FShidenScenarioProgress Progress;
		Progress.ScenarioId = FGuid::NewGuid();
		Progress.CurrentIndex = 0;
		Stack.Stack.Add(Progress);
		ShidenSubsystem->ScenarioProgressStack.Add(TEXT("Default"), Stack);

		// Initialize local variables with ScopeKey "Default$0"
		TObjectPtr<UShidenScenario> TestScenario = NewObject<UShidenScenario>();
		// Manually setup local variables since we don't have a real scenario
		for (const FShidenVariableDefinition& Def : Params.LocalVariableDefinitions)
		{
			TestScenario->LocalVariableDefinitions.Add(Def);
		}
		const FString ScopeKey = TEXT("Default$0");
		ShidenSubsystem->LocalVariable.InitLocalVariable(ScopeKey, TestScenario);
	}

	// Execute
	const FString Result = UShidenExpressionBlueprintLibrary::ReplaceVariablesInExpression(TEXT("Default"), Params.InputText);

	// Verify
	bool bSuccess = true;
	if (Result != Params.ExpectedResult)
	{
		AddError(FString::Printf(TEXT("Expected '%s', got '%s'"), *Params.ExpectedResult, *Result));
		bSuccess = false;
	}

	// Restore original state
	ShidenSubsystem->UserVariable = OriginalUserVariable;
	ShidenSubsystem->SystemVariable = OriginalSystemVariable;
	ShidenSubsystem->LocalVariable = OriginalLocalVariable;
	ShidenSubsystem->ScenarioProgressStack = OriginalScenarioProgressStack;

	return bSuccess;
}
