// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenSaveSlotsSaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShidenSaveBlueprintLibrary.generated.h"

using namespace UE::Tasks;

DECLARE_DELEGATE_OneParam(FOnSaveCompletedDelegate, bool)

UCLASS()
class SHIDENCORE_API UShidenSaveBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static FPipe SaveGamePipe;

	static void WaitUntilEmpty();

	static bool DoesSaveSlotsExist();

public:
	/**
	 * Converts a FShidenSaveTexture to a UTexture2D object.
	 * 
	 * @param SaveTexture The save texture data to convert
	 * @return The converted UTexture2D object
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static UTexture2D* ConvertSaveTextureToTexture2D(const FShidenSaveTexture& SaveTexture);

	/**
	 * Saves user data to a specified save slot.
	 * 
	 * @param SlotName The name of the save slot to save to
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (AutoCreateRefTerm = "SlotMetadata", DisplayName = "Save User Data"))
	static UPARAM(DisplayName = "Success") bool TrySaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata);

	/**
	 * Saves system data.
	 * 
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Save System Data"))
	static UPARAM(DisplayName = "Success") bool TrySaveSystemData();

	/**
	 * Saves predefined system data.
	 * 
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Save Predefined System Data"))
	static UPARAM(DisplayName = "Success") bool TrySavePredefinedSystemData();

	/**
	 * Asynchronously saves user data to a specified save slot.
	 * 
	 * @param SlotName The name of the save slot to save to
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @param SavedDelegate Delegate called when the save operation completes
	 */
	static void AsyncSaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata,
	                              FOnSaveCompletedDelegate SavedDelegate = FOnSaveCompletedDelegate());

	/**
	 * Asynchronously saves system data.
	 * 
	 * @param SavedDelegate Delegate called when the save operation completes
	 */
	static void AsyncSaveSystemData(FOnSaveCompletedDelegate SavedDelegate = FOnSaveCompletedDelegate());

	/**
	 * Asynchronously saves predefined system data.
	 * 
	 * @param SavedDelegate Delegate called when the save operation completes
	 */
	static void AsyncSavePredefinedSystemData(FOnSaveCompletedDelegate SavedDelegate = FOnSaveCompletedDelegate());

	/**
	 * Gets a reference to all available save slots.
	 * 
	 * @return Reference to a map of save slots, where the key is the slot name
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static TMap<FString, FShidenSaveSlot>& AcquireSaveSlots();

	/**
	 * Loads user data from a specified save slot.
	 * 
	 * @param SlotName The name of the save slot to load from
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadUserData(const FString& SlotName);

	/**
	 * Gets the name of the most recently used save slot.
	 * 
	 * @param SlotName [out] The name of the most recent save slot
	 * @return True if a save slot was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Get Latest User Save Slot Name"))
	static UPARAM(DisplayName = "Success") bool TryGetLatestUserSaveSlotName(FString& SlotName);

	/**
	 * Loads system data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadSystemData();

	/**
	 * Loads predefined system data.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (WorldContext = "WorldContextObject"))
	static void LoadPredefinedSystemData(const UObject* WorldContextObject);

	/**
	 * Deletes user data from a specified save slot.
	 * 
	 * @param SlotName The name of the save slot to delete
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeleteUserData(const FString& SlotName);

	/**
	 * Deletes system data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeleteSystemData();

	/**
	 * Deletes predefined system data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeletePredefinedSystemData();

	/**
	 * Checks if any user save data exists.
	 * 
	 * @return True if at least one save slot contains data
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesAnyUserDataExist();

	/**
	 * Checks if user data exists in a specified save slot.
	 * 
	 * @param SlotName The name of the save slot to check
	 * @return True if the specified save slot contains data
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesUserDataExist(const FString& SlotName);

	/**
	 * Checks if system data exists.
	 * 
	 * @return True if system data exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesSystemDataExist();

	/**
	 * Checks if predefined system data exists.
	 * 
	 * @return True if predefined system data exists
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesPredefinedSystemDataExist();

	/**
	 * Clears all loaded system data from memory.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void ClearLoadedSystemData();

	/**
	 * Clears all loaded predefined system data from memory.
	 * 
	 * @param WorldContextObject Object that provides context for the world
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (WorldContext = "WorldContextObject"))
	static void ClearLoadedPredefinedSystemData(const UObject* WorldContextObject);

	/**
	 * Clears all loaded user data from memory.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void ClearLoadedUserData();
};
