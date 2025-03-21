// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Save/ShidenSaveFunctionLibrary.h"
#include "AudioDevice.h"
#include "Kismet/GameplayStatics.h"
#include "Save/ShidenSystemSaveGame.h"
#include "Save/ShidenUserSaveGame.h"
#include "System/ShidenSubsystem.h"
#include "Tasks/Pipe.h"
#include "Async/Async.h"
#include "Engine/Engine.h"

FPipe UShidenSaveFunctionLibrary::SaveGamePipe = FPipe{TEXT("SaveGamePipe")};

SHIDENCORE_API UTexture2D* UShidenSaveFunctionLibrary::ConvertSaveTextureToTexture2D(const FShidenSaveTexture& SaveTexture)
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

	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];

	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, SaveTexture.Pixels.GetData(), SaveTexture.Pixels.Num());
	Mip.BulkData.Unlock();

	Texture->UpdateResource();

	return Texture;
}

TObjectPtr<UShidenUserSaveGame> UpdateUserSaveGameInstance(const FString& SlotName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenSaveFunctionLibrary::DoesUserDataExist(SlotName)
		                                                         ? Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0))
		                                                         : Cast<UShidenUserSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenUserSaveGame::StaticClass()));
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

TObjectPtr<UShidenSaveSlotsSaveGame> UpdateSaveSlotsSaveGameInstance(const FString& SlotName, const FShidenSaveTexture& SaveTexture, const TMap<FString, FString>& SaveSlotMetadata)
{
	TMap<FString, FShidenSaveSlot> SaveSlots = UShidenSaveFunctionLibrary::AcquireSaveSlots();
	const FDateTime CreatedAt = SaveSlots.Contains(SlotName) ? SaveSlots[SlotName].CreatedAt : FDateTime::UtcNow();
	SaveSlots.Add(SlotName, FShidenSaveSlot{SlotName, SaveSlotMetadata, SaveTexture, CreatedAt, FDateTime::UtcNow()});
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSaveSlotsSaveGame::StaticClass()));
	SaveSlotsInstance->SaveSlots = SaveSlots;
	return SaveSlotsInstance;
}

void UShidenSaveFunctionLibrary::WaitUntilEmpty()
{
	while (true)
	{
		if (!SaveGamePipe.HasWork())
		{
			break;
		}
		FPlatformProcess::Sleep(0.0f);
	}
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::SaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, bool& bSuccess)
{
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		return;
	}

	if (SlotName == TEXT("ShidenSaveSlots"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSaveSlots\" as user save slot name"));
		return;
	}

	WaitUntilEmpty();

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UpdateUserSaveGameInstance(SlotName);
	bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (!bSuccess)
	{
		return;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UpdateSaveSlotsSaveGameInstance(SlotName, SaveTexture, SlotMetadata);
	bSuccess = UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0);
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::AsyncSaveUserData(const FString& SlotName, UTexture2D* Thumbnail,const TMap<FString, FString>& SlotMetadata, FAsyncSaveDataDelegate SavedDelegate)
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
	TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UpdateUserSaveGameInstance(SlotName);
	TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UpdateSaveSlotsSaveGameInstance(SlotName, SaveTexture, SlotMetadata);

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

TObjectPtr<UShidenSystemSaveGame> UpdateSystemSaveGameInstance()
{
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSystemSaveGame::StaticClass()));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SaveGameInstance->SystemVariable = ShidenSubsystem->SystemVariable;
	SaveGameInstance->PredefinedSystemVariable = ShidenSubsystem->PredefinedSystemVariable;
	SaveGameInstance->ScenarioReadLines = ShidenSubsystem->ScenarioReadLines;
	if (SaveGameInstance->CreatedAt == FDateTime::MinValue())
	{
		SaveGameInstance->CreatedAt = FDateTime::UtcNow();
	}
	SaveGameInstance->UpdatedAt = FDateTime::UtcNow();

	return SaveGameInstance;
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::SaveSystemData(bool& bSuccess)
{
	WaitUntilEmpty();
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UpdateSystemSaveGameInstance();
	bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::AsyncSaveSystemData(FAsyncSaveDataDelegate SavedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SavedDelegate]
	{
		const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UpdateSystemSaveGameInstance();
		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
		AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSuccess]
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API TMap<FString, FShidenSaveSlot>& UShidenSaveFunctionLibrary::AcquireSaveSlots()
{
	WaitUntilEmpty();

	if (!DoesUserDataExist(TEXT("ShidenSaveSlots")))
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSaveSlotsSaveGame::StaticClass()))->SaveSlots;
	}

	return Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSaveSlots"), 0))->SaveSlots;
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::LoadUserData(const FString& SlotName)
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

SHIDENCORE_API void UShidenSaveFunctionLibrary::LoadSystemData(const UObject* WorldContextObject)
{
	WaitUntilEmpty();

	if (!DoesUserDataExist(TEXT("ShidenSystemData")))
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return;
	}

	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSystemData"), 0));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	ShidenSubsystem->SystemVariable = SaveGameInstance->SystemVariable;
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
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetMasterSoundClass(), ShidenSubsystem->PredefinedSystemVariable.MasterVolumeRate, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetBgmSoundClass(), ShidenSubsystem->PredefinedSystemVariable.BgmVolumeRate, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetSeSoundClass(), ShidenSubsystem->PredefinedSystemVariable.SeVolumeRate, 1.0, 0.0, true);
		AudioDevice->SetSoundMixClassOverride(ShidenProjectConfig->GetSoundClassMix(), ShidenProjectConfig->GetVoiceSoundClass(), ShidenSubsystem->PredefinedSystemVariable.VoiceVolumeRate, 1.0, 0.0, true);
		AudioDevice->PushSoundMixModifier(ShidenProjectConfig->GetSoundClassMix());
	}
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::DeleteUserData(const FString& SlotName)
{
	WaitUntilEmpty();

	if (DoesUserDataExist(SlotName))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}

	if (DoesUserDataExist(TEXT("ShidenSaveSlots")))
	{
		const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSaveSlots"), 0));
		SaveSlotsInstance->SaveSlots.Remove(SlotName);
		UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0);
	}
}

SHIDENCORE_API void UShidenSaveFunctionLibrary::DeleteSystemData()
{
	WaitUntilEmpty();

	if (DoesSystemDataExist())
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("ShidenSystemData"), 0);
	}
}

SHIDENCORE_API bool UShidenSaveFunctionLibrary::DoesUserDataExist(const FString& SlotName)
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

SHIDENCORE_API bool UShidenSaveFunctionLibrary::DoesSystemDataExist()
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(TEXT("ShidenSystemData"), 0);
}
