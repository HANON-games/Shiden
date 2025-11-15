// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "Widgets/Input/SComboBox.h"

class FShidenCommandDefinitionCustomization final : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FShidenCommandDefinitionCustomization);
	}

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
	                             IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
	                               IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	TSharedPtr<IPropertyHandle> NoteHandle;
	TSharedPtr<IPropertyHandle> StyleHandle;
	TSharedPtr<IPropertyHandle> CanCallInMacroHandle;
	TSharedPtr<IPropertyHandle> CommandSoftObjectPathHandle;
	TSharedPtr<IPropertyHandle> ArgsHandle;

	static TSet<FTopLevelAssetPath> GetCachedDerivedClassNames();
};
