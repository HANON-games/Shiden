// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenCsvParsedRow.h"
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
	static TArray<FString> SortStringArray(const TArray<FString>& InArray);

	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void ParseCsv(FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow);
};
