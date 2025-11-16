// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Save/ShidenSaveBlueprintLibrary.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"
#include "Save/ShidenSystemSaveGame.h"
#include "Save/ShidenUserSaveGame.h"
#include "System/ShidenSubsystem.h"
#include "Tasks/Pipe.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Save/ShidenPredefinedSystemSaveGame.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

FPipe UShidenSaveBlueprintLibrary::SaveGamePipe = FPipe{TEXT("SaveGamePipe")};

SHIDENCORE_API UTexture2D* UShidenSaveBlueprintLibrary::ConvertSaveTextureToTexture2D(const FShidenSaveTexture& SaveTexture)
{
	if (SaveTexture.Width == 0 || SaveTexture.Height == 0)
	{
		return nullptr;
	}

	const TObjectPtr<UTexture2D> Texture = UTexture2D::CreateTransient(SaveTexture.Width, SaveTexture.Height, SaveTexture.Format);
	if (!Texture)
	{
		return nullptr;
	}

#if WITH_EDITORONLY_DATA
	Texture->MipGenSettings = TMGS_NoMipmaps;
#endif
	Texture->NeverStream = true;
	Texture->SRGB = true;
	Texture->LODGroup = TEXTUREGROUP_Pixels2D;

	FTexturePlatformData* PlatformData = Texture->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		return nullptr;
	}

	FTexture2DMipMap& Mip = PlatformData->Mips[0];

	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, SaveTexture.Pixels.GetData(), SaveTexture.Pixels.Num());
	Mip.BulkData.Unlock();

	Texture->UpdateResource();

	return Texture;
}

TObjectPtr<UShidenUserSaveGame> CreateOrUpdateUserSaveGame(const FString& SlotName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenSaveBlueprintLibrary::DoesUserDataExist(SlotName)
		                                                         ? Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0))
		                                                         : Cast<UShidenUserSaveGame>(
			                                                         UGameplayStatics::CreateSaveGameObject(UShidenUserSaveGame::StaticClass()));
	SaveGameInstance->ScenarioProperties = ShidenSubsystem->ScenarioProperties;
	SaveGameInstance->UserVariable = ShidenSubsystem->UserVariable;
	SaveGameInstance->LocalVariable = ShidenSubsystem->LocalVariable;
	SaveGameInstance->ScenarioProgressStack = ShidenSubsystem->ScenarioProgressStack;
	if (SaveGameInstance->CreatedAt == FDateTime::MinValue())
	{
		SaveGameInstance->CreatedAt = FDateTime::UtcNow();
	}
	SaveGameInstance->UpdatedAt = FDateTime::UtcNow();

	return SaveGameInstance;
}

TObjectPtr<UShidenSaveSlotsSaveGame> CreateOrUpdateSaveSlots(const FString& SlotName, const FShidenSaveTexture& SaveTexture,
                                                             const TMap<FString, FString>& SaveSlotMetadata)
{
	TMap<FString, FShidenSaveSlot> SaveSlots = UShidenSaveBlueprintLibrary::AcquireSaveSlots();
	const FDateTime CreatedAt = SaveSlots.Contains(SlotName) ? SaveSlots[SlotName].CreatedAt : FDateTime::UtcNow();
	SaveSlots.Add(SlotName, FShidenSaveSlot{SlotName, SaveSlotMetadata, SaveTexture, CreatedAt, FDateTime::UtcNow()});
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = Cast<UShidenSaveSlotsSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UShidenSaveSlotsSaveGame::StaticClass()));
	SaveSlotsInstance->SaveSlots = SaveSlots;
	return SaveSlotsInstance;
}

void UShidenSaveBlueprintLibrary::WaitUntilEmpty()
{
	while (SaveGamePipe.HasWork())
	{
		// Sleep for 1ms to avoid busy-waiting and reduce CPU usage
		FPlatformProcess::Sleep(0.001f);
	}
}

bool UShidenSaveBlueprintLibrary::DoesSaveSlotsExist()
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(TEXT("ShidenSaveSlots"), 0);
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TrySaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata)
{
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

	WaitUntilEmpty();

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = CreateOrUpdateUserSaveGame(SlotName);
	const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (!bSuccess)
	{
		return false;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = CreateOrUpdateSaveSlots(SlotName, SaveTexture, SlotMetadata);
	return UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0);
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncSaveUserData(const FString& SlotName, UTexture2D* Thumbnail,
                                                                   const TMap<FString, FString>& SlotMetadata, FOnSaveCompletedDelegate SavedDelegate)
{
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		// ReSharper disable once CppExpressionWithoutSideEffects
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	if (SlotName == TEXT("ShidenSaveSlots"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSaveSlots\" as user save slot name"));
		// ReSharper disable once CppExpressionWithoutSideEffects
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	TObjectPtr<UShidenUserSaveGame> SaveGameInstance = CreateOrUpdateUserSaveGame(SlotName);
	TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = CreateOrUpdateSaveSlots(SlotName, SaveTexture, SlotMetadata);

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SlotName, SaveSlotsInstance, SaveGameInstance, SavedDelegate]
	{
		if (const bool bSaveSlotSuccess = UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0))
		{
			const bool bSaveUserDataSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
			AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSaveUserDataSuccess]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				SavedDelegate.ExecuteIfBound(bSaveUserDataSuccess);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSaveSlotSuccess]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				SavedDelegate.ExecuteIfBound(bSaveSlotSuccess);
			});
		}
	});
}

TObjectPtr<UShidenSystemSaveGame> CreateOrUpdateSystemSaveGame()
{
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UShidenSystemSaveGame::StaticClass()));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SaveGameInstance->SystemVariable = ShidenSubsystem->SystemVariable;
	if (SaveGameInstance->CreatedAt == FDateTime::MinValue())
	{
		SaveGameInstance->CreatedAt = FDateTime::UtcNow();
	}
	SaveGameInstance->UpdatedAt = FDateTime::UtcNow();

	return SaveGameInstance;
}


TObjectPtr<UShidenPredefinedSystemSaveGame> CreateOrUpdatePredefinedSystemSaveGame()
{
	const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = Cast<UShidenPredefinedSystemSaveGame>(
		UGameplayStatics::CreateSaveGameObject(UShidenPredefinedSystemSaveGame::StaticClass()));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SaveGameInstance->PredefinedSystemVariable = ShidenSubsystem->PredefinedSystemVariable;
	SaveGameInstance->ScenarioReadLines = ShidenSubsystem->ScenarioReadLines;
	if (SaveGameInstance->CreatedAt == FDateTime::MinValue())
	{
		SaveGameInstance->CreatedAt = FDateTime::UtcNow();
	}
	SaveGameInstance->UpdatedAt = FDateTime::UtcNow();

	return SaveGameInstance;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TrySaveSystemData()
{
	WaitUntilEmpty();
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = CreateOrUpdateSystemSaveGame();
	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TrySavePredefinedSystemData()
{
	WaitUntilEmpty();
	const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = CreateOrUpdatePredefinedSystemSaveGame();
	return UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenPredefinedSystemData"), 0);
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncSaveSystemData(FOnSaveCompletedDelegate SavedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SavedDelegate]
	{
		const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = CreateOrUpdateSystemSaveGame();
		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
		AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSuccess]
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncSavePredefinedSystemData(FOnSaveCompletedDelegate SavedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SavedDelegate]
	{
		const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = CreateOrUpdatePredefinedSystemSaveGame();
		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenPredefinedSystemData"), 0);
		AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSuccess]
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API TMap<FString, FShidenSaveSlot>& UShidenSaveBlueprintLibrary::AcquireSaveSlots()
{
	WaitUntilEmpty();

	if (!DoesSaveSlotsExist())
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSaveSlotsSaveGame::StaticClass()))->SaveSlots;
	}

	return Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSaveSlots"), 0))->SaveSlots;
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::LoadUserData(const FString& SlotName)
{
	WaitUntilEmpty();

	if (!DoesUserDataExist(SlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("User data does not exist for slot name: %s"), *SlotName);
		return;
	}

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->ScenarioProperties = SaveGameInstance->ScenarioProperties;
	ShidenSubsystem->UserVariable = SaveGameInstance->UserVariable;
	ShidenSubsystem->UserVariable.UpdateVariableDefinitions(ShidenProjectConfig->UserVariableDefinitions);
	ShidenSubsystem->ScenarioProgressStack = SaveGameInstance->ScenarioProgressStack;
	ShidenSubsystem->LocalVariable = SaveGameInstance->LocalVariable;
	ShidenSubsystem->LocalVariable.UpdateVariableDefinitions();
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryGetLatestUserSaveSlotName(FString& SlotName)
{
	WaitUntilEmpty();

	if (!DoesAnyUserDataExist())
	{
		UE_LOG(LogTemp, Warning, TEXT("User data does not exist."));
		return false;
	}

	const TMap<FString, FShidenSaveSlot> SaveSlots = AcquireSaveSlots();
	const FString LatestSlotName = [&SaveSlots]
	{
		FString SaveSlotName;
		FDateTime LatestDateTime = FDateTime::MinValue();
		for (const auto& [Key, Value] : SaveSlots)
		{
			if (Value.UpdatedAt > LatestDateTime)
			{
				SaveSlotName = Key;
				LatestDateTime = Value.UpdatedAt;
			}
		}
		return SaveSlotName;
	}();

	SlotName = LatestSlotName;
	return true;
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::LoadSystemData()
{
	WaitUntilEmpty();

	if (!DoesSystemDataExist())
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return;
	}

	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSystemData"), 0));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable = SaveGameInstance->SystemVariable;
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::LoadPredefinedSystemData(const UObject* WorldContextObject)
{
	WaitUntilEmpty();

	if (!DoesPredefinedSystemDataExist())
	{
		UE_LOG(LogTemp, Warning, TEXT("Predefined system data does not exist"));
		return;
	}

	const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = Cast<UShidenPredefinedSystemSaveGame>(
		UGameplayStatics::LoadGameFromSlot(TEXT("ShidenPredefinedSystemData"), 0));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->SystemVariable.UpdateVariableDefinitions(ShidenProjectConfig->SystemVariableDefinitions);
	ShidenSubsystem->PredefinedSystemVariable = FShidenPredefinedSystemVariable(SaveGameInstance->PredefinedSystemVariable);
	ShidenSubsystem->ScenarioReadLines = SaveGameInstance->ScenarioReadLines;

	// Apply volume rate
	if (!GEngine || !GEngine->UseSound())
	{
		return;
	}

	const TObjectPtr<UWorld> ThisWorld = WorldContextObject->GetWorld();
	if (!ThisWorld || !ThisWorld->bAllowAudioPlayback)
	{
		return;
	}

	if (FAudioDeviceHandle AudioDevice = ThisWorld->GetAudioDevice())
	{
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.MasterVolume, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBGMSoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.BGMVolume, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetSESoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.SEVolume, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(),
		                                      ShidenSubsystem->PredefinedSystemVariable.VoiceVolume, 1.0, 0.0, true);
		AudioDevice->PushSoundMixModifier(ShidenProjectConfig->GetSoundClassMix());
	}
}

void UShidenSaveBlueprintLibrary::DeleteUserData(const FString& SlotName)
{
	WaitUntilEmpty();

	if (DoesUserDataExist(SlotName))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}

	if (DoesSaveSlotsExist())
	{
		const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = Cast<UShidenSaveSlotsSaveGame>(
			UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSaveSlots"), 0));
		SaveSlotsInstance->SaveSlots.Remove(SlotName);
		UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0);
	}
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::DeleteSystemData()
{
	WaitUntilEmpty();

	if (DoesSystemDataExist())
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("ShidenSystemData"), 0);
	}
}

void UShidenSaveBlueprintLibrary::DeletePredefinedSystemData()
{
	WaitUntilEmpty();

	if (DoesPredefinedSystemDataExist())
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("ShidenPredefinedSystemData"), 0);
	}
}

bool UShidenSaveBlueprintLibrary::DoesAnyUserDataExist()
{
	WaitUntilEmpty();

	return DoesSaveSlotsExist() && AcquireSaveSlots().Num() > 0;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::DoesUserDataExist(const FString& SlotName)
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::DoesSystemDataExist()
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::DoesPredefinedSystemDataExist()
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(TEXT("ShidenPredefinedSystemData"), 0);
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::ClearLoadedSystemData()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	UShidenVariableBlueprintLibrary::ResetSystemVariables();
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::ClearLoadedPredefinedSystemData(const UObject* WorldContextObject)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	UShidenVariableBlueprintLibrary::ResetPredefinedSystemVariables(WorldContextObject);
}

void UShidenSaveBlueprintLibrary::ClearLoadedUserData()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	UShidenVariableBlueprintLibrary::ResetUserVariables();
	UShidenVariableBlueprintLibrary::ResetAllLocalVariables();
	ShidenSubsystem->ScenarioProperties.Empty();
	ShidenSubsystem->ScenarioProgressStack.Empty();
}
