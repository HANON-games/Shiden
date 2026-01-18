// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ShidenAsyncActionRetrieveSaveGame.generated.h"

class UShidenUserSaveGame;
class UShidenSystemSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAsyncRetrieveUserDataCompleted, bool, bSuccess, UShidenUserSaveGame*, SaveGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAsyncRetrieveSystemDataCompleted, bool, bSuccess, UShidenSystemSaveGame*, SaveGame);

/**
 * Async action for retrieving user save game data.
 */
UCLASS()
class SHIDENCORE_API UShidenAsyncActionRetrieveUserData : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Asynchronously retrieves user data from a specified save slot without applying it.
	 * Use this when you need to inspect or modify the save game before applying.
	 * @param WorldContextObject Object that provides context for the world
	 * @param SlotName The name of the save slot to retrieve from
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game|Advanced", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionRetrieveUserData* AsyncRetrieveUserData(UObject* WorldContextObject, const FString& SlotName);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncRetrieveUserDataCompleted Completed;

protected:
	UPROPERTY()
	FString SlotName;

	void ExecuteCompleted(bool bSuccess, UShidenUserSaveGame* SaveGame);
};

/**
 * Async action for retrieving system save game data.
 */
UCLASS()
class SHIDENCORE_API UShidenAsyncActionRetrieveSystemData : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Asynchronously retrieves system data without applying it.
	 * Use this when you need to inspect or modify the save game before applying.
	 * @param WorldContextObject Object that provides context for the world
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game|Advanced", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionRetrieveSystemData* AsyncRetrieveSystemData(UObject* WorldContextObject);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncRetrieveSystemDataCompleted Completed;

protected:
	void ExecuteCompleted(bool bSuccess, UShidenSystemSaveGame* SaveGame);
};
