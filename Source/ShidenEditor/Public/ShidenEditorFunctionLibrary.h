// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCsvParsedRow.h"
#include "ShidenScenarioStruct.h"
#include "ShidenEditorFunctionLibrary.generated.h"

UCLASS()
class SHIDENEDITOR_API UShidenEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void CreateModalForSave(UClass* AssetClass, const FString DefaultPath, const FString DefaultAssetName, FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|EditorUtility")
	static bool CanCreateFolder(FName InPath);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void LoadTextFile(const FString& Extension, FString& FileData, FString& FileName, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static bool SaveTextFile(const FString DefaultFileName, const FString SaveText, const FString Extension);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static TArray<FString> SortStringArray(const TArray<FString>& InArray);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void SetDefaultClassProperty(const UClass* TargetClass, const FName PropertyName, UClass* Value);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static void ParseCsv(FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow);
	
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static UShidenScenario* ConvertToScenarioFromCsv(const FString& CsvString);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static FString ConvertToCsvFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static UShidenScenario* ConvertToScenarioFromJson(const FString& Json, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility|Serialization")
	static FString ConvertToJsonFromScenario(const UShidenScenario* SourceScenario, const bool bExpandPresets, bool& bSuccess);
};
