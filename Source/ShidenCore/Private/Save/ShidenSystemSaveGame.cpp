// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Save/ShidenSystemSaveGame.h"
#include "System/ShidenStructuredLog.h"
#include "System/ShidenSubsystem.h"
#include "Config/ShidenProjectConfig.h"
#include "Engine/Engine.h"

TObjectPtr<UShidenSystemSaveGame> UShidenSystemSaveGame::GetOrCreate()
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	TSubclassOf<UShidenSystemSaveGame> SaveGameClass = ShidenProjectConfig->SystemSaveGameClass;

	if (DoesExist())
	{
		const TObjectPtr<USaveGame> SaveGame = UGameplayStatics::LoadGameFromSlot(SystemDataSlotName, 0);
		// If it differs from save game class, issue a warning
		if (SaveGameClass && !SaveGame->IsA(SaveGameClass))
		{
			SHIDEN_WARNING("Loaded ShidenSystemSaveGame is not of the expected class. Expected {expected} but got {actual}.",
			               *SaveGameClass->GetName(),
			               *SaveGame->GetClass()->GetName());
		}
		return Cast<UShidenSystemSaveGame>(SaveGame);
	}

	if (!SaveGameClass)
	{
		SaveGameClass = StaticClass();
	}
	return Cast<UShidenSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
}

void UShidenSystemSaveGame::Apply() const
{
	GEngine->GetEngineSubsystem<UShidenSubsystem>()->SystemVariable = SystemVariable;
}

void UShidenSystemSaveGame::Prepare()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	SystemVariable = ShidenSubsystem->SystemVariable;
	if (CreatedAt == FDateTime::MinValue())
	{
		CreatedAt = FDateTime::UtcNow();
	}
	UpdatedAt = FDateTime::UtcNow();
}

bool UShidenSystemSaveGame::TryCommit()
{
	return UGameplayStatics::SaveGameToSlot(this, SystemDataSlotName, 0);
}

bool UShidenSystemSaveGame::DoesExist()
{
	return UGameplayStatics::DoesSaveGameExist(SystemDataSlotName, 0);
}

bool UShidenSystemSaveGame::TryDelete()
{
	return UGameplayStatics::DeleteGameInSlot(SystemDataSlotName, 0);
}
