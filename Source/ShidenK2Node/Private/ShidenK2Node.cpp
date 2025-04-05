// Copyright (c) 2025 HANON. All Rights Reserved.

#include "ShidenK2Node.h"
#include "ShidenCustomGraphPinFactory.h"

#define LOCTEXT_NAMESPACE "FShidenK2NodeModule"

void FShidenK2NodeModule::StartupModule()
{
	const TSharedPtr<FShidenCustomGraphPanelPinFactory> CGraphPinFactory = MakeShareable(new FShidenCustomGraphPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(CGraphPinFactory);
}

void FShidenK2NodeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShidenK2NodeModule, ShidenK2Node)
