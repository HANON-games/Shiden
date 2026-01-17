// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenEditor.h"
#include "ISettingsModule.h"
#include "ShidenEditorConfig.h"
#include "EditorSubsystem.h"
#include "ShidenCommandDefinitionCustomization.h"
#include "ShidenScenarioCustomization.h"
#include "Scenario/ShidenScenario.h"
#include "Editor/EditorEngine.h"
#include "ShidenAddNewContextMenu.h"
#include "ShidenMainMenu.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Config/ShidenProjectConfig.h"
#include "ShidenScenarioSyncManager.h"

#define LOCTEXT_NAMESPACE "FShidenEditorModule"

void FShidenEditorModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings(
			"Editor",
			"Plugins",
			"ShidenVisualNovelEditor",
			LOCTEXT("ShidenVisualNovelEditorName", "Shiden Visual Novel Editor"),
			LOCTEXT("ShidenVisualNovelEditorDescription", "Shiden Visual Novel Editor"),
			GetMutableDefault<UShidenEditorConfig>()
		);
	}

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(
		"ShidenCommandDefinition",
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FShidenCommandDefinitionCustomization::MakeInstance)
	);
	PropertyEditorModule.RegisterCustomPropertyTypeLayout(
		UShidenScenario::StaticClass()->GetFName(),
		FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FShidenScenarioCustomization::MakeInstance)
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

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(
			"Editor",
			"Plugins",
			"ShidenVisualNovelEditor"
		);
	}

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.UnregisterCustomPropertyTypeLayout("ShidenCommandDefinition");
	PropertyEditorModule.UnregisterCustomPropertyTypeLayout(UShidenScenario::StaticClass()->GetFName());

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
