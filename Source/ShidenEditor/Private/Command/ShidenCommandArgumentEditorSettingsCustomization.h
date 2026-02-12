// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"

class FShidenCommandArgumentEditorSettingsCustomization final : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance()
	{
		return MakeShareable(new FShidenCommandArgumentEditorSettingsCustomization);
	}

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> StructPropertyHandle, FDetailWidgetRow& HeaderRow,
	                             IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder,
	                               IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;

private:
	TSharedPtr<IPropertyHandle> DisplayNameHandle;
	TSharedPtr<IPropertyHandle> TemplateWidgetHandle;
	TSharedPtr<IPropertyHandle> TemplateParametersHandle;
	TSharedPtr<IPropertyHandle> VisibilityConditionHandle;
	TSharedPtr<IPropertyHandle> InformationMessagesHandle;
	TSharedPtr<IPropertyHandle> WarningMessagesHandle;
	TSharedPtr<IPropertyHandle> ErrorMessagesHandle;

	static const TSet<FString>& GetCachedDerivedClassNames();
};
