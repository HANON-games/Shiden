// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "EditorUtilityWidget.h"
#include "Engine/EngineTypes.h"
#include "ShidenSvgWidget.generated.h"

UCLASS(BlueprintType)
class SHIDENEDITOR_API UShidenSvgWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, meta = (FilePathFilter = "svg"), Category = "SVG")
	FFilePath SvgFile;

	UPROPERTY(EditAnywhere, Category = "SVG")
	FVector2D IconSize = FVector2D(20.0f, 20.0f);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

private:
	TUniquePtr<FSlateVectorImageBrush> SvgBrush;
};
