// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "System/ShidenCsvParsedRow.h"
#include "Scenario/ShidenScenarioStruct.h"
#include "Variable/ShidenVariableDescriptor.h"
#include "System/ShidenCommandRedirector.h"
#include "System/ShidenPluginVersion.h"
#include "Command/ShidenCommandDefinition.h"
#include "Expression/ShidenExpressionEvaluator.h"
#include "Variable/ShidenVariableKind.h"
#include "ShidenEditorBlueprintLibrary.generated.h"

UCLASS()
class SHIDENEDITOR_API UShidenEditorBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void ShowSaveAssetDialog(const UClass* AssetClass, const FString& DefaultPath, const FString& DefaultAssetName,
	                                FString& SavePackagePath, FString& SaveAssetName, bool& bAssetNameWasSet);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|EditorUtility")
	static bool CanCreateFolder(const FName& Path);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility", meta = (DisplayName = "Load Text File"))
	static UPARAM(DisplayName = "Success") bool TryLoadTextFile(const FString& Extension, FString& FileData, FString& FileName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility", meta = (DisplayName = "Save Text File"))
	static UPARAM(DisplayName = "Success") bool TrySaveTextFile(const FString& DefaultFileName, const FString& SaveText, const FString& Extension);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static TArray<FString> CreateSortedStringArray(const TArray<FString>& Array);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility", meta = (AutoCreateRefTerm = "PropertyName"))
	static void SetDefaultClassProperty(const UClass* TargetClass, const FName& PropertyName, UClass* Value);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static FShidenCommand ExpandCommand(const FShidenCommand& SourceCommand, bool bShouldExpandPreset = true, bool bShouldExpandDefaultValue = false);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static UShidenScenario* ConvertToScenarioFromCsv(const FString& CsvString);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static FString ConvertToCsvFromScenario(const UShidenScenario* SourceScenario);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization", meta = (DisplayName = "Convert to Scenario from Json"))
	static  UPARAM(DisplayName = "Success") bool TryConvertToScenarioFromJson(const FString& Json, UShidenScenario*& Scenario);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization", meta = (DisplayName = "Convert to Json from Scenario"))
	static  UPARAM(DisplayName = "Success") bool TryConvertToJsonFromScenario(const UShidenScenario* SourceScenario, FString& Json);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListUserVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListPredefinedSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListLocalVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Utility")
	static void ForceGC();

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void AddUserVariableDefinition(const FShidenVariableDefinition& VariableDefinition);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void UpdateUserVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void UpdateUserVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void RemoveUserVariableDefinition(const FString& Name);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void AddSystemVariableDefinition(const FShidenVariableDefinition& VariableDefinition);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void UpdateSystemVariableDefinitions(const TArray<FShidenVariableDefinition>& VariableDefinitions);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void UpdateSystemVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void RemoveSystemVariableDefinition(const FString& Name);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|EditorUtility|Config")
	static void GetPredefinedSystemVariableDefinitions(TArray<FShidenVariableDefinition>& VariableDefinitions);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config", meta = (DisplayName = "Get Current Plugin Version"))
	static UPARAM(DisplayName = "Success") bool TryGetCurrentPluginVersion(FShidenPluginVersion& PluginVersion);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config", meta = (DisplayName = "Parse Version String"))
	static UPARAM(DisplayName = "Success") bool TryParseVersionString(const FString& VersionStr, FShidenPluginVersion& OutVersion);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command")
	static void RedirectCommands(UShidenScenario* Scenario, const FShidenPluginVersion& SourcePluginVersion, bool& AnyCommandUpdated);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command")
	static void RedirectLocalVariables(UShidenScenario* Scenario, const FString& OldVariableName, const FString& NewVariableName, bool& AnyCommandUpdated);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command")
	static void RedirectAllMacroParameters(const UShidenScenario* TargetMacro, const FString& TargetMacroPath, const FString& OldParameterName, const FString& NewParameterName,
	                                       const bool bNeedTransaction, TArray<UShidenScenario*>& AssetToBeSaved);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command")
	static void RedirectAllVariables(const EShidenVariableKind VariableKind, const FString& OldVariableName, const FString& NewVariableName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config", meta = (DisplayName = "Migrate Plugin"))
	static UPARAM(DisplayName = "Success") bool TryMigratePlugin();

	UFUNCTION(BlueprintCallable, Category="SvnInternal|EditorUtility|Config", meta=(AutoCreateRefTerm="ContainerName, CategoryName, SectionName"))
	static void OpenSettings(const FName& ContainerName, const FName& CategoryName, const FName& SectionName);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command", meta = (DisplayName = "Evaluate Input Visibility"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateInputVisibility(const UShidenScenario* Scenario, const TMap<FString, FString>& ExpandedCommandArgs, const FString& EditorVisibilityCondition,
	                                                                       bool& OutShouldShow, FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command", meta = (DisplayName = "Evaluate Conditional Messages"))
	static UPARAM(DisplayName = "Success") bool TryEvaluateConditionalMessages(const UShidenScenario* Scenario, const FShidenCommandDefinition& CommandDefinition, const TMap<FString, FString>& CommandArgs,
	                                                                           const FName& ArgName, TArray<FText>& OutInformationMessages, TArray<FText>& OutWarningMessages, TArray<FText>& OutErrorMessages,FString& ErrorMessage);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command", meta = (DisplayName = "Validate Command"))
	static UPARAM(DisplayName = "Success") bool TryValidateCommand(const UShidenScenario* Scenario, const FShidenCommandDefinition& CommandDefinition,
	                                                               const TMap<FString, FString>& CommandArgs, bool&HasInformation, bool& HasWarning, bool& HasError,FString& ErrorMessage);

private:
	static bool TryEvaluateConditionalMessagesCore(const FShidenExpressionVariableDefinitionContext& Context, const TMap<FString, FString>& CommandArgs,
												   const TArray<FShidenConditionalMessage>& InformationConditionalMessages, const TArray<FShidenConditionalMessage>& WarningConditionalMessages,
												   const TArray<FShidenConditionalMessage>& ErrorConditionalMessages, TArray<FText>& OutInformationMessages, TArray<FText>& OutWarningMessages,
												   TArray<FText>& OutErrorMessages,FString& ErrorMessage);
	
	static TArray<FShidenCommandRedirector> GetRedirectDefinitions(const FShidenPluginVersion& SourcePluginVersion);

	static void ParseCsvContent(const FString& CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow);

	static FString ReplaceArgumentReferences(const TMap<FString, FString>& CommandArgs, const FString& Expression);

	static FShidenExpressionVariableDefinitionContext BuildExpressionContext(const UShidenScenario* Scenario);
};
