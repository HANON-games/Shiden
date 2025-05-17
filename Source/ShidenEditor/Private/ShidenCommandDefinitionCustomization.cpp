#include "ShidenCommandDefinitionCustomization.h"
#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Command/ShidenCommandDefinition.h"
#include "Command/ShidenCommandObject.h"
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
}

void FShidenCommandDefinitionCustomization::CustomizeChildren(const TSharedRef<IPropertyHandle> StructPropertyHandle,
                                                              IDetailChildrenBuilder& StructBuilder,
                                                              IPropertyTypeCustomizationUtils& StructCustomizationUtils)
{
	StructBuilder.AddProperty(NoteHandle.ToSharedRef());
	StructBuilder.AddProperty(StyleHandle.ToSharedRef());
	StructBuilder.AddProperty(CanCallInMacroHandle.ToSharedRef());

	StructBuilder
		.AddCustomRow(FText::FromString(TEXT("Command Soft Object Path")))
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
				if (ObjectPath.StartsWith(TEXT("/Engine/")))
				{
					return true;
				}
				if (AssetData.GetTagValueRef<FString>("GeneratedClass").IsEmpty())
				{
					return !GetDerivedClassNames().Contains(FTopLevelAssetPath(ObjectPath));
				}
				const FString ParentClassName = AssetData.GetTagValueRef<FString>(FBlueprintTags::ParentClassPath);
				if (ParentClassName == TEXT("/Script/CoreUObject.Class'/Script/ShidenCore.ShidenCommandObject'"))
				{
					return false;
				}
				if (ParentClassName.IsEmpty())
				{
					return true;
				}
				if (const UClass* ParentClass = FindObject<UClass>(nullptr, *ParentClassName))
				{
					ParentClass = ParentClass->GetSuperClass();
					while (ParentClass)
					{
						if (ParentClass->GetName() == TEXT("ShidenCommandObject"))
						{
							return false;
						}
						ParentClass = ParentClass->GetSuperClass();
					}
				}
				return true;
			}))
		];

	StructBuilder.AddProperty(ArgsHandle.ToSharedRef());
}

TSet<FTopLevelAssetPath> FShidenCommandDefinitionCustomization::GetDerivedClassNames()
{
	static TSet<FTopLevelAssetPath> DerivedClassNames;
	if (DerivedClassNames.Num() > 0)
	{
		return DerivedClassNames;
	}
	const IAssetRegistry* Registry = &FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
	Registry->GetDerivedClassNames(TArray{UShidenCommandObject::StaticClass()->GetClassPathName()}, TSet<FTopLevelAssetPath>(), DerivedClassNames);
	return DerivedClassNames;
}

#undef LOCTEXT_NAMESPACE
