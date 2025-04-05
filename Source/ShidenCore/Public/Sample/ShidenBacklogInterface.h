// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Scenario/ShidenManagerInterface.h"
#include "UObject/Interface.h"
#include "ShidenBacklogInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenBacklogInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenBacklogInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void InitBacklog(const TScriptInterface<UShidenManagerInterface>& ShidenManager);
};
