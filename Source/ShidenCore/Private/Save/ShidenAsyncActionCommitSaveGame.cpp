// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Save/ShidenAsyncActionCommitSaveGame.h"
#include "Save/ShidenSaveBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShidenAsyncActionCommitSaveGame)

// UShidenAsyncActionCommitUserData

UShidenAsyncActionCommitUserData* UShidenAsyncActionCommitUserData::AsyncCommitUserData(UObject* WorldContextObject, UShidenUserSaveGame* SaveGame,
                                                                                        UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata)
{
	const TObjectPtr<UShidenAsyncActionCommitUserData> Action = NewObject<UShidenAsyncActionCommitUserData>();
	Action->UserSaveGame = SaveGame;
	Action->Thumbnail = Thumbnail;
	Action->SlotMetadata = SlotMetadata;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionCommitUserData::Activate()
{
	UShidenSaveBlueprintLibrary::AsyncCommitUserData(UserSaveGame, Thumbnail, SlotMetadata,
	                                                 FOnSaveCompletedDelegate::CreateUObject(this, &UShidenAsyncActionCommitUserData::ExecuteCompleted));
}

void UShidenAsyncActionCommitUserData::ExecuteCompleted(const bool bSuccess)
{
	Completed.Broadcast(bSuccess);
	SetReadyToDestroy();
}

// UShidenAsyncActionCommitSystemData

UShidenAsyncActionCommitSystemData* UShidenAsyncActionCommitSystemData::AsyncCommitSystemData(UObject* WorldContextObject, UShidenSystemSaveGame* SaveGame)
{
	const TObjectPtr<UShidenAsyncActionCommitSystemData> Action = NewObject<UShidenAsyncActionCommitSystemData>();
	Action->SystemSaveGame = SaveGame;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionCommitSystemData::Activate()
{
	UShidenSaveBlueprintLibrary::AsyncCommitSystemData(SystemSaveGame,
	                                                   FOnSaveCompletedDelegate::CreateUObject(this, &UShidenAsyncActionCommitSystemData::ExecuteCompleted));
}

void UShidenAsyncActionCommitSystemData::ExecuteCompleted(const bool bSuccess)
{
	Completed.Broadcast(bSuccess);
	SetReadyToDestroy();
}
