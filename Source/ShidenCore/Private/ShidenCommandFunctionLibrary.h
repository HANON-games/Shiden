// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenSubsystem.h"
#include "CoreMinimal.h"
#include "Engine/Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShidenCommandFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenCommandFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command")
	static void RegisterScenarioProperty(const FString CommandName, const FString Key, const FString Value);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command")
	static void RemoveScenarioProperty(const FString CommandName, const FString Key);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Command")
	static void ClearScenarioProperties(const FString CommandName);
};