// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCsvParsedRow.h"
#include "ShidenScenarioStruct.h"
#include "Variable/ShidenPredefinedSystemVariableDefinition.h"
#include "Variable/ShidenVariableDescriptor.h"
#include "ShidenCommandRedirector.h"
#include "ShidenEditorFunctionLibrary.generated.h"

UCLASS()
class SHIDENEDITOR_API UShidenEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void CreateModalForSave(UClass* AssetClass, const FString& DefaultPath, const FString& DefaultAssetName, FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|EditorUtility")
	static bool CanCreateFolder(FName InPath);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void LoadTextFile(const FString& Extension, FString& FileData, FString& FileName, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void SaveTextFile(const FString& DefaultFileName, const FString& SaveText, const FString& Extension, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static TArray<FString> SortStringArray(const TArray<FString>& InArray);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility", meta = (AutoCreateRefTerm = "PropertyName"))
	static void SetDefaultClassProperty(const UClass* TargetClass, const FName& PropertyName, UClass* Value);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static UShidenScenario* ConvertToScenarioFromCsv(const FString& CsvString);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static FString ConvertToCsvFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static UShidenScenario* ConvertToScenarioFromJson(const FString& Json, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static FString ConvertToJsonFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListUserVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListPredefinedSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListSystemVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Variables")
	static void ListLocalVariableDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void AddUserVariableDefinition(const FShidenVariableDefinition& VariableDefinition);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
    static void UpdateUserVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void RemoveUserVariableDefinition(const FString& Name);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void AddSystemVariableDefinition(const FShidenVariableDefinition& VariableDefinition);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void UpdateSystemVariableDefinition(const FString& OldName, const FShidenVariableDefinition& VariableDefinition);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Config")
	static void RemoveSystemVariableDefinition(const FString& Name);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|EditorUtility|Config")
	static void GetPredefinedSystemVariableDefinitions(TArray<FShidenVariableDefinition>& VariableDefinitions);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Command")
	static void RedirectCommands(UShidenScenario* Scenario, bool& bAnyCommandUpdated);

	static TArray<FShidenCommandRedirector> GetRedirectDefinitions();

private:
	static void ParseCsv(const FString& CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow);
};
