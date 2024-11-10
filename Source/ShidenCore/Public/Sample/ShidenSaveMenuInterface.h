// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenSaveMenuInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenSaveMenuInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnSaveSlotSelectedDelegate, const FString&, SlotName);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoadSlotSelectedDelegate, const FString&, SlotName);

class SHIDENCORE_API IShidenSaveMenuInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnSaveCompleted"))
	void InitSaveMenu(const FOnSaveSlotSelectedDelegate& OnSaveSlotSelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnLoadCompleted"))
	void InitLoadMenu(const FOnLoadSlotSelectedDelegate& OnLoadSlotSelected);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SaveSlot(const FString& SlotName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void LoadSlot(const FString& SlotName);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void DeleteSlot(const FString& SlotName);
};
