// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class SHIDENCORE_API FShidenCoreModule final : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};
