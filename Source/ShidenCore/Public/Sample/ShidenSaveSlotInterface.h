// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenSaveSlotInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenSaveSlotInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenSaveSlotInterface
{
	GENERATED_BODY()

public:
	/**
	 * Initializes a save slot widget with save game data and preview information.
	 * @param SlotName The internal name identifier for this save slot
	 * @param DisplayName The user-friendly display name for this save slot
	 * @param SaveMenuWidget The parent save menu widget that contains this slot
	 * @param bIsSaveMode True if this is a save operation, false if it's a load operation
	 * @param Thumbnail The screenshot thumbnail image for this save
	 * @param UpdatedAtText The formatted timestamp when this save was last updated
	 * @param NameText The character name text at the time of save
	 * @param TalkText The dialogue text preview at the time of save
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget",
		meta = (AutoCreateRefTerm = "DisplayName,UpdatedAtText,NameText,TalkText"))
	void InitSaveSlot(const FString& SlotName, const FText& DisplayName, const UUserWidget* SaveMenuWidget,
	                  const bool bIsSaveMode, const UTexture2D* Thumbnail, const FText& UpdatedAtText,
	                  const FText& NameText, const FText& TalkText);

	/**
	 * Initializes an empty save slot widget when no save data exists.
	 * @param SlotName The internal name identifier for this save slot
	 * @param DisplayName The user-friendly display name for this save slot
	 * @param SaveMenuWidget The parent save menu widget that contains this slot
	 * @param bIsSaveMode True if this is a save operation, false if it's a load operation
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "DisplayName"))
	void InitNoDataSaveSlot(const FString& SlotName, const FText& DisplayName, const UUserWidget* SaveMenuWidget, const bool bIsSaveMode);
};
