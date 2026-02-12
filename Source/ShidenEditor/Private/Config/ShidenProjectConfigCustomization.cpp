// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Config/ShidenProjectConfigCustomization.h"
#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Config/ShidenProjectConfig.h"
#include "System/ShidenEditorBlueprintLibrary.h"
#include "PropertyCustomizationHelpers.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Layout/SBox.h"
#include "Styling/AppStyle.h"
#include "Widgets/Images/SImage.h"

#define LOCTEXT_NAMESPACE "ShidenProjectConfigCustomization"

void FShidenProjectConfigCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	IDetailCategoryBuilder& ConfigCategory = DetailBuilder.EditCategory("Shiden Visual Novel");

	// Add custom path properties with browse buttons
	AddContentBrowserPathProperty(DetailBuilder, GET_MEMBER_NAME_CHECKED(UShidenProjectConfig, ScenarioDirectoryPath),
	                              LOCTEXT("ScenarioDirectoryPath", "Scenario Directory Path"));

	AddContentBrowserPathProperty(DetailBuilder, GET_MEMBER_NAME_CHECKED(UShidenProjectConfig, MacroDirectoryPath),
	                              LOCTEXT("MacroDirectoryPath", "Macro Directory Path"));

	const TSharedRef<IPropertyHandle> CommandDefinitionsHandle = DetailBuilder.GetProperty(
		GET_MEMBER_NAME_CHECKED(UShidenProjectConfig, CommandDefinitions));

	// Hide the default auto-generated property first to prevent duplicate display
	DetailBuilder.HideProperty(CommandDefinitionsHandle);

	// Get the array handle to access array operations
	TSharedPtr<IPropertyHandleArray> ArrayHandle = CommandDefinitionsHandle->AsArray();

	// Get default name widget
	TSharedPtr<SWidget> NameWidget;
	TSharedPtr<SWidget> ValueWidget;
	IDetailPropertyRow& PropertyRow = ConfigCategory.AddProperty(CommandDefinitionsHandle);
	PropertyRow.GetDefaultWidgets(NameWidget, ValueWidget);

	// Manually construct the value content with custom button order
	PropertyRow.ShowPropertyButtons(false) // Hide default property buttons
		.CustomWidget(true) // true = show children (array elements)
		.NameContent()
		[
			NameWidget.ToSharedRef()
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			// Array element count text
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			.Padding(0.0f, 0.0f, 2.0f, 0.0f)
			[
				SNew(STextBlock)
				.Text_Lambda([ArrayHandle]
				{
					uint32 NumElements = 0;
					if (ArrayHandle.IsValid())
					{
						ArrayHandle->GetNumElements(NumElements);
					}
					if (NumElements == 1)
					{
						return LOCTEXT("SingleArrayElement", "1 Array element");
					}
					return FText::Format(LOCTEXT("NumArrayElements", "{0} Array elements"), NumElements);
				})
				.Font(IDetailLayoutBuilder::GetDetailFont())
			]
			// Array buttons (Add and Clear)
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				PropertyCustomizationHelpers::MakeAddButton(
					FSimpleDelegate::CreateLambda([ArrayHandle]
					{
						if (ArrayHandle.IsValid())
						{
							ArrayHandle->AddItem();
						}
					}),
					LOCTEXT("AddButtonToolTipText", "Add Element")
				)
			]
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.VAlign(VAlign_Center)
			[
				PropertyCustomizationHelpers::MakeEmptyButton(
					FSimpleDelegate::CreateLambda([ArrayHandle]
					{
						if (ArrayHandle.IsValid())
						{
							ArrayHandle->EmptyArray();
						}
					}),
					LOCTEXT("EmptyButtonToolTipText", "Remove All Elements")
				)
			]
			// Export button
			+ SHorizontalBox::Slot()
			.AutoWidth()
			.Padding(4.0f, 0.0f, 0.0f, 1.0f)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.Text(LOCTEXT("ExportCommandDefinitionsJsonButton", "Export as JSON"))
				.ToolTipText(LOCTEXT("ExportCommandDefinitionsJsonTooltip", "Export Command Definitions as JSON file"))
				.OnClicked_Lambda([]() -> FReply
				{
					OnExportCommandDefinitionsJsonClicked();
					return FReply::Handled();
				})
			]
		];
}

void FShidenProjectConfigCustomization::OnExportCommandDefinitionsJsonClicked()
{
	FString Json;
	if (UShidenEditorBlueprintLibrary::TryConvertCommandDefinitionsToJson(Json))
	{
		UShidenEditorBlueprintLibrary::TrySaveTextFile(TEXT("CommandDefinitions"), Json, TEXT("json"));
	}
}

void FShidenProjectConfigCustomization::AddContentBrowserPathProperty(
	IDetailLayoutBuilder& DetailBuilder,
	const FName& PropertyName, const FText&)
{
	const TSharedRef<IPropertyHandle> PropertyHandle = DetailBuilder.GetProperty(PropertyName);

	// Get the default property row and customize it
	if (IDetailPropertyRow* PropertyRow = DetailBuilder.EditDefaultProperty(PropertyHandle))
	{
		PropertyRow->CustomWidget()
			.NameContent()
			[
				PropertyHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			.MinDesiredWidth(250.0f)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
				.VAlign(VAlign_Center)
				[
					SNew(SEditableTextBox)
					.Text_Lambda([PropertyHandle]
					{
						FString Value;
						PropertyHandle->GetValue(Value);
						return FText::FromString(Value);
					})
					.OnTextCommitted_Lambda([PropertyHandle](const FText& NewText, ETextCommit::Type)
					{
						PropertyHandle->SetValue(NewText.ToString());
					})
					.Font(IDetailLayoutBuilder::GetDetailFont())
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				.Padding(4.0f, 0.0f, 0.0f, 0.0f)
				[
					SNew(SButton)
					.ButtonStyle(FAppStyle::Get(), "SimpleButton")
					.OnClicked_Lambda([PropertyHandle]
					{
						OnBrowseContentPathClicked(PropertyHandle);
						return FReply::Handled();
					})
					.ContentPadding(0)
					.ToolTipText(LOCTEXT("BrowsePathTooltip", "Browse for a content directory"))
					[
						SNew(SImage)
						.Image(FAppStyle::GetBrush("Icons.FolderOpen"))
						.ColorAndOpacity(FSlateColor::UseForeground())
					]
				]
			];
	}
}

void FShidenProjectConfigCustomization::OnBrowseContentPathClicked(TSharedPtr<IPropertyHandle> PropertyHandle)
{
	const FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	FString CurrentPath;
	PropertyHandle->GetValue(CurrentPath);
	if (CurrentPath.IsEmpty())
	{
		CurrentPath = TEXT("/Game");
	}
	// Remove trailing slash for the picker
	if (CurrentPath.EndsWith(TEXT("/")))
	{
		CurrentPath = CurrentPath.LeftChop(1);
	}

	FPathPickerConfig PathPickerConfig;
	PathPickerConfig.DefaultPath = CurrentPath;
	PathPickerConfig.bAllowContextMenu = false;
	PathPickerConfig.bAllowClassesFolder = false;
	PathPickerConfig.bFocusSearchBoxWhenOpened = true;
	PathPickerConfig.OnPathSelected = FOnPathSelected::CreateLambda([PropertyHandle](const FString& SelectedPath)
	{
		FString PathWithSlash = SelectedPath;
		if (!PathWithSlash.EndsWith(TEXT("/")))
		{
			PathWithSlash += TEXT("/");
		}
		PropertyHandle->SetValue(PathWithSlash);
		FSlateApplication::Get().DismissAllMenus();
	});

	const TSharedRef<SWidget> PathPickerWidget = ContentBrowserModule.Get().CreatePathPicker(PathPickerConfig);

	FSlateApplication::Get().PushMenu(
		FSlateApplication::Get().GetActiveTopLevelWindow().ToSharedRef(),
		FWidgetPath(),
		SNew(SBox)
		.WidthOverride(300.0f)
		.HeightOverride(400.0f)
		[
			PathPickerWidget
		],
		FSlateApplication::Get().GetCursorPos(),
		FPopupTransitionEffect::ContextMenu
	);
}

#undef LOCTEXT_NAMESPACE
