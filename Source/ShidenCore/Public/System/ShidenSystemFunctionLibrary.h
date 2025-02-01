// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShidenSystemFunctionLibrary.generated.h"

UCLASS()
class SHIDENCORE_API UShidenSystemFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Shiden Visual Novel|System")
	static bool IsAutoTextMode();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|System")
	static void SetAutoTextMode(bool bMode);
};
