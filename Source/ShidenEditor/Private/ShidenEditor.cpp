// Copyright (c) 2026 HANON. All Rights Reserved.

#include "ShidenEditor.h"
#include "Config/ShidenEditorConfig.h"
#include "EditorSubsystem.h"
#include "Command/ShidenCommandDefinitionCustomization.h"
#include "Command/ShidenCommandArgumentEditorSettingsCustomization.h"
#include "Config/ShidenProjectConfigCustomization.h"
#include "Config/ShidenEditorConfigCustomization.h"
#include "Scenario/ShidenScenarioCustomization.h"
#include "Scenario/ShidenScenario.h"
#include "Editor/EditorEngine.h"
#include "Menu/ShidenAddNewContextMenu.h"
#include "Menu/ShidenMainMenu.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Config/ShidenProjectConfig.h"
#include "System/ShidenScenarioSyncManager.h"

#define LOCTEXT_NAMESPACE "FShidenEditorModule"

void FShidenEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(
		"ShidenCommandDefinition",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FShidenCommandDefinitionCustomization::MakeInstance)
	);
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(
		"ShidenCommandArgumentEditorSettings",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FShidenCommandArgumentEditorSettingsCustomization::MakeInstance)
	);
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(
		UShidenScenario::StaticClass()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FShidenScenarioCustomization::MakeInstance)
	);
	PropertyEditorModule.RegisterCustomClassLayout(
		UShidenProjectConfig::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FShidenProjectConfigCustomization::MakeInstance)
	);
	PropertyEditorModule.RegisterCustomClassLayout(
		UShidenEditorConfig::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FShidenEditorConfigCustomization::MakeInstance)
	);
	PropertyEditorModule.NotifyCustomizationModuleChanged();

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FShidenEditorModule::RegisterMenuExtensions)
	);

	FEditorDelegates::OnEditorInitialized.AddLambda([](double)
	{
		InitializeScenarioPaths();

		// Start scenario sync if enabled
		const TObjectPtr<const UShidenEditorConfig> EditorConfig = GetDefault<UShidenEditorConfig>();
		if (EditorConfig && EditorConfig->bEnableScenarioSync)
		{
			UShidenScenarioSyncManager::StartWatchingDirectory();
		}
	});
}

void FShidenEditorModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	// Stop scenario sync watching
	UShidenScenarioSyncManager::StopWatchingDirectory();

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomPropertyTypeLayout("ShidenCommandDefinition");
	PropertyEditorModule.UnregisterCustomPropertyTypeLayout("ShidenCommandArgumentEditorSettings");
	PropertyEditorModule.UnregisterCustomPropertyTypeLayout(UShidenScenario::StaticClass()->GetFName());
	PropertyEditorModule.UnregisterCustomClassLayout(UShidenProjectConfig::StaticClass()->GetFName());
	PropertyEditorModule.UnregisterCustomClassLayout(UShidenEditorConfig::StaticClass()->GetFName());

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FEditorDelegates::OnEditorInitialized.RemoveAll(this);
}

void FShidenEditorModule::RegisterMenuExtensions()
{
	UShidenMainMenu::RegisterMenuExtensions(this);
	UShidenAddNewContextMenu::RegisterMenuExtensions(this);
}

void FShidenEditorModule::InitializeScenarioPaths()
{
	if (!GIsEditor || IsRunningCommandlet() || IsRunningCookCommandlet())
	{
		return;
	}

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	AssetRegistry.WaitForCompletion();

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	TArray<FAssetData> AssetDataList;
	AssetRegistry.GetAssetsByPath(FName(*ShidenProjectConfig->ScenarioDirectoryPath), AssetDataList, true);

	TArray<FAssetData> MacroAssetDataList;
	AssetRegistry.GetAssetsByPath(FName(*ShidenProjectConfig->MacroDirectoryPath), MacroAssetDataList, true);
	AssetDataList.Append(MacroAssetDataList);

	TMap<FGuid, FString> ScenarioPaths;
	for (const FAssetData& AssetData : AssetDataList)
	{
		if (AssetData.AssetClassPath != UShidenScenario::StaticClass()->GetClassPathName())
		{
			continue;
		}

		if (const TObjectPtr<UShidenScenario> Scenario = Cast<UShidenScenario>(AssetData.GetAsset()))
		{
			ScenarioPaths.Add(Scenario->ScenarioId, AssetData.GetObjectPathString());
		}
	}

	ShidenProjectConfig->SetScenarioPaths(ScenarioPaths);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShidenEditorModule, ShidenEditor)
