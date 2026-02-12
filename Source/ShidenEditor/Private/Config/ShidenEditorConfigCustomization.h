// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class FShidenEditorConfigCustomization : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	void OnBrowseDirectoryClicked() const;
	void OnDirectoryPathChanged(const FText& NewText) const;

	TSharedPtr<IPropertyHandle> DirectoryPathHandle;
};
