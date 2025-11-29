// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Save/ShidenUserSaveGame.h"
#include "Config/ShidenProjectConfig.h"
#include "Engine/Engine.h"
#include "System/ShidenSubsystem.h"

bool UShidenUserSaveGame::IsValidSlotName(const FString& SlotName)
{
	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot use empty string as user save slot name"));
		return false;
	}
	
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		return false;
	}

	if (SlotName == TEXT("ShidenSaveSlots"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSaveSlots\" as user save slot name"));
		return false;
	}

	if (SlotName == TEXT("ShidenPredefinedSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenPredefinedSystemData\" as user save slot name"));
		return false;
	}

	return true;
}

TObjectPtr<UShidenUserSaveGame> UShidenUserSaveGame::GetOrCreate(const FString& InSlotName)
{
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();
	TSubclassOf<UShidenUserSaveGame> SaveGameClass = ShidenProjectConfig->UserSaveGameClass;

	if (DoesExist(InSlotName))
	{
		const TObjectPtr<USaveGame> SaveGame = UGameplayStatics::LoadGameFromSlot(InSlotName, 0);
		// If it differs from save game class, issue a warning
		if (SaveGameClass && !SaveGame->IsA(SaveGameClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("Loaded ShidenUserSaveGame is not of the expected class. Expected %s but got %s."),
			       *SaveGameClass->GetName(),
			       *SaveGame->GetClass()->GetName());
		}
		const TObjectPtr<UShidenUserSaveGame> UserSaveGame =  Cast<UShidenUserSaveGame>(SaveGame);
		UserSaveGame->SlotName = InSlotName;
		return UserSaveGame;
	}

	if (!SaveGameClass)
	{
		SaveGameClass = StaticClass();
	}
	
	const TObjectPtr<UShidenUserSaveGame> UserSaveGame = Cast<UShidenUserSaveGame>(UGameplayStatics::CreateSaveGameObject(SaveGameClass));
	UserSaveGame->SlotName = InSlotName;
	return UserSaveGame;
}

void UShidenUserSaveGame::Apply() const
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->ScenarioProperties = ScenarioProperties;
	ShidenSubsystem->UserVariable = UserVariable;
	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ShidenProjectConfig->UserVariableDefinitions);
	ShidenSubsystem->ScenarioProgressStack = ScenarioProgressStack;
	ShidenSubsystem->LocalVariable = LocalVariable;
	ShidenSubsystem->LocalVariable.UpdateVariableDefinitions();
}

void UShidenUserSaveGame::Prepare()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	ScenarioProperties = ShidenSubsystem->ScenarioProperties;
	UserVariable = ShidenSubsystem->UserVariable;
	LocalVariable = ShidenSubsystem->LocalVariable;
	ScenarioProgressStack = ShidenSubsystem->ScenarioProgressStack;
	if (CreatedAt == FDateTime::MinValue())
	{
		CreatedAt = FDateTime::UtcNow();
	}
	UpdatedAt = FDateTime::UtcNow();
}

bool UShidenUserSaveGame::TryCommit()
{
	if (!IsValidSlotName(SlotName))
	{
		return false;
	}
	return UGameplayStatics::SaveGameToSlot(this, SlotName, 0);
}

bool UShidenUserSaveGame::DoesExist(const FString& SlotName)
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool UShidenUserSaveGame::TryDelete(const FString& SlotName)
{
	return UGameplayStatics::DeleteGameInSlot(SlotName, 0);
}
