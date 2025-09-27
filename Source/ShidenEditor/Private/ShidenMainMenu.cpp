// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenMainMenu.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "ShidenNamespace"

void UShidenMainMenu::RegisterMenuExtensions(IModuleInterface *Owner)
{
	FToolMenuOwnerScoped OwnerScoped(Owner);

	const TObjectPtr<UToolMenu> MainMenu = UToolMenus::Get()->ExtendMenu(FName("MainFrame.MainMenu.Window"));
	if (!MainMenu)
	{
		return;
	}

	FToolMenuSection& Section = MainMenu->AddSection(FName("SHIDEN"), FText::FromString(TEXT("Shiden Visual Novel")), FToolMenuInsert(NAME_None, EToolMenuInsertType::After));

	Section.AddMenuEntry(
		"OpenShidenEditor",
		LOCTEXT("ShidenEditorLabel", "Shiden Editor"),
		LOCTEXT("OpenShidenEditorTooltip", "Open Shiden Editor"),
		FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
	FUIAction(FExecuteAction::CreateLambda([]
		{
			if (!GEditor)
			{
				return;
			}
			static UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, TEXT("/Shiden/Editor/Core/EUW_ShidenVisualNovelEditor.EUW_ShidenVisualNovelEditor"));
			GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBP);
		}))
	);
	
	Section.AddMenuEntry(
		"OpenShidenDebugger",
		LOCTEXT("ShidenDebuggerLabel", "Shiden Debugger"),
		LOCTEXT("OpenShidenDebuggerTooltip", "Open Shiden Debugger"),
		FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
		FUIAction(FExecuteAction::CreateLambda([]
		{
			if (!GEditor)
			{
				return;
			}
			static UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, TEXT("/Shiden/Editor/Core/EUW_ShidenDebugger.EUW_ShidenDebugger"));
			GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBP);
		}))
	);
	
	Section.AddMenuEntry(
		"OpenShidenAssetList",
		LOCTEXT("ShidenAssetListLabel", "Shiden Asset List"),
		LOCTEXT("OpenShidenAssetListTooltip", "Open Shiden Asset List"),
		FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
		FUIAction(FExecuteAction::CreateLambda([]
		{
			if (!GEditor)
			{
				return;
			}
			static UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, TEXT("/Shiden/Editor/Core/EUW_ShidenAssetList.EUW_ShidenAssetList"));
			GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBP);
		}))
	);

	Section.AddMenuEntry(
		"OpenShidenVariables",
		LOCTEXT("ShidenVariablesLabel", "Shiden Variables"),
		LOCTEXT("OpenShidenVariablesTooltip", "Open Shiden Variables"),
		FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
		FUIAction(FExecuteAction::CreateLambda([]
		{
			if (!GEditor)
			{
				return;
			}
			static UEditorUtilityWidgetBlueprint* WidgetBP = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, TEXT("/Shiden/Editor/Core/EUW_ShidenVariables.EUW_ShidenVariables"));
			GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->SpawnAndRegisterTab(WidgetBP);
		}))
	);
}

#undef LOCTEXT_NAMESPACE