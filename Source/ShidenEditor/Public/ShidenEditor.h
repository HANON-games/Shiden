// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class SHIDENEDITOR_API FShidenEditorModule final : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	void RegisterMenuExtensions();
	virtual void ShutdownModule() override;

private:
	static void InitializeScenarioPaths();
};
