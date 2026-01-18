// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Scenario/ShidenScenarioCustomization.h"
#include "DetailWidgetRow.h"
#include "Scenario/ShidenScenario.h"
#include "Config/ShidenProjectConfig.h"
#include "System/ShidenEditorBlueprintLibrary.h"
#include "Config/ShidenEditorConfig.h"
#include "Widgets/Input/SButton.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Subsystems/EditorAssetSubsystem.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "ShidenScenarioCustomization"

void FShidenScenarioCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow,
                                                   IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ScenarioPropertyHandle = PropertyHandle;

	HeaderRow
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				PropertyHandle->CreatePropertyValueWidget()
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(0, 4, 0, 0)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(0, 0, 4, 0)
				[
					SNew(SButton)
					.Text(LOCTEXT("NewScenarioButton", "New Scenario"))
					.OnClicked_Lambda([this]() -> FReply
					{
						OnNewScenarioClicked();
						return FReply::Handled();
					})
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SButton)
					.Text(LOCTEXT("OpenInEditorButton", "Open in Editor"))
					.IsEnabled_Lambda([this]() -> bool
					{
						if (ScenarioPropertyHandle.IsValid())
						{
							UObject* Object = nullptr;
							return ScenarioPropertyHandle->GetValue(Object) == FPropertyAccess::Success && Object != nullptr;
						}
						return false;
					})
					.OnClicked_Lambda([this]() -> FReply
					{
						OnOpenInEditorClicked();
						return FReply::Handled();
					})
				]
			]
		];
}

void FShidenScenarioCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder,
                                                     IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

void FShidenScenarioCustomization::OnOpenInEditorClicked() const
{
	if (!ScenarioPropertyHandle.IsValid())
	{
		return;
	}

	UObject* Object = nullptr;
	if (ScenarioPropertyHandle->GetValue(Object) != FPropertyAccess::Success || Object == nullptr)
	{
		return;
	}

	const TObjectPtr<const UShidenScenario> Scenario = Cast<UShidenScenario>(Object);
	if (!Scenario)
	{
		return;
	}

	UShidenEditorConfig::SetEditScenarioPath(Scenario->GetPackage()->GetName());

	const TObjectPtr<UEditorUtilitySubsystem> EditorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	check(EditorUtilitySubsystem);

	const FString EditorWidgetPath = TEXT("/Shiden/Editor/Core/EUW_ShidenVisualNovelEditor.EUW_ShidenVisualNovelEditor");

	const TObjectPtr<UEditorUtilityWidgetBlueprint> EditorWidgetBlueprint = LoadObject<UEditorUtilityWidgetBlueprint>(nullptr, *EditorWidgetPath);
	if (!EditorWidgetBlueprint)
	{
		return;
	}

	if (const TObjectPtr<UEditorUtilityWidget> EditorWidget = EditorUtilitySubsystem->FindUtilityWidgetFromBlueprint(EditorWidgetBlueprint); !EditorWidget)
	{
		EditorUtilitySubsystem->SpawnAndRegisterTab(EditorWidgetBlueprint);
	}
}

void FShidenScenarioCustomization::OnNewScenarioClicked() const
{
	if (!ScenarioPropertyHandle.IsValid())
	{
		return;
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	FString SavePackagePath, SaveAssetName;
	bool bAssetNameWasSet = false;

	UShidenEditorBlueprintLibrary::ShowSaveAssetDialog(
		UShidenScenario::StaticClass(),
		ProjectConfig->ScenarioDirectoryPath,
		TEXT("NewScenario"),
		SavePackagePath,
		SaveAssetName,
		bAssetNameWasSet
	);

	if (!bAssetNameWasSet)
	{
		return;
	}

	const FString PackageName = SavePackagePath + TEXT("/") + SaveAssetName;
	if (const TObjectPtr<UPackage> Package = CreatePackage(*PackageName))
	{
		if (const TObjectPtr<UShidenScenario> NewScenario = NewObject<UShidenScenario>(Package, UShidenScenario::StaticClass(), *SaveAssetName,
		                                                                               RF_Public | RF_Standalone))
		{
			FAssetRegistryModule::AssetCreated(NewScenario);
			// ReSharper disable once CppExpressionWithoutSideEffects
			Package->MarkPackageDirty();

			if (const TObjectPtr<UEditorAssetSubsystem> EditorAssetSubsystem = GEditor->GetEditorSubsystem<UEditorAssetSubsystem>())
			{
				if (EditorAssetSubsystem->SaveAsset(PackageName))
				{
					UShidenEditorConfig::SetEditScenarioPath(PackageName);
					ScenarioPropertyHandle->SetValue(NewScenario);
				}
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
