// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ShidenAsyncActionCommitSaveGame.generated.h"

class UShidenUserSaveGame;
class UShidenSystemSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncCommitUserDataCompleted, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncCommitSystemDataCompleted, bool, bSuccess);

/**
 * Async action for committing prepared user save game data to storage.
 */
UCLASS()
class SHIDENCORE_API UShidenAsyncActionCommitUserData : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Asynchronously commits a prepared user save game to storage.
	 * @param WorldContextObject Object that provides context for the world
	 * @param SaveGame The prepared save game instance to commit
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game|Advanced", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionCommitUserData* AsyncCommitUserData(UObject* WorldContextObject, UShidenUserSaveGame* SaveGame, UTexture2D* Thumbnail,
	                                                              const TMap<FString, FString>& SlotMetadata);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncCommitUserDataCompleted Completed;

protected:
	UPROPERTY()
	TObjectPtr<UTexture2D> Thumbnail;

	UPROPERTY()
	TMap<FString, FString> SlotMetadata;

	UPROPERTY()
	TObjectPtr<UShidenUserSaveGame> UserSaveGame;

	void ExecuteCompleted(bool bSuccess);
};

/**
 * Async action for committing prepared system save game data to storage.
 */
UCLASS()
class SHIDENCORE_API UShidenAsyncActionCommitSystemData : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Asynchronously commits a prepared system save game to storage.
	 * @param WorldContextObject Object that provides context for the world
	 * @param SaveGame The prepared save game instance to commit
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game|Advanced", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionCommitSystemData* AsyncCommitSystemData(UObject* WorldContextObject, UShidenSystemSaveGame* SaveGame);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncCommitSystemDataCompleted Completed;

protected:
	UPROPERTY()
	TObjectPtr<UShidenSystemSaveGame> SystemSaveGame;

	void ExecuteCompleted(bool bSuccess);
};
