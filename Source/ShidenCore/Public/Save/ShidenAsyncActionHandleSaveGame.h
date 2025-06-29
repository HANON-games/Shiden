// Copyright (c) 2025 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Templates/SubclassOf.h"
#include "ShidenAsyncActionHandleSaveGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncHandleSaveGame, bool, bSuccess);

UENUM()
enum class EShidenSaveOperation : uint8
{
	User,
	System,
	PredefinedSystem,
};

UCLASS()
class SHIDENCORE_API UShidenAsyncActionHandleSaveGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/**
	 * Asynchronously saves user data to a specified save slot.
	 * @param WorldContextObject Object that provides context for the world
	 * @param SlotName The name of the save slot to save to
	 * @param Thumbnail The thumbnail image to save with the data
	 * @param SlotMetadata Additional metadata to store with the save
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionHandleSaveGame* AsyncSaveUserData(UObject* WorldContextObject, const FString& SlotName, UTexture2D* Thumbnail,
	                                                           const TMap<FString, FString>& SlotMetadata);

	/**
	 * Asynchronously saves system data.
	 * @param WorldContextObject Object that provides context for the world
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionHandleSaveGame* AsyncSaveSystemData(UObject* WorldContextObject);

	/**
	 * Asynchronously saves predefined system data.
	 * @param WorldContextObject Object that provides context for the world
	 * @return The async action object that can be used to track completion
	 */
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionHandleSaveGame* AsyncSavePredefinedSystemData(UObject* WorldContextObject);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncHandleSaveGame Completed;

protected:
	UPROPERTY()
	EShidenSaveOperation Operation;

	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	UTexture2D* Thumbnail;

	UPROPERTY()
	TMap<FString, FString> SlotMetadata;

	virtual void ExecuteCompleted(bool bSuccess);
};
