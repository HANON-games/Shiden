// Copyright (c) 2026 HANON. All Rights Reserved.

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
	/**
	 * Initializes the save menu widget with a callback delegate for save slot selection.
	 * @param OnSaveSlotSelected Delegate called when a save slot is selected for saving
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnSaveCompleted"))
	void InitSaveMenu(const FOnSaveSlotSelectedDelegate& OnSaveSlotSelected);

	/**
	 * Initializes the load menu widget with a callback delegate for load slot selection.
	 * @param OnLoadSlotSelected Delegate called when a save slot is selected for loading
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnLoadCompleted"))
	void InitLoadMenu(const FOnLoadSlotSelectedDelegate& OnLoadSlotSelected);

	/**
	 * Saves the current game state to the specified save slot.
	 * @param SlotName The name of the save slot to save to
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SaveToSlot(const FString& SlotName);

	/**
	 * Loads game state from the specified save slot.
	 * @param SlotName The name of the save slot to load from
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void LoadFromSlot(const FString& SlotName);

	/**
	 * Deletes the save data in the specified save slot.
	 * @param SlotName The name of the save slot to delete
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void DeleteSlot(const FString& SlotName);
};
