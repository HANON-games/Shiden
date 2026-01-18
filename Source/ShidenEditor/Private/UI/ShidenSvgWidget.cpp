// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenSvgWidget.h"
#include "Misc/Paths.h"
#include "Widgets/Images/SImage.h"

TSharedRef<SWidget> UShidenSvgWidget::RebuildWidget()
{
	SvgBrush.Reset();
	if (FPaths::FileExists(SvgFile.FilePath))
	{
		SvgBrush = MakeUnique<FSlateVectorImageBrush>(SvgFile.FilePath, IconSize);
	}

	return SNew(SImage)
		.Image(SvgBrush.Get())
		.Visibility(SvgBrush ? EVisibility::Visible : EVisibility::Collapsed);
}

const FText UShidenSvgWidget::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}
