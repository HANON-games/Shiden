// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/ShidenCommandArgumentEditorSettingsCustomization.h"
#include "System/ShidenStructuredLog.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Command/ShidenCommandDefinition.h"
#include "System/ShidenEditorConstants.h"
#include "ThumbnailRendering/ThumbnailManager.h"
#include "EditorUtilityWidgetBlueprint.h"

#define LOCTEXT_NAMESPACE "ShidenCommandArgumentEditorSettings"

void FShidenCommandArgumentEditorSettingsCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
                                                                        IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	DisplayNameHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, DisplayName));
	TemplateWidgetHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, TemplateWidget));
	TemplateParametersHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, TemplateParameters));
	VisibilityConditionHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, VisibilityCondition));
	InformationMessagesHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, InformationMessages));
	WarningMessagesHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, WarningMessages));
	ErrorMessagesHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandArgumentEditorSettings, ErrorMessages));

	// Validate that all required property handles were found
	if (!DisplayNameHandle.IsValid() || !TemplateWidgetHandle.IsValid() || !TemplateParametersHandle.IsValid() ||
		!VisibilityConditionHandle.IsValid() || !InformationMessagesHandle.IsValid() ||
		!WarningMessagesHandle.IsValid() || !ErrorMessagesHandle.IsValid())
	{
		SHIDEN_WARNING("Failed to find all required property handles for FShidenCommandArgumentEditorSettings");
	}
}

void FShidenCommandArgumentEditorSettingsCustomization::CustomizeChildren(const TSharedRef<IPropertyHandle> StructPropertyHandle,
                                                                          IDetailChildrenBuilder& StructBuilder,
                                                                          IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	// Add DisplayName property
	if (DisplayNameHandle.IsValid())
	{
		StructBuilder.AddProperty(DisplayNameHandle.ToSharedRef());
	}

	// Add TemplateWidget with custom filtering
	if (TemplateWidgetHandle.IsValid())
	{
		StructBuilder
			.AddCustomRow(ShidenEditorConstants::TemplateWidgetDisplayName)
			.NameContent()
			[
				TemplateWidgetHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SObjectPropertyEntryBox)
				.PropertyHandle(TemplateWidgetHandle)
				.ThumbnailPool(UThumbnailManager::Get().GetSharedThumbnailPool())
				.OnShouldFilterAsset(FOnShouldFilterAsset::CreateLambda([](const FAssetData& AssetData)
				{
					const FString ObjectPath = AssetData.GetObjectPathString();
					if (ObjectPath.StartsWith(ShidenEditorConstants::EnginePathPrefix))
					{
						return true;
					}

					const FString GeneratedClass = AssetData.GetTagValueRef<FString>("GeneratedClass");
					if (GeneratedClass.IsEmpty())
					{
						return true;
					}

					// Check if the generated class is abstract
					if (const TObjectPtr<const UClass> Class = FindObject<UClass>(nullptr, *GeneratedClass))
					{
						if (Class->HasAnyClassFlags(CLASS_Abstract))
						{
							return true;
						}
					}

					return !GetCachedDerivedClassNames().Contains(GeneratedClass);
				}))
			];
	}

	// Add remaining properties
	if (TemplateParametersHandle.IsValid())
	{
		StructBuilder.AddProperty(TemplateParametersHandle.ToSharedRef());
	}

	if (VisibilityConditionHandle.IsValid())
	{
		StructBuilder.AddProperty(VisibilityConditionHandle.ToSharedRef());
	}

	if (InformationMessagesHandle.IsValid())
	{
		StructBuilder.AddProperty(InformationMessagesHandle.ToSharedRef());
	}

	if (WarningMessagesHandle.IsValid())
	{
		StructBuilder.AddProperty(WarningMessagesHandle.ToSharedRef());
	}

	if (ErrorMessagesHandle.IsValid())
	{
		StructBuilder.AddProperty(ErrorMessagesHandle.ToSharedRef());
	}
}

const TSet<FString>& FShidenCommandArgumentEditorSettingsCustomization::GetCachedDerivedClassNames()
{
	static TSet<FString> DerivedClassNames;
	if (DerivedClassNames.Num() > 0)
	{
		return DerivedClassNames;
	}

	const IAssetRegistry* Registry = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(ShidenEditorConstants::AssetRegistryModuleName).Get();

	FARFilter Filter;
	Filter.ClassPaths.Add(UEditorUtilityWidgetBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;

	TArray<FAssetData> WidgetAssets;
	Registry->GetAssets(Filter, WidgetAssets);

	// Find ShidenInputBase and get its GeneratedClass
	FString InputBaseGeneratedClass;
	for (const FAssetData& Asset : WidgetAssets)
	{
		if (Asset.AssetName.ToString() == ShidenEditorConstants::ShidenInputBaseName)
		{
			InputBaseGeneratedClass = Asset.GetTagValueRef<FString>("GeneratedClass");
			break;
		}
	}

	if (InputBaseGeneratedClass.IsEmpty())
	{
		return DerivedClassNames;
	}

	// Build a map of GeneratedClass -> ParentClassPath for efficient lookup
	TMap<FString, FString> ClassToParentMap;
	for (const FAssetData& Asset : WidgetAssets)
	{
		const FString GeneratedClass = Asset.GetTagValueRef<FString>("GeneratedClass");
		const FString ParentClass = Asset.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
		if (!GeneratedClass.IsEmpty() && !ParentClass.IsEmpty())
		{
			ClassToParentMap.Add(GeneratedClass, ParentClass);
		}
	}

	// Find all classes that derive from InputBase (recursively)
	for (const TPair<FString, FString>& Pair : ClassToParentMap)
	{
		FString CurrentParent = Pair.Value;
		while (!CurrentParent.IsEmpty())
		{
			if (CurrentParent == InputBaseGeneratedClass)
			{
				DerivedClassNames.Add(Pair.Key);
				break;
			}

			// Move up the hierarchy
			const FString* NextParent = ClassToParentMap.Find(CurrentParent);
			CurrentParent = NextParent ? *NextParent : FString();
		}
	}

	return DerivedClassNames;
}

#undef LOCTEXT_NAMESPACE
