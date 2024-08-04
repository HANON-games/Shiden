// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorConfig.h"

SHIDENEDITOR_API UShidenEditorConfig::UShidenEditorConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EditScenarioPath("")
	, bPreviewSound(false)
	, ShidenDebuggerRefreshInterval(0.1f)
	, bAutoSaveScenario(false)
{
	PreviewWidget.SetPath(TEXT("/Shiden/Samples/WBP_ShidenScenarioWidgetSample.WBP_ShidenScenarioWidgetSample"));
}