// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenEditorStartup.h"
#include "EditorUtilityObject.h"
#include "EditorUtilitySubsystem.h"
#include "Editor/EditorEngine.h"

#define LOCTEXT_NAMESPACE "FShidenEditorStartupModule"

SHIDENEDITORSTARTUP_API void FShidenEditorStartupModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	if (GEditor)
	{
		TObjectPtr <UObject> Object = LoadObject<UObject>(nullptr, TEXT("/Shiden/Editor/Utilities/EUB_ShidenStartup.EUB_ShidenStartup"));
		GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>()->TryRun(Object);
	}
}

SHIDENEDITORSTARTUP_API void FShidenEditorStartupModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FShidenEditorStartupModule, ShidenEditorStartup)
