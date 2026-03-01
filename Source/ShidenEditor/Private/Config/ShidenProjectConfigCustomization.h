// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FShidenProjectConfigCustomization final : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FShidenProjectConfigCustomization);
	}

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	static void OnExportCommandDefinitionsJsonClicked();

	static void AddContentBrowserPathProperty(IDetailLayoutBuilder& DetailBuilder, const FName& PropertyName, const FText& DisplayName);

	static void OnBrowseContentPathClicked(TSharedPtr<IPropertyHandle> PropertyHandle);

	TSharedPtr<IPropertyHandle> ScenarioDirectoryPathHandle;
	TSharedPtr<IPropertyHandle> MacroDirectoryPathHandle;
};
