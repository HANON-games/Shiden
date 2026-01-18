// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Command/ShidenCommandDefinitionCustomization.h"
#include "System/ShidenStructuredLog.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Command/ShidenCommandDefinition.h"
#include "Command/ShidenCommandObject.h"
#include "System/ShidenEditorConstants.h"
#include "ThumbnailRendering/ThumbnailManager.h"

#define LOCTEXT_NAMESPACE "ShidenCommandDefinition"

void FShidenCommandDefinitionCustomization::CustomizeHeader(const TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
                                                            IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	NoteHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandDefinition, Note));
	StyleHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandDefinition, Style));
	CanCallInMacroHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandDefinition, bCanCallInMacro));
	CommandSoftObjectPathHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandDefinition, CommandSoftObjectPath));
	ArgsHandle = StructPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FShidenCommandDefinition, Args));

	// Validate that all required property handles were found
	if (!NoteHandle.IsValid() || !StyleHandle.IsValid() || !CanCallInMacroHandle.IsValid() ||
		!CommandSoftObjectPathHandle.IsValid() || !ArgsHandle.IsValid())
	{
		SHIDEN_WARNING("Failed to find all required property handles for FShidenCommandDefinition");
	}
}

void FShidenCommandDefinitionCustomization::CustomizeChildren(const TSharedRef<IPropertyHandle> StructPropertyHandle,
                                                              IDetailChildrenBuilder& StructBuilder,
                                                              IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	// Add properties with null checks
	if (NoteHandle.IsValid())
	{
		StructBuilder.AddProperty(NoteHandle.ToSharedRef());
	}

	if (StyleHandle.IsValid())
	{
		StructBuilder.AddProperty(StyleHandle.ToSharedRef());
	}

	if (CanCallInMacroHandle.IsValid())
	{
		StructBuilder.AddProperty(CanCallInMacroHandle.ToSharedRef());
	}

	// Add CommandSoftObjectPath with custom UI if handle is valid
	if (CommandSoftObjectPathHandle.IsValid())
	{
		StructBuilder
			.AddCustomRow(ShidenEditorConstants::CommandSoftObjectPathDisplayName)
			.NameContent()
			[
				CommandSoftObjectPathHandle->CreatePropertyNameWidget()
			]
			.ValueContent()
			[
				SNew(SObjectPropertyEntryBox)
				.PropertyHandle(CommandSoftObjectPathHandle)
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
						// For non-Blueprint assets, check if it's abstract
						if (const TObjectPtr<const UClass> Class = FindObject<UClass>(nullptr, *ObjectPath))
						{
							if (Class->HasAnyClassFlags(CLASS_Abstract))
							{
								return true;
							}
						}
						return !GetCachedDerivedClassNames().Contains(FTopLevelAssetPath(ObjectPath));
					}

					// Check if the generated class is abstract
					if (const TObjectPtr<const UClass> Class = FindObject<UClass>(nullptr, *GeneratedClass))
					{
						if (Class->HasAnyClassFlags(CLASS_Abstract))
						{
							return true;
						}
					}

					const FString ParentClassName = AssetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
					if (ParentClassName == ShidenEditorConstants::ShidenCommandObjectClassName)
					{
						return false;
					}

					if (ParentClassName.IsEmpty())
					{
						return true;
					}

					if (TObjectPtr<const UClass> ParentClass = FindObject<UClass>(nullptr, *ParentClassName))
					{
						ParentClass = ParentClass->GetSuperClass();
						while (ParentClass)
						{
							if (ParentClass->GetName() == ShidenEditorConstants::ShidenCommandObjectName)
							{
								return false;
							}
							ParentClass = ParentClass->GetSuperClass();
						}
					}
					return true;
				}))
			];
	}

	// Add Args property if handle is valid
	if (ArgsHandle.IsValid())
	{
		StructBuilder.AddProperty(ArgsHandle.ToSharedRef());
	}
}

TSet<FTopLevelAssetPath> FShidenCommandDefinitionCustomization::GetCachedDerivedClassNames()
{
	static TSet<FTopLevelAssetPath> DerivedClassNames;
	if (DerivedClassNames.Num() > 0)
	{
		return DerivedClassNames;
	}
	const IAssetRegistry* Registry = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>(ShidenEditorConstants::AssetRegistryModuleName).Get();
	Registry->GetDerivedClassNames(TArray{UShidenCommandObject::StaticClass()->GetClassPathName()}, TSet<FTopLevelAssetPath>(), DerivedClassNames);
	return DerivedClassNames;
}

#undef LOCTEXT_NAMESPACE
