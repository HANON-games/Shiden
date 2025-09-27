// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenAddNewContextMenu.h"
#include "AssetToolsModule.h"
#include "ContentBrowserDataMenuContexts.h"
#include "ContentBrowserDataSubsystem.h"
#include "ContentBrowserItemPath.h"
#include "EditorUtilityLibrary.h"
#include "IAssetTools.h"
#include "UObject/ScriptInterface.h"
#include "IContentBrowserDataModule.h"
#include "ShidenEditorBlueprintLibrary.h"
#include "Command/ShidenCommandDefinitions.h"
#include "Subsystems/EditorAssetSubsystem.h"

#define LOCTEXT_NAMESPACE "ShidenAddNewContextMenu"

void UShidenAddNewContextMenu::RegisterMenuExtensions(IModuleInterface *Owner)
{
	FToolMenuOwnerScoped OwnerScoped(Owner);

	UToolMenu* AddNewContextMenu = UToolMenus::Get()->ExtendMenu(FName("ContentBrowser.AddNewContextMenu"));
	if (!AddNewContextMenu)
	{
		return;
	}
	
	FToolMenuSection& Section = AddNewContextMenu->FindOrAddSection(TEXT("ContentBrowserNewAdvancedAsset"));
	
	Section.AddDynamicEntry(
		"ShidenAsset",
		FNewToolMenuSectionDelegate::CreateLambda([](FToolMenuSection& InSection)
		{
			const TObjectPtr<UContentBrowserDataSubsystem> ContentBrowserData = IContentBrowserDataModule::Get().GetSubsystem();
			
			const UContentBrowserDataMenuContext_AddNewMenu* ContextObject = InSection.FindContext<UContentBrowserDataMenuContext_AddNewMenu>();
			checkf(ContextObject, TEXT("Required context UContentBrowserDataMenuContext_AddNewMenu was missing!"));
			
			if (ContextObject->SelectedPaths.Num() != 1 || !ContentBrowserData->CanCreateFolder(ContextObject->SelectedPaths[0], nullptr))
			{
				return;
			}
			
			InSection.AddSubMenu(
				"ShidenAsset",
				FText::FromString("Shiden Visual Novel"),
				FText::GetEmpty(),
				FNewToolMenuDelegate::CreateLambda([](UToolMenu* InMenu)
				{
					FToolMenuSection& ToolMenuSection = InMenu->AddSection(FName("Shiden"));
		
					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenActor"),
						FText::FromString("Shiden Actor"),
						LOCTEXT("CreateShidenActorTooltip", "Create a new Shiden Actor"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.Actor"), FName("ClassIcon.Actor")),
						FUIAction(
							FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/AC_ShidenActorSample"), TEXT("AC_MyShidenActor")); })
						)
					);

					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenWidget"),
						FText::FromString("Shiden Widget"),
						LOCTEXT("CreateShidenWidgetTooltip", "Create a new Shiden Widget"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
						FUIAction(
							FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenWidgetSample"), TEXT("WBP_MyShidenWidget")); })
						)
					);

					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenTextWidget"),
						FText::FromString("Shiden Text Widget"),
						LOCTEXT("CreateShidenTextWidgetTooltip", "Create a new Shiden Text Widget"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
						FUIAction(
							FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenTextWidgetSample"), TEXT("WBP_MyTextWidget")); })
						)
					);

					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenBacklogWidget"),
						FText::FromString("Shiden Backlog Widget"),
						LOCTEXT("CreateShidenBacklogTooltip", "Create a new Shiden Backlog Widget"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
						FUIAction(
							FExecuteAction::CreateLambda([]
							{
								const TObjectPtr<UObject> BacklogAsset = CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenBacklogSample"), TEXT("WBP_MyBacklog"));
								const TObjectPtr<UObject> BacklogItemAsset = CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenBacklogItemSample"), TEXT("WBP_MyBacklogItem"));

								const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

								const FString BacklogPackagePath = FPackageName::ObjectPathToPackageName(BacklogAsset->GetPathName());
								const FString BacklogItemPackagePath = FPackageName::ObjectPathToPackageName(BacklogItemAsset->GetPathName());

								const TObjectPtr<UClass> BacklogAssetClass = EditorAssetSubsystem->LoadBlueprintClass(BacklogPackagePath);
								const TObjectPtr<UClass> BacklogItemAssetClass = EditorAssetSubsystem->LoadBlueprintClass(BacklogItemPackagePath);

								UShidenEditorBlueprintLibrary::SetDefaultClassProperty(BacklogAssetClass, FName("BacklogItemClass"), BacklogItemAssetClass);
							})
						)
					);

					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenSaveMenu"),
						FText::FromString("Shiden Save Menu"),
						LOCTEXT("CreateShidenSaveMenuTooltip", "Create a new Shiden Save Menu"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
						FUIAction(
							FExecuteAction::CreateLambda([]
							{
								const TObjectPtr<UObject> SaveMenuAsset = CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenSaveMenuSample"), TEXT("WBP_MySaveMenu"));
								const TObjectPtr<UObject> SaveSlotAsset = CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenSaveSlotSample"), TEXT("WBP_MySaveSlot"));
								
								const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();

								const FString SaveMenuPackagePath = FPackageName::ObjectPathToPackageName(SaveMenuAsset->GetPathName());
								const FString SaveSlotPackagePath = FPackageName::ObjectPathToPackageName(SaveSlotAsset->GetPathName());

								const TObjectPtr<UClass> SaveMenuAssetClass = EditorAssetSubsystem->LoadBlueprintClass(SaveMenuPackagePath);
								const TObjectPtr<UClass> SaveSlotAssetClass = EditorAssetSubsystem->LoadBlueprintClass(SaveSlotPackagePath);

								UShidenEditorBlueprintLibrary::SetDefaultClassProperty(SaveMenuAssetClass, FName("SaveSlotClass"), SaveSlotAssetClass);
							})
						)
					);

					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenConfigMenu"),
						FText::FromString("Shiden Config Menu"),
						LOCTEXT("CreateShidenConfigMenuTooltip", "Create a new Shiden Config Menu"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
						FUIAction(
							FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenConfigMenuSample"), TEXT("WBP_MyConfigMenu")); })
						)
					);

					ToolMenuSection.AddMenuEntry(
						FName("CreateShidenTitle"),
						FText::FromString("Shiden Title"),
						LOCTEXT("CreateShidenTitleTooltip", "Create a new Shiden Title"),
						FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
						FUIAction(
							FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/WBP_ShidenTitleSample"), TEXT("WBP_MyTitle")); })
						)
					);
					
					ToolMenuSection.AddSubMenu(
                        "ShidenAdvanced",
                        FText::FromString("Advanced"),
                        FText::GetEmpty(),
                        FNewToolMenuDelegate::CreateLambda([](UToolMenu* InAdvancedMenu)
                        {
                        	FToolMenuSection& AdvancedToolMenuSection = InAdvancedMenu->AddSection(FName("Shiden"));

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateShidenCommand"),
								FText::FromString("Shiden Command"),
								LOCTEXT("CreateShidenCommandTooltip", "Create a new Shiden Command"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.Blueprint"), FName("ClassIcon.Blueprint")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/BP_ShidenCommandSample"), TEXT("BP_MyCommand")); })
								)
							);

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateShidenTextCommand"),
								FText::FromString("Shiden Text Command"),
								LOCTEXT("CreateShidenTextCommandTooltip", "Create a new Shiden Text Command"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.Blueprint"), FName("ClassIcon.Blueprint")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Samples/BP_ShidenTextCommandSample"), TEXT("BP_MyTextCommand")); })
								)
							);

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateNewCommandDefinitions"),
								FText::FromString("Shiden Command Definitions"),
								LOCTEXT("CreateShidenCommandDefinitionsTooltip", "Create a new Shiden Command Definitions"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.DataAsset"), FName("ClassIcon.DataAsset")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAssetFromClass(UShidenCommandDefinitions::StaticClass(), TEXT("DA_MyCommandDefinitions")); })
								)
							);

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateShidenEditorInput"),
								FText::FromString("Shiden Editor Input"),
								LOCTEXT("CreateShidenEditorInputTooltip", "Create a new Shiden Editor Input"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.EditorUtilityWidgetBlueprint"), FName("ClassIcon.EditorUtilityWidgetBlueprint")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Editor/Inputs/EUW_ShidenTextInput"), TEXT("EUW_MyInput")); })
								)
							);

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateShidenRichTextStyleRow"),
								FText::FromString("Shiden Rich Text Style Row"),
								LOCTEXT("CreateShidenRichTextStyleRowTooltip", "Create a new Shiden Rich Text Style Row"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Misc/RichText/DT_ShidenRichTextStyleRow"), TEXT("DT_MyRichTextStyleRow")); })
								)
							);

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateShidenRichImageRow"),
								FText::FromString("Shiden Rich Image Row"),
								LOCTEXT("CreateShidenRichImageRowTooltip", "Create a new Shiden Rich Image Row"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.UserWidget"), FName("ClassIcon.UserWidget")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAsset(TEXT("/Shiden/Misc/RichText/DT_ShidenRichImageRow"), TEXT("DT_MyRichImageRow")); })
								)
							);

                        	AdvancedToolMenuSection.AddMenuEntry(
								FName("CreateNewCommandRedirectors"),
								FText::FromString("Shiden Command Redirectors"),
								LOCTEXT("CreateShidenCommandRedirectorsTooltip", "Create a new Shiden Command Redirectors"),
								FSlateIcon(FName("EditorStyle"), FName("ClassThumbnail.DataAsset"), FName("ClassIcon.DataAsset")),
								FUIAction(
									FExecuteAction::CreateLambda([] { CreateAssetFromClass(UShidenCommandRedirectors::StaticClass(), TEXT("DT_MyCommandRedirectors")); })
								)
							);
                        })
                    );
				})
			);
		}
	));
}

UObject* UShidenAddNewContextMenu::CreateAsset(const FString& Path, const FString& NewAssetName)
{
	FString PackageName;
	if (!TryDecideNewPackageName(NewAssetName, PackageName))
	{
		return nullptr;
	}

	const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>();
	return EditorAssetSubsystem->DuplicateAsset(Path, PackageName);
}

UObject* UShidenAddNewContextMenu::CreateAssetFromClass(UClass* AssetClass, const FString& NewAssetName)
{
	if (!IsValid(AssetClass))
	{
		return nullptr;
	}

	FString PackageName;
	if (!TryDecideNewPackageName(NewAssetName, PackageName))
	{
		return nullptr;
	}

	FString AssetName, PackagePath;
	PackageName.Split(TEXT("/"), &PackagePath, &AssetName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	const FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	IAssetTools& AssetTools = AssetToolsModule.Get();
	return AssetTools.CreateAsset(AssetName, PackagePath, AssetClass, nullptr);
}

bool UShidenAddNewContextMenu::TryDecideNewPackageName(const FString& NewAssetName, FString& OutAssetPath)
{
	if (!GEditor)
	{
		return false;
	}

	TArray<FString> Paths = UEditorUtilityLibrary::GetSelectedPathViewFolderPaths();
	if (Paths.Num() != 1)
	{
		return false;
	}
	
	FContentBrowserItemPath CurrentPath;
	CurrentPath.SetPathFromString(Paths[0], EContentBrowserPathType::Virtual);
	
	const FString PackageNamePrefix = CurrentPath.GetInternalPathString() + TEXT("/") + NewAssetName;

	OutAssetPath = PackageNamePrefix;
	int32 Suffix = 1;

	while (FPackageName::DoesPackageExist(OutAssetPath))
	{
		OutAssetPath = FString::Printf(TEXT("%s%d"), *PackageNamePrefix, Suffix++);
	}

	return true;
}

#undef LOCTEXT_NAMESPACE