// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Save/ShidenAsyncActionRetrieveSaveGame.h"
#include "Save/ShidenSaveBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShidenAsyncActionRetrieveSaveGame)

// UShidenAsyncActionRetrieveUserData

UShidenAsyncActionRetrieveUserData* UShidenAsyncActionRetrieveUserData::AsyncRetrieveUserData(UObject* WorldContextObject, const FString& SlotName)
{
	const TObjectPtr<UShidenAsyncActionRetrieveUserData> Action = NewObject<UShidenAsyncActionRetrieveUserData>();
	Action->SlotName = SlotName;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionRetrieveUserData::Activate()
{
	UShidenSaveBlueprintLibrary::AsyncRetrieveUserData(SlotName,
	                                                   FOnRetrieveUserDataCompletedDelegate::CreateUObject(this, &UShidenAsyncActionRetrieveUserData::ExecuteCompleted));
}

void UShidenAsyncActionRetrieveUserData::ExecuteCompleted(const bool bSuccess, UShidenUserSaveGame* SaveGame)
{
	Completed.Broadcast(bSuccess, SaveGame);
	SetReadyToDestroy();
}

// UShidenAsyncActionRetrieveSystemData

UShidenAsyncActionRetrieveSystemData* UShidenAsyncActionRetrieveSystemData::AsyncRetrieveSystemData(UObject* WorldContextObject)
{
	const TObjectPtr<UShidenAsyncActionRetrieveSystemData> Action = NewObject<UShidenAsyncActionRetrieveSystemData>();
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionRetrieveSystemData::Activate()
{
	UShidenSaveBlueprintLibrary::AsyncRetrieveSystemData(
		FOnRetrieveSystemDataCompletedDelegate::CreateUObject(this, &UShidenAsyncActionRetrieveSystemData::ExecuteCompleted));
}

void UShidenAsyncActionRetrieveSystemData::ExecuteCompleted(const bool bSuccess, UShidenSystemSaveGame* SaveGame)
{
	Completed.Broadcast(bSuccess, SaveGame);
	SetReadyToDestroy();
}
