// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorFunctionLibrary.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserDataModule.h"
#include "Serialization/Csv/CsvParser.h"

#define LOCTEXT_NAMESPACE "AssetTools"

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::CreateModalForSave(UClass* AssetClass, const FString DefaultPath, const FString DefaultAssetName ,FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess)
{
	FSaveAssetDialogConfig SaveAssetDialogConfig;
	SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
	SaveAssetDialogConfig.DefaultPath = DefaultPath;
	SaveAssetDialogConfig.DefaultAssetName = DefaultAssetName;
	SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::AllowButWarn;
	SaveAssetDialogConfig.AssetClassNames.Add(AssetClass->GetClassPathName());

	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);

	const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
	SavePackagePath = FPaths::GetPath(SavePackageName);
	SaveAssetName = FPaths::GetBaseFilename(SavePackageName);
	bSuccess = SaveAssetName.Len() > 0;
}

SHIDENEDITOR_API bool UShidenEditorFunctionLibrary::CanCreateFolder(const FName InPath)
{
	const TObjectPtr<UContentBrowserDataSubsystem> ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
	return ContentBrowserData->CanCreateFolder(InPath, nullptr);
}

SHIDENEDITOR_API TArray<FString> UShidenEditorFunctionLibrary::SortStringArray(const TArray<FString>& InArray)
{
	TArray<FString> OutArray = InArray;
	OutArray.Sort();
	return OutArray;
}

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::ParseCsv(const FString CsvText, TArray<FShidenCsvParsedRow>& CsvParsedRow)
{
	const FCsvParser Parser = FCsvParser(CsvText);
	auto& Rows = Parser.GetRows();
	bool bCommentEnd = false;

	for (auto& Row : Rows)
	{
		// Skip comment lines like "# comment"
		if (!bCommentEnd && FString(Row[0]).TrimStart().TrimEnd().StartsWith(TEXT("#")))
		{
			continue;
		}

		bCommentEnd = true;

		FShidenCsvParsedRow ParsedRow;
		for (const TCHAR* Cell : Row)
		{
			ParsedRow.Row.Add(FString(Cell));
		}
		CsvParsedRow.Add(ParsedRow);
	}
}
