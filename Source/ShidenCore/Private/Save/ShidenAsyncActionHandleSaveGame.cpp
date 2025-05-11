// Copyright (c) 2025 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "Save/ShidenAsyncActionHandleSaveGame.h"
#include "Save/ShidenSaveBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShidenAsyncActionHandleSaveGame)

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSaveUserData(UObject* WorldContextObject, const FString& SlotName,
                                                                                      UTexture2D* Thumbnail,
                                                                                      const TMap<FString, FString>& SlotMetadata)
{
	UShidenAsyncActionHandleSaveGame* Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveGameOperationName::SaveUserData;
	Action->SlotName = SlotName;
	Action->Thumbnail = Thumbnail;
	Action->SlotMetadata = SlotMetadata;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSaveSystemData(UObject* WorldContextObject)
{
	UShidenAsyncActionHandleSaveGame* Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveGameOperationName::SaveSystemData;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionHandleSaveGame::Activate()
{
	switch (Operation)
	{
	case EShidenSaveGameOperationName::SaveUserData:
		UShidenSaveBlueprintLibrary::AsyncSaveUserData(SlotName, Thumbnail, SlotMetadata,
		                                               FAsyncSaveDataDelegate::CreateUObject(
			                                               this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
		return;
	case EShidenSaveGameOperationName::SaveSystemData:
		UShidenSaveBlueprintLibrary::AsyncSaveSystemData(
			FAsyncSaveDataDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
		return;
	default:
		UE_LOG(LogScript, Error, TEXT("UAsyncActionHandleSaveGame Created with invalid operation!"));
		UShidenAsyncActionHandleSaveGame::ExecuteCompleted(false);
	}
}

void UShidenAsyncActionHandleSaveGame::ExecuteCompleted(const bool bSuccess)
{
	Completed.Broadcast(bSuccess);
	SetReadyToDestroy();
}
