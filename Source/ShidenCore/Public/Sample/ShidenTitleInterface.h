// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenTitleInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnNewGameDelegate);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoadGameDelegate, const FString&, SlotName);

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenTitleInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenTitleInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnNewGame,OnLoadGame"))
	void InitTitle(const FOnNewGameDelegate& OnNewGame, const FOnLoadGameDelegate& OnLoadGame);
};
