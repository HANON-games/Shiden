// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenSaveSlotsSaveGame.h"
#include "ShidenUserSaveGame.h"
#include "ShidenSystemSaveGame.h"
#include "ShidenPredefinedSystemSaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShidenSaveBlueprintLibrary.generated.h"

DECLARE_DELEGATE_OneParam(FOnSaveCompletedDelegate, bool)
DECLARE_DELEGATE_OneParam(FOnLoadCompletedDelegate, bool)
DECLARE_DELEGATE_TwoParams(FOnRetrieveUserDataCompletedDelegate, bool, UShidenUserSaveGame*)
DECLARE_DELEGATE_TwoParams(FOnRetrieveSystemDataCompletedDelegate, bool, UShidenSystemSaveGame*)
DECLARE_DELEGATE_TwoParams(FOnRetrievePredefinedSystemDataCompletedDelegate, bool, UShidenPredefinedSystemSaveGame*)

UCLASS()
class SHIDENCORE_API UShidenSaveBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static UE::Tasks::FPipe SaveGamePipe;

	static void WaitUntilEmpty();

	static bool DoSaveSlotsExist();

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
	 * Saves user data to a specified save slot. This operation includes both preparation and committing the data.
	 * 
	 * @param SlotName The name of the save slot to save to
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (AutoCreateRefTerm = "SlotMetadata", DisplayName = "Save User Data (Prepare & Commit)"))
	static UPARAM(DisplayName = "Success") bool TrySaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata);

	/**
	 * Saves system data. This operation includes both preparation and committing the data.
	 * 
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Save System Data (Prepare & Commit)"))
	static UPARAM(DisplayName = "Success") bool TrySaveSystemData();

	/**
	 * Saves predefined system data.
	 *
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Save Predefined System Data"))
	static UPARAM(DisplayName = "Success") bool TrySavePredefinedSystemData();

	/**
	 * Prepares a user save game instance with current subsystem state.
	 * Use this when you need to customize the save game before committing.
	 *
	 * @param SlotName The name of the save slot
	 * @return The prepared save game instance with Prepare() already called
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced")
	static UShidenUserSaveGame* PrepareUserData(const FString& SlotName);

	/**
	 * Commits a prepared user save game to storage.
	 *
	 * @param SaveGame The save game instance to commit
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced", meta = (AutoCreateRefTerm = "SlotMetadata"))
	static UPARAM(DisplayName = "Success") bool TryCommitUserData(UShidenUserSaveGame* SaveGame, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata);

	/**
	 * Prepares a system save game instance with current subsystem state.
	 * Use this when you need to customize the save game before committing.
	 *
	 * @return The prepared save game instance with Prepare() already called
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced")
	static UShidenSystemSaveGame* PrepareSystemData();

	/**
	 * Commits a prepared system save game to storage.
	 *
	 * @param SaveGame The save game instance to commit
	 * @return True if the save operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced")
	static UPARAM(DisplayName = "Success") bool TryCommitSystemData(UShidenSystemSaveGame* SaveGame);

	/**
	 * Asynchronously saves user data to a specified save slot. This operation includes both preparation and committing the data.
	 *
	 * @param SlotName The name of the save slot to save to
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @param SavedDelegate Delegate called when the save operation completes
	 */
	static void AsyncSaveUserData(const FString& SlotName, TObjectPtr<UTexture2D> Thumbnail, const TMap<FString, FString>& SlotMetadata,
	                              FOnSaveCompletedDelegate SavedDelegate = FOnSaveCompletedDelegate());

	/**
	 * Asynchronously saves system data. This operation includes both preparation and committing the data.
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
	 * Asynchronously commits a prepared user save game to storage.
	 *
	 * @param SaveGame The save game instance to commit
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @param SavedDelegate Delegate called when the save operation completes
	 */
	static void AsyncCommitUserData(TObjectPtr<UShidenUserSaveGame> SaveGame, TObjectPtr<UTexture2D> Thumbnail, const TMap<FString, FString>& SlotMetadata,
	                                FOnSaveCompletedDelegate SavedDelegate = FOnSaveCompletedDelegate());

	/**
	 * Asynchronously commits a prepared system save game to storage.
	 *
	 * @param SaveGame The save game instance to commit
	 * @param SavedDelegate Delegate called when the save operation completes
	 */
	static void AsyncCommitSystemData(const TObjectPtr<UShidenSystemSaveGame>& SaveGame, FOnSaveCompletedDelegate SavedDelegate = FOnSaveCompletedDelegate());

	/**
	 * Gets a reference to all available save slots.
	 * 
	 * @return Reference to a map of save slots, where the key is the slot name
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static TMap<FString, FShidenSaveSlot>& AcquireSaveSlots();

	/**
	 * Loads user data from a specified save slot. This operation includes both retrieving and applying the data.
	 *
	 * @param SlotName The name of the save slot to load from
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Load User Data (Retrieve & Apply)"))
	static UPARAM(DisplayName = "Success") bool TryLoadUserData(const FString& SlotName);

	/**
	 * Loads system data. This operation includes both retrieving and applying the data.
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Load System Data (Retrieve & Apply)"))
	static UPARAM(DisplayName = "Success") bool TryLoadSystemData();

	/**
	 * Loads predefined system data. This operation includes both retrieving and applying the data.
	 *
	 * @param WorldContextObject Object that provides context for the world
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (WorldContext = "WorldContextObject", DisplayName = "Load Predefined System Data"))
	static UPARAM(DisplayName = "Success") bool TryLoadPredefinedSystemData(const UObject* WorldContextObject);

	/**
	 * Retrieves user data from a specified save slot without applying it.
	 * Use this when you need to inspect or modify the save game before applying.
	 *
	 * @param SlotName The name of the save slot to retrieve from
	 * @param SaveGame [out] The retrieved save game instance
	 * @return True if the retrieval was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced", meta = (DisplayName = "Retrieve User Data"))
	static UPARAM(DisplayName = "Success") bool TryRetrieveUserData(const FString& SlotName, UShidenUserSaveGame*& SaveGame);

	/**
	 * Applies a retrieved user save game to the current game state.
	 *
	 * @param SaveGame The save game instance to apply
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced")
	static void ApplyUserData(UShidenUserSaveGame* SaveGame);

	/**
	 * Retrieves system data without applying it.
	 * Use this when you need to inspect or modify the save game before applying.
	 *
	 * @param SaveGame [out] The retrieved save game instance
	 * @return True if the retrieval was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced", meta = (DisplayName = "Retrieve System Data"))
	static UPARAM(DisplayName = "Success") bool TryRetrieveSystemData(UShidenSystemSaveGame*& SaveGame);

	/**
	 * Applies a retrieved system save game to the current game state.
	 *
	 * @param SaveGame The save game instance to apply
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game|Advanced")
	static void ApplySystemData(UShidenSystemSaveGame* SaveGame);

	/**
	 * Asynchronously loads user data from a specified save slot. This operation includes both retrieving and applying the data.
	 *
	 * @param SlotName The name of the save slot to load from
	 * @param LoadedDelegate Delegate called when the load operation completes
	 */
	static void AsyncLoadUserData(const FString& SlotName, FOnLoadCompletedDelegate LoadedDelegate = FOnLoadCompletedDelegate());

	/**
	 * Asynchronously loads system data. This operation includes both retrieving and applying the data.
	 *
	 * @param LoadedDelegate Delegate called when the load operation completes
	 */
	static void AsyncLoadSystemData(FOnLoadCompletedDelegate LoadedDelegate = FOnLoadCompletedDelegate());

	/**
	 * Asynchronously loads predefined system data. This operation includes both retrieving and applying the data.
	 *
	 * @param WorldContextObject Object that provides context for the world
	 * @param LoadedDelegate Delegate called when the load operation completes
	 */
	static void AsyncLoadPredefinedSystemData(const TObjectPtr<UObject>& WorldContextObject, FOnLoadCompletedDelegate LoadedDelegate = FOnLoadCompletedDelegate());

	/**
	 * Asynchronously retrieves user data from a specified save slot without applying it.
	 * Use this when you need to inspect or modify the save game before applying.
	 *
	 * @param SlotName The name of the save slot to retrieve from
	 * @param RetrievedDelegate Delegate called when the retrieve operation completes with the save game instance
	 */
	static void AsyncRetrieveUserData(const FString& SlotName, FOnRetrieveUserDataCompletedDelegate RetrievedDelegate = FOnRetrieveUserDataCompletedDelegate());

	/**
	 * Asynchronously retrieves system data without applying it.
	 * Use this when you need to inspect or modify the save game before applying.
	 *
	 * @param RetrievedDelegate Delegate called when the retrieve operation completes with the save game instance
	 */
	static void AsyncRetrieveSystemData(FOnRetrieveSystemDataCompletedDelegate RetrievedDelegate = FOnRetrieveSystemDataCompletedDelegate());

	/**
	 * Gets the name of the most recently used save slot.
	 * 
	 * @param SlotName [out] The name of the most recent save slot
	 * @return True if a save slot was found
	 */
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (DisplayName = "Get Latest User Save Slot Name"))
	static UPARAM(DisplayName = "Success") bool TryGetLatestUserSaveSlotName(FString& SlotName);

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
