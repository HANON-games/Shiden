// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenEditorUtilityWidget.h"

UShidenEditorUtilityWidget::UShidenEditorUtilityWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShidenEditorUtilityWidget::PostEditUndo()
{
	Super::PostEditUndo();

	ReceivePostUndo();
}

const FText UShidenEditorUtilityWidget::GetPaletteCategory()
{
	return NSLOCTEXT("ShidenNamespace", "Shiden Editor", "Shiden Editor");
}
