// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Command/ShidenCommandObject.h"
#include "Scenario/ShidenPlayScenarioStatus.h"
#include "UObject/Interface.h"
#include "UI/ShidenWidget.h"
#include "Scenario/ShidenManagerInterface.h"
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
	void OnPrePlayScenario(const UShidenWidget* ShidenWidget, const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostPlayScenario(const EShidenPlayScenarioStatus Status, const FString& NextScenarioName,
	                        const FString& BreakReason, const FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPrePreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                            const TScriptInterface<IShidenManagerInterface>& ShidenManager);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostPreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                             const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                             const EShidenPreProcessStatus Status, const FString& ErrorMessage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPreProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                         const float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Hook")
	void OnPostProcessCommand(const FString& ProcessName, const FShidenCommand Command, const UShidenWidget* ShidenWidget,
	                          const TScriptInterface<IShidenManagerInterface>& ShidenManager,
	                          const float DeltaTime, const EShidenProcessStatus Status, const FString& NextScenarioName,
	                          const FString& BreakReason, const FString& ErrorMessage);
};
