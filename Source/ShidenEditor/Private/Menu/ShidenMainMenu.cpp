// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Menu/ShidenMainMenu.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

namespace
{
	void AddEditorUtilityWidgetMenuEntry(FToolMenuSection& Section, const FName& EntryName, const FText& Label, const FText& Tooltip, const FString& WidgetPath)
	{
		Section.AddMenuEntry(
			EntryName,
			Label,
			Tooltip,
			FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
			FUIAction(FExecuteAction::CreateLambda([WidgetPath]
			{
				if (GEditor)
				{
					if (const TObjectPtr<UEditorUtilityWidgetBlueprint> WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *WidgetPath))
					{
						GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBP);
					}
				}
			}))
		);
	}
}

void UShidenMainMenu::RegisterMenuExtensions(IModuleInterface* Owner)
{
	FToolMenuOwnerScoped OwnerScoped(Owner);

	const TObjectPtr<UToolMenu> MainMenu = UToolMenus::Get()->ExtendMenu(FName("MainFrame.MainMenu.Window"));
	if (!MainMenu)
	{
		return;
	}

	FToolMenuSection& Section = MainMenu->AddSection(FName("SHIDEN"), FText::FromString(TEXT("Shiden Visual Novel")), FToolMenuInsert(NAME_None, EToolMenuInsertType::After));

	AddEditorUtilityWidgetMenuEntry(
		Section,
		"OpenShidenEditor",
		LOCTEXT("ShidenEditorLabel", "Shiden Editor"),
		LOCTEXT("OpenShidenEditorTooltip", "Open Shiden Editor"),
		TEXT("/Shiden/Editor/Core/EUW_ShidenVisualNovelEditor.EUW_ShidenVisualNovelEditor")
	);

	AddEditorUtilityWidgetMenuEntry(
		Section,
		"OpenShidenDebugger",
		LOCTEXT("ShidenDebuggerLabel", "Shiden Debugger"),
		LOCTEXT("OpenShidenDebuggerTooltip", "Open Shiden Debugger"),
		TEXT("/Shiden/Editor/Core/EUW_ShidenDebugger.EUW_ShidenDebugger")
	);

	AddEditorUtilityWidgetMenuEntry(
		Section,
		"OpenShidenAssetList",
		LOCTEXT("ShidenAssetListLabel", "Shiden Asset List"),
		LOCTEXT("OpenShidenAssetListTooltip", "Open Shiden Asset List"),
		TEXT("/Shiden/Editor/Core/EUW_ShidenAssetList.EUW_ShidenAssetList")
	);

	AddEditorUtilityWidgetMenuEntry(
		Section,
		"OpenShidenVariables",
		LOCTEXT("ShidenVariablesLabel", "Shiden Variables"),
		LOCTEXT("OpenShidenVariablesTooltip", "Open Shiden Variables"),
		TEXT("/Shiden/Editor/Core/EUW_ShidenVariables.EUW_ShidenVariables")
	);
}

#undef LOCTEXT_NAMESPACE
