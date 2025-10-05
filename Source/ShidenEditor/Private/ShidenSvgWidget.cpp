// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenSvgWidget.h"
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
