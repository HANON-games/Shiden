// Copyright (c) 2026 HANON. All Rights Reserved.

#include "ShidenK2Node.h"
#include "ShidenCustomGraphPinFactory.h"
#include "ShidenGetCommandArgumentsNodeDetails.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "FShidenK2NodeModule"

void FShidenK2NodeModule::StartupModule()
{
	const TSharedPtr<FShidenCustomGraphPanelPinFactory> CGraphPinFactory = MakeShareable(new FShidenCustomGraphPanelPinFactory());
	FEdGraphUtilities::RegisterVisualPinFactory(CGraphPinFactory);

	FPropertyEditorModule& PropertyEditorModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	PropertyEditorModule.RegisterCustomClassLayout(
		"K2Node_GetCommandArguments",
		FOnGetDetailCustomizationInstance::CreateStatic(&FShidenGetCommandArgumentsNodeDetails::MakeInstance));
	PropertyEditorModule.NotifyCustomizationModuleChanged();
}

void FShidenK2NodeModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyEditorModule.UnregisterCustomClassLayout("K2Node_GetCommandArguments");
		PropertyEditorModule.NotifyCustomizationModuleChanged();
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShidenK2NodeModule, ShidenK2Node)
