// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenMainMenu.generated.h"

UCLASS()
class SHIDENEDITOR_API UShidenMainMenu : public UToolMenuEntryScript
{
	GENERATED_BODY()

public:
	static void RegisterMenuExtensions(IModuleInterface *Owner);
};
