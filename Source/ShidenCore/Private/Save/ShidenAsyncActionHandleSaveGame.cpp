// Copyright (c) 2024 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "Save/ShidenAsyncActionHandleSaveGame.h"
#include "Utility/ShidenCoreFunctionLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShidenAsyncActionHandleSaveGame)

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSaveUserData(UObject* WorldContextObject, const FString SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata)
{
	UShidenAsyncActionHandleSaveGame* Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = ESaveGameOperationName::SaveUserData;
	Action->SlotName = SlotName;
	Action->Thumbnail = Thumbnail;
	Action->SlotMetadata = SlotMetadata;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSaveSystemData(UObject* WorldContextObject)
{
	UShidenAsyncActionHandleSaveGame* Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = ESaveGameOperationName::SaveSystemData;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionHandleSaveGame::Activate()
{
	switch (Operation)
	{
	case ESaveGameOperationName::SaveUserData:
		UShidenCoreFunctionLibrary::AsyncSaveUserData(SlotName, Thumbnail, SlotMetadata, FAsyncSaveDataDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
		return;
	case ESaveGameOperationName::SaveSystemData:
		UShidenCoreFunctionLibrary::AsyncSaveSystemData(FAsyncSaveDataDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
		return;
	}

	UE_LOG(LogScript, Error, TEXT("UAsyncActionHandleSaveGame Created with invalid operation!"));

	UShidenAsyncActionHandleSaveGame::ExecuteCompleted(false);
}

void UShidenAsyncActionHandleSaveGame::ExecuteCompleted(const bool bSuccess)
{
	Completed.Broadcast(bSuccess);
	SetReadyToDestroy();
}