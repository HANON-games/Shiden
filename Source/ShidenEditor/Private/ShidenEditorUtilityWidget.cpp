// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorUtilityWidget.h"

UShidenEditorUtilityWidget::UShidenEditorUtilityWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UShidenEditorUtilityWidget::PostEditUndo()
{
	Super::PostEditUndo();

	ReceivePostUndo();
}
