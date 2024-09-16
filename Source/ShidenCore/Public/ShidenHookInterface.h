// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenWidget.h"
#include "ShidenScenarioManagerInterface.h"
#include "ShidenHookInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UShidenHookInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenHookInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostInit(const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPrePreProcess(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostPreProcess(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const EShidenPreProcessStatus Status);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPreProcess(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostProcess(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* Widget, const TScriptInterface <IShidenScenarioManagerInterface>& ScenarioManager, const float DeltaTime, const EShidenProcessStatus Status);
};
