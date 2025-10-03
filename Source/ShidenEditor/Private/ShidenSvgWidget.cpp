// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenSvgWidget.h"
#include "Misc/Paths.h"
#include "Widgets/Images/SImage.h"

TSharedRef<SWidget> UShidenSvgWidget::RebuildWidget()
{
	const FVector2D IconSize(20.f, 20.f);

	SvgBrush.Reset();
	if (FPaths::FileExists(SvgFile.FilePath))
	{
		SvgBrush = MakeUnique<FSlateVectorImageBrush>(SvgFile.FilePath, IconSize);
	}

	return SNew(SImage)
		.Image(SvgBrush.Get())
		.Visibility(SvgBrush ? EVisibility::Visible : EVisibility::Collapsed);
}
