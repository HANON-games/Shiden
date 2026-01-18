// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "UI/ShidenWidget.h"
#include "ShidenParallelProcessManagerInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UShidenParallelProcessManagerInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenParallelProcessManagerInterface
{
	GENERATED_BODY()

public:
	/**
	 * Starts processing a macro as a parallel process.
	 * @param NewProcessName The name of the new parallel process
	 * @param ShidenWidget The Shiden widget
	 * @param ShidenManager The Shiden manager interface
	 * @param CallerObject The object that initiated the macro call
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Scenario")
	void StartProcessingMacro(const FString& NewProcessName, UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager, UObject* CallerObject);
};
