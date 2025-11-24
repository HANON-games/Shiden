// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Save/ShidenSaveSlotsSaveGame.h"
#include "System/ShidenSubsystem.h"

TObjectPtr<UShidenSaveSlotsSaveGame> UShidenSaveSlotsSaveGame::GetOrCreate()
{
	return DoesExist()
		   ? Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotsName, 0))
		   : Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::CreateSaveGameObject(StaticClass()));
}

void UShidenSaveSlotsSaveGame::Prepare(const FString& SlotName, const FShidenSaveTexture& SaveTexture, const TMap<FString, FString>& SaveSlotMetadata)
{
	const FDateTime CreatedAt = SaveSlots.Contains(SlotName) ? SaveSlots[SlotName].CreatedAt : FDateTime::UtcNow();
	SaveSlots.Add(SlotName, FShidenSaveSlot{SlotName, SaveSlotMetadata, SaveTexture, CreatedAt, FDateTime::UtcNow()});
}

bool UShidenSaveSlotsSaveGame::TryCommit()
{
	return UGameplayStatics::SaveGameToSlot(this, SaveSlotsName, 0);
}

bool UShidenSaveSlotsSaveGame::DoesExist()
{
	return UGameplayStatics::DoesSaveGameExist(SaveSlotsName, 0);
}

bool UShidenSaveSlotsSaveGame::TryDelete(const FString& SlotName)
{
	if (!DoesExist())
	{
		return false;
	}
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = GetOrCreate();
	SaveSlotsInstance->SaveSlots.Remove(SlotName);
	return SaveSlotsInstance->TryCommit();
}
