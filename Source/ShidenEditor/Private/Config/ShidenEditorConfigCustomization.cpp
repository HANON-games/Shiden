// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Config/ShidenEditorConfigCustomization.h"
#include "Config/ShidenEditorConfig.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"
#include "Misc/Paths.h"
#include "Framework/Application/SlateApplication.h"
#include "Styling/AppStyle.h"

#define LOCTEXT_NAMESPACE "ShidenEditorConfigCustomization"

TSharedRef<IDetailCustomization> FShidenEditorConfigCustomization::MakeInstance()
{
	return MakeShareable(new FShidenEditorConfigCustomization);
}

void FShidenEditorConfigCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DirectoryPathHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UShidenEditorConfig, ScenarioSyncDirectoryPath));

	// Get the default property row and customize it
	if (IDetailPropertyRow* PropertyRow = DetailBuilder.EditDefaultProperty(DirectoryPathHandle))
	{
		PropertyRow->CustomWidget()
			.NameContent()
			[
				DirectoryPathHandle->CreatePropertyNameWidget()
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
					.Text_Lambda([this]
					{
						FString Value;
						DirectoryPathHandle->GetValue(Value);
						return FText::FromString(Value);
					})
					.OnTextCommitted_Lambda([this](const FText& NewText, ETextCommit::Type)
					{
						DirectoryPathHandle->SetValue(NewText.ToString());
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
					.OnClicked_Lambda([this]
					{
						OnBrowseDirectoryClicked();
						return FReply::Handled();
					})
					.ContentPadding(0)
					.ToolTipText(LOCTEXT("BrowseDirectoryTooltip", "Browse for a directory"))
					[
						SNew(SImage)
						.Image(FAppStyle::GetBrush("Icons.FolderOpen"))
						.ColorAndOpacity(FSlateColor::UseForeground())
					]
				]
			];
	}
}

void FShidenEditorConfigCustomization::OnBrowseDirectoryClicked() const
{
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (!DesktopPlatform)
	{
		return;
	}

	const void* ParentWindowHandle = nullptr;
	const TSharedPtr<SWindow> ActiveTopLevelWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
	if (ActiveTopLevelWindow.IsValid() && ActiveTopLevelWindow->GetNativeWindow().IsValid())
	{
		ParentWindowHandle = ActiveTopLevelWindow->GetNativeWindow()->GetOSWindowHandle();
	}

	const FString ProjectDir = FPaths::ProjectDir();

	FString SelectedPath;
	if (DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, TEXT("Select Directory"), ProjectDir, SelectedPath))
	{
		// Convert to relative path from project directory
		FString RelativePath = SelectedPath;
		FPaths::MakePathRelativeTo(RelativePath, *ProjectDir);

		DirectoryPathHandle->SetValue(RelativePath);
	}
}

void FShidenEditorConfigCustomization::OnDirectoryPathChanged(const FText& NewText) const
{
	DirectoryPathHandle->SetValue(NewText.ToString());
}

#undef LOCTEXT_NAMESPACE
