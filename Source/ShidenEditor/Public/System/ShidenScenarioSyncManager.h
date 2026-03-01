// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenScenarioSyncManager.generated.h"

class UShidenScenario;
class IDirectoryWatcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScenarioSyncedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPostUndoDelegate);

/**
 * Manages synchronization between scenario data assets and CSV/JSON files
 * Use GetInstance() to access the singleton and bind to OnScenarioSynced event
 */
UCLASS()
class SHIDENEDITOR_API UShidenScenarioSyncManager : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * Gets the singleton instance of the Scenario Sync Manager
	 * @return The singleton instance
	 */
	UFUNCTION(BlueprintPure, Category = "SVNInternal", meta = (DisplayName = "Get Scenario Sync Manager"))
	static UShidenScenarioSyncManager* GetInstance();

	/**
	 * Event fired when a scenario is successfully imported or updated from CSV/JSON
	 * Bind to this event in Editor Utility Widgets to respond to file changes
	 */
	UPROPERTY(BlueprintAssignable, Category = "SVNInternal")
	FOnScenarioSyncedDelegate OnScenarioSynced;

	/**
	 * Event triggered after an undo operation is performed in the editor.
	 * Use this delegate to handle custom logic or update UI elements following an undo action.
	 */
	UPROPERTY(BlueprintAssignable, Category = "SVNInternal")
	FOnPostUndoDelegate OnPostUndo;

	/**
	 * Exports a single scenario to CSV or JSON file
	 * @param Scenario The scenario to export
	 * @return True if export was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "SVNInternal")
	static bool ExportScenario(UShidenScenario* Scenario);

	/**
	 * Called after the undo operation is performed
	 * Override PostEditUndo to handle undo notifications
	 */
	virtual void PostEditUndo() override;

	static bool ExportAllScenarios();

	static void StartWatchingDirectory();

	static void StopWatchingDirectory();

private:
	static FDelegateHandle DirectoryWatcherHandle;

	static FDelegateHandle AssetAddedHandle;

	static FDelegateHandle AssetRemovedHandle;

	static IDirectoryWatcher* DirectoryWatcher;

	static FString WatchedDirectoryPath;

	static UShidenScenarioSyncManager* Instance;

	static void OnAssetAdded(const FAssetData& AssetData);

	static void OnAssetRemoved(const FAssetData& AssetData);
};
