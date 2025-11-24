// Copyright (c) 2025 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "Save/ShidenAsyncActionHandleSaveGame.h"
#include "Save/ShidenSaveBlueprintLibrary.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ShidenAsyncActionHandleSaveGame)

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSaveUserData(UObject* WorldContextObject, const FString& SlotName,
                                                                                      UTexture2D* Thumbnail,
                                                                                      const TMap<FString, FString>& SlotMetadata)
{
	const TObjectPtr<UShidenAsyncActionHandleSaveGame> Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveLoadOperation::SaveUser;
	Action->SlotName = SlotName;
	Action->Thumbnail = Thumbnail;
	Action->SlotMetadata = SlotMetadata;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSaveSystemData(UObject* WorldContextObject)
{
	const TObjectPtr<UShidenAsyncActionHandleSaveGame> Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveLoadOperation::SaveSystem;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncSavePredefinedSystemData(UObject* WorldContextObject)
{
	const TObjectPtr<UShidenAsyncActionHandleSaveGame> Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveLoadOperation::SavePredefinedSystem;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncLoadUserData(UObject* WorldContextObject, const FString& SlotName)
{
	const TObjectPtr<UShidenAsyncActionHandleSaveGame> Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveLoadOperation::LoadUser;
	Action->SlotName = SlotName;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncLoadSystemData(UObject* WorldContextObject)
{
	const TObjectPtr<UShidenAsyncActionHandleSaveGame> Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveLoadOperation::LoadSystem;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

UShidenAsyncActionHandleSaveGame* UShidenAsyncActionHandleSaveGame::AsyncLoadPredefinedSystemData(UObject* WorldContextObject)
{
	const TObjectPtr<UShidenAsyncActionHandleSaveGame> Action = NewObject<UShidenAsyncActionHandleSaveGame>();
	Action->Operation = EShidenSaveLoadOperation::LoadPredefinedSystem;
	Action->WorldContextObject = WorldContextObject;
	Action->RegisterWithGameInstance(WorldContextObject);
	return Action;
}

void UShidenAsyncActionHandleSaveGame::Activate()
{
	switch (Operation)
	{
	case EShidenSaveLoadOperation::SaveUser:
		{
			UShidenSaveBlueprintLibrary::AsyncSaveUserData(SlotName, Thumbnail, SlotMetadata,
														   FOnSaveCompletedDelegate::CreateUObject(
															   this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
			return;
		}
	case EShidenSaveLoadOperation::SaveSystem:
		{
			UShidenSaveBlueprintLibrary::AsyncSaveSystemData(
				FOnSaveCompletedDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
			return;
		}
	case EShidenSaveLoadOperation::SavePredefinedSystem:
		{
			UShidenSaveBlueprintLibrary::AsyncSavePredefinedSystemData(
				FOnSaveCompletedDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
			return;
		}
	case EShidenSaveLoadOperation::LoadUser:
		{
			UShidenSaveBlueprintLibrary::AsyncLoadUserData(SlotName,
														   FOnLoadCompletedDelegate::CreateUObject(
															   this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
			return;
		}
	case EShidenSaveLoadOperation::LoadSystem:
		{
			UShidenSaveBlueprintLibrary::AsyncLoadSystemData(
				FOnLoadCompletedDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
			return;
		}
	case EShidenSaveLoadOperation::LoadPredefinedSystem:
		{
			UShidenSaveBlueprintLibrary::AsyncLoadPredefinedSystemData(WorldContextObject,
			                                                           FOnLoadCompletedDelegate::CreateUObject(this, &UShidenAsyncActionHandleSaveGame::ExecuteCompleted));
			return;
		}
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("UAsyncActionHandleSaveGame Created with invalid operation!"));
			UShidenAsyncActionHandleSaveGame::ExecuteCompleted(false);
		}
	}
}

void UShidenAsyncActionHandleSaveGame::ExecuteCompleted(const bool bSuccess)
{
	Completed.Broadcast(bSuccess);
	SetReadyToDestroy();
}
