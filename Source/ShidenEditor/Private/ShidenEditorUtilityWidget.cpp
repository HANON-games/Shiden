// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorUtilityWidget.h"

SHIDENEDITOR_API UShidenEditorUtilityWidget::UShidenEditorUtilityWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShidenEditorUtilityWidget::PostEditUndo()
{
	Super::PostEditUndo();

	UShidenEditorUtilityWidget::ReceivePostUndo();
}
