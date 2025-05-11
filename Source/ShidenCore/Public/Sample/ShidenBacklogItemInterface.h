// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Scenario/ShidenManagerInterface.h"
#include "UObject/Interface.h"
#include "ShidenBacklogItemInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenBacklogItemInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenBacklogItemInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "Name,Text,VoicePath"))
	void InitBacklogItem(const TScriptInterface<UShidenManagerInterface>& ShidenManager, const FString& Name, const FString& Text,
	                     const FString& VoicePath);
};
