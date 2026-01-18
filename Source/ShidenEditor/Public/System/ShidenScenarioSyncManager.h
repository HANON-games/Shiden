// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenScenarioSyncManager.generated.h"

class UShidenScenario;
class IDirectoryWatcher;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScenarioSyncedDelegate);

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
	 * Event fired when a scenario is successfully imported, updated and deleted from CSV/JSON
	 * Bind to this event in Editor Utility Widgets to respond to file changes
	 */
	UPROPERTY(BlueprintAssignable, Category = "SVNInternal")
	FOnScenarioSyncedDelegate OnScenarioSynced;
	
	/**
	 * Exports a single scenario to CSV or JSON file
	 * @param Scenario The scenario to export
	 * @return True if export was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "SVNInternal")
	static bool ExportScenario(UShidenScenario* Scenario);

	static bool ExportAllScenarios();

	static void StartWatchingDirectory();

	static void StopWatchingDirectory();

private:
	static bool ImportAndUpdateScenario(const FString& FilePath);

	static void CheckForExternalChanges();

	static void OnFileAdded(const FString& FilePath);

	static void OnFileModified(const FString& FilePath);

	static void OnFileRemoved(const FString& FilePath);

	static FString GetSyncFilePathForScenario(const UShidenScenario* Scenario);

	static UShidenScenario* FindScenarioByGuid(const FGuid& ScenarioId);

	static FDelegateHandle DirectoryWatcherHandle;

	static IDirectoryWatcher* DirectoryWatcher;

	static FString WatchedDirectoryPath;

	static UShidenScenarioSyncManager* Instance;
};
