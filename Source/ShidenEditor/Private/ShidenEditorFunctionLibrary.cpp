// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorFunctionLibrary.h"
#include "IContentBrowserSingleton.h"
#include "ContentBrowserModule.h"

#define LOCTEXT_NAMESPACE "AssetTools"

SHIDENEDITOR_API void UShidenEditorFunctionLibrary::CreateModalForSave(UClass* AssetClass, const FString DefaultPath, const FString DefaultAssetName ,FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess)
{
	FSaveAssetDialogConfig SaveAssetDialogConfig;
	SaveAssetDialogConfig.DialogTitleOverride = LOCTEXT("SaveAssetDialogTitle", "Save Asset As");
	SaveAssetDialogConfig.DefaultPath = DefaultPath;
	SaveAssetDialogConfig.DefaultAssetName = DefaultAssetName;
	SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::AllowButWarn;
	SaveAssetDialogConfig.AssetClassNames.Add(AssetClass->GetClassPathName());

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	const FString SaveObjectPath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);

	const FString SavePackageName = FPackageName::ObjectPathToPackageName(SaveObjectPath);
	SavePackagePath = FPaths::GetPath(SavePackageName);
	SaveAssetName = FPaths::GetBaseFilename(SavePackageName);
	bSuccess = SaveAssetName.Len() > 0;
}

SHIDENEDITOR_API bool UShidenEditorFunctionLibrary::CanCreateFolder(FName InPath)
{
	UContentBrowserDataSubsystem* ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
	return ContentBrowserData->CanCreateFolder(InPath, nullptr);
}