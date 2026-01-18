// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Save/ShidenSaveBlueprintLibrary.h"
#include "System/ShidenStructuredLog.h"
#include "Save/ShidenSystemSaveGame.h"
#include "Save/ShidenUserSaveGame.h"
#include "System/ShidenSubsystem.h"
#include "Tasks/Pipe.h"
#include "Async/Async.h"
#include "Engine/Engine.h"
#include "Save/ShidenPredefinedSystemSaveGame.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

using namespace UE::Tasks;

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

void UShidenSaveBlueprintLibrary::WaitUntilEmpty()
{
	while (SaveGamePipe.HasWork())
	{
		// Sleep for 1ms to avoid busy-waiting and reduce CPU usage
		FPlatformProcess::Sleep(0.001f);
	}
}

bool UShidenSaveBlueprintLibrary::DoSaveSlotsExist()
{
	WaitUntilEmpty();
	return UShidenSaveSlotsSaveGame::DoesExist();
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TrySaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata)
{
	if (!UShidenUserSaveGame::IsValidSlotName(SlotName))
	{
		return false;
	}

	WaitUntilEmpty();

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenUserSaveGame::GetOrCreate(SlotName);
	SaveGameInstance->Prepare();
	if (!SaveGameInstance->TryCommit())
	{
		return false;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UShidenSaveSlotsSaveGame::GetOrCreate();
	SaveSlotsInstance->Prepare(SlotName, SaveTexture, SlotMetadata);
	return SaveSlotsInstance->TryCommit();
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncSaveUserData(const FString& SlotName, const TObjectPtr<UTexture2D> Thumbnail,
                                                                   const TMap<FString, FString>& SlotMetadata, FOnSaveCompletedDelegate SavedDelegate)
{
	if (!UShidenUserSaveGame::IsValidSlotName(SlotName))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SlotName, SaveTexture, SlotMetadata, SavedDelegate]
	{
		const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UShidenSaveSlotsSaveGame::GetOrCreate();
		SaveSlotsInstance->Prepare(SlotName, SaveTexture, SlotMetadata);

		if (const bool bSaveSlotSuccess = SaveSlotsInstance->TryCommit())
		{
			const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenUserSaveGame::GetOrCreate(SlotName);
			SaveGameInstance->Prepare();

			const bool bSaveUserDataSuccess = SaveGameInstance->TryCommit();
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

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TrySaveSystemData()
{
	WaitUntilEmpty();
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UShidenSystemSaveGame::GetOrCreate();
	SaveGameInstance->Prepare();
	return SaveGameInstance->TryCommit();
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TrySavePredefinedSystemData()
{
	WaitUntilEmpty();
	const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = UShidenPredefinedSystemSaveGame::GetOrCreate();
	SaveGameInstance->Prepare();
	return SaveGameInstance->TryCommit();
}

SHIDENCORE_API UShidenUserSaveGame* UShidenSaveBlueprintLibrary::PrepareUserData(const FString& SlotName)
{
	if (!UShidenUserSaveGame::IsValidSlotName(SlotName))
	{
		return nullptr;
	}

	WaitUntilEmpty();

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenUserSaveGame::GetOrCreate(SlotName);
	SaveGameInstance->Prepare();
	return SaveGameInstance;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryCommitUserData(UShidenUserSaveGame* SaveGame, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata)
{
	if (!SaveGame || !UShidenUserSaveGame::IsValidSlotName(SaveGame->SlotName))
	{
		return false;
	}

	WaitUntilEmpty();

	if (!SaveGame->TryCommit())
	{
		return false;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UShidenSaveSlotsSaveGame::GetOrCreate();
	SaveSlotsInstance->Prepare(SaveGame->SlotName, SaveTexture, SlotMetadata);
	return SaveSlotsInstance->TryCommit();
}

SHIDENCORE_API UShidenSystemSaveGame* UShidenSaveBlueprintLibrary::PrepareSystemData()
{
	WaitUntilEmpty();

	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UShidenSystemSaveGame::GetOrCreate();
	SaveGameInstance->Prepare();
	return SaveGameInstance;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryCommitSystemData(UShidenSystemSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	WaitUntilEmpty();
	return SaveGame->TryCommit();
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncSaveSystemData(FOnSaveCompletedDelegate SavedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SavedDelegate]
	{
		const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UShidenSystemSaveGame::GetOrCreate();
		SaveGameInstance->Prepare();
		const bool bSuccess = SaveGameInstance->TryCommit();
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
		const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = UShidenPredefinedSystemSaveGame::GetOrCreate();
		SaveGameInstance->Prepare();
		const bool bSuccess = SaveGameInstance->TryCommit();
		AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSuccess]
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncCommitUserData(const TObjectPtr<UShidenUserSaveGame> SaveGame, const TObjectPtr<UTexture2D> Thumbnail,
                                                                     const TMap<FString, FString>& SlotMetadata, FOnSaveCompletedDelegate SavedDelegate)
{
	if (!SaveGame || !UShidenUserSaveGame::IsValidSlotName(SaveGame->SlotName))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	const FString SlotName = SaveGame->SlotName;
	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail.Get());

	SaveGame->AddToRoot();

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SaveGame, SlotName, SaveTexture, SlotMetadata, SavedDelegate]
	{
		if (!SaveGame->TryCommit())
		{
			AsyncTask(ENamedThreads::GameThread, [SaveGame, SavedDelegate]
			{
				SaveGame->RemoveFromRoot();
				// ReSharper disable once CppExpressionWithoutSideEffects
				SavedDelegate.ExecuteIfBound(false);
			});
			return;
		}

		const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UShidenSaveSlotsSaveGame::GetOrCreate();
		SaveSlotsInstance->Prepare(SlotName, SaveTexture, SlotMetadata);
		const bool bSuccess = SaveSlotsInstance->TryCommit();
		AsyncTask(ENamedThreads::GameThread, [SaveGame, SavedDelegate, bSuccess]
		{
			SaveGame->RemoveFromRoot();
			// ReSharper disable once CppExpressionWithoutSideEffects
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncCommitSystemData(const TObjectPtr<UShidenSystemSaveGame>& SaveGame, FOnSaveCompletedDelegate SavedDelegate)
{
	if (!SaveGame)
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	SaveGame->AddToRoot();

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SaveGame, SavedDelegate]
	{
		const bool bSuccess = SaveGame->TryCommit();
		AsyncTask(ENamedThreads::GameThread, [SaveGame, SavedDelegate, bSuccess]
		{
			SaveGame->RemoveFromRoot();
			// ReSharper disable once CppExpressionWithoutSideEffects
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API TMap<FString, FShidenSaveSlot>& UShidenSaveBlueprintLibrary::AcquireSaveSlots()
{
	WaitUntilEmpty();
	return UShidenSaveSlotsSaveGame::GetOrCreate()->SaveSlots;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryLoadUserData(const FString& SlotName)
{
	WaitUntilEmpty();

	if (!UShidenUserSaveGame::DoesExist(SlotName))
	{
		SHIDEN_WARNING("User data does not exist for slot name: {name}", *SlotName);
		return false;
	}

	UShidenUserSaveGame::GetOrCreate(SlotName)->Apply();
	return true;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryRetrieveUserData(const FString& SlotName, UShidenUserSaveGame*& SaveGame)
{
	WaitUntilEmpty();

	SaveGame = nullptr;

	if (!UShidenUserSaveGame::DoesExist(SlotName))
	{
		SHIDEN_WARNING("User data does not exist for slot name: {name}", *SlotName);
		return false;
	}

	SaveGame = UShidenUserSaveGame::GetOrCreate(SlotName);
	return true;
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::ApplyUserData(UShidenUserSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		SHIDEN_WARNING("Cannot apply null user save game");
		return;
	}

	SaveGame->Apply();
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryRetrieveSystemData(UShidenSystemSaveGame*& SaveGame)
{
	WaitUntilEmpty();

	SaveGame = nullptr;

	if (!UShidenSystemSaveGame::DoesExist())
	{
		SHIDEN_WARNING("System data does not exist");
		return false;
	}

	SaveGame = UShidenSystemSaveGame::GetOrCreate();
	return true;
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::ApplySystemData(UShidenSystemSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		SHIDEN_WARNING("Cannot apply null system save game");
		return;
	}

	SaveGame->Apply();
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncLoadUserData(const FString& SlotName, FOnLoadCompletedDelegate LoadedDelegate)
{
	if (!UShidenUserSaveGame::IsValidSlotName(SlotName))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		LoadedDelegate.ExecuteIfBound(false);
		return;
	}

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SlotName, LoadedDelegate]
	{
		const bool bExists = UShidenUserSaveGame::DoesExist(SlotName);
		if (!bExists)
		{
			AsyncTask(ENamedThreads::GameThread, [LoadedDelegate]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				LoadedDelegate.ExecuteIfBound(false);
			});
			return;
		}

		const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenUserSaveGame::GetOrCreate(SlotName);
		SaveGameInstance->AddToRoot();
		AsyncTask(ENamedThreads::GameThread, [SaveGameInstance, LoadedDelegate]
		{
			SaveGameInstance->Apply();
			SaveGameInstance->RemoveFromRoot();
			// ReSharper disable once CppExpressionWithoutSideEffects
			LoadedDelegate.ExecuteIfBound(true);
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncLoadSystemData(FOnLoadCompletedDelegate LoadedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [LoadedDelegate]
	{
		const bool bExists = UShidenSystemSaveGame::DoesExist();
		if (!bExists)
		{
			AsyncTask(ENamedThreads::GameThread, [LoadedDelegate]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				LoadedDelegate.ExecuteIfBound(false);
			});
			return;
		}

		const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UShidenSystemSaveGame::GetOrCreate();
		SaveGameInstance->AddToRoot();
		AsyncTask(ENamedThreads::GameThread, [SaveGameInstance, LoadedDelegate]
		{
			SaveGameInstance->Apply();
			SaveGameInstance->RemoveFromRoot();
			// ReSharper disable once CppExpressionWithoutSideEffects
			LoadedDelegate.ExecuteIfBound(true);
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncLoadPredefinedSystemData(const TObjectPtr<UObject>& WorldContextObject, FOnLoadCompletedDelegate LoadedDelegate)
{
	if (!WorldContextObject)
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		LoadedDelegate.ExecuteIfBound(false);
		return;
	}

	WorldContextObject->AddToRoot();
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [WorldContextObject, LoadedDelegate]
	{
		const bool bExists = UShidenPredefinedSystemSaveGame::DoesExist();
		if (!bExists)
		{
			WorldContextObject->RemoveFromRoot();
			AsyncTask(ENamedThreads::GameThread, [LoadedDelegate]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				LoadedDelegate.ExecuteIfBound(false);
			});
			return;
		}

		const TObjectPtr<UShidenPredefinedSystemSaveGame> SaveGameInstance = UShidenPredefinedSystemSaveGame::GetOrCreate();
		AsyncTask(ENamedThreads::GameThread, [SaveGameInstance, WorldContextObject, LoadedDelegate]
		{
			SaveGameInstance->Apply(WorldContextObject);
			WorldContextObject->RemoveFromRoot();
			// ReSharper disable once CppExpressionWithoutSideEffects
			LoadedDelegate.ExecuteIfBound(true);
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncRetrieveUserData(const FString& SlotName, FOnRetrieveUserDataCompletedDelegate RetrievedDelegate)
{
	if (!UShidenUserSaveGame::IsValidSlotName(SlotName))
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		RetrievedDelegate.ExecuteIfBound(false, nullptr);
		return;
	}

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SlotName, RetrievedDelegate]
	{
		const bool bExists = UShidenUserSaveGame::DoesExist(SlotName);
		if (!bExists)
		{
			AsyncTask(ENamedThreads::GameThread, [RetrievedDelegate]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				RetrievedDelegate.ExecuteIfBound(false, nullptr);
			});
			return;
		}

		const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenUserSaveGame::GetOrCreate(SlotName);
		AsyncTask(ENamedThreads::GameThread, [SaveGameInstance, RetrievedDelegate]
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			RetrievedDelegate.ExecuteIfBound(true, SaveGameInstance.Get());
		});
	});
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::AsyncRetrieveSystemData(FOnRetrieveSystemDataCompletedDelegate RetrievedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [RetrievedDelegate]
	{
		const bool bExists = UShidenSystemSaveGame::DoesExist();
		if (!bExists)
		{
			AsyncTask(ENamedThreads::GameThread, [RetrievedDelegate]
			{
				// ReSharper disable once CppExpressionWithoutSideEffects
				RetrievedDelegate.ExecuteIfBound(false, nullptr);
			});
			return;
		}

		const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UShidenSystemSaveGame::GetOrCreate();
		AsyncTask(ENamedThreads::GameThread, [SaveGameInstance, RetrievedDelegate]
		{
			// ReSharper disable once CppExpressionWithoutSideEffects
			RetrievedDelegate.ExecuteIfBound(true, SaveGameInstance.Get());
		});
	});
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryGetLatestUserSaveSlotName(FString& SlotName)
{
	WaitUntilEmpty();

	if (!DoesAnyUserDataExist())
	{
		SHIDEN_WARNING("User data does not exist.");
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

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryLoadSystemData()
{
	WaitUntilEmpty();

	if (!UShidenSystemSaveGame::DoesExist())
	{
		SHIDEN_WARNING("System data does not exist");
		return false;
	}

	UShidenSystemSaveGame::GetOrCreate()->Apply();
	return true;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::TryLoadPredefinedSystemData(const UObject* WorldContextObject)
{
	WaitUntilEmpty();

	if (!UShidenPredefinedSystemSaveGame::DoesExist())
	{
		SHIDEN_WARNING("Predefined system data does not exist");
		return false;
	}

	UShidenPredefinedSystemSaveGame::GetOrCreate()->Apply(WorldContextObject);
	return true;
}

void UShidenSaveBlueprintLibrary::DeleteUserData(const FString& SlotName)
{
	WaitUntilEmpty();

	if (UShidenUserSaveGame::DoesExist(SlotName))
	{
		UShidenUserSaveGame::TryDelete(SlotName);
	}

	if (UShidenSaveSlotsSaveGame::DoesExist())
	{
		UShidenSaveSlotsSaveGame::TryDelete(SlotName);
	}
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::DeleteSystemData()
{
	WaitUntilEmpty();
	UShidenSystemSaveGame::TryDelete();
}

void UShidenSaveBlueprintLibrary::DeletePredefinedSystemData()
{
	WaitUntilEmpty();
	UShidenPredefinedSystemSaveGame::TryDelete();
}

bool UShidenSaveBlueprintLibrary::DoesAnyUserDataExist()
{
	WaitUntilEmpty();
	return UShidenSaveSlotsSaveGame::DoesExist() && AcquireSaveSlots().Num() > 0;
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::DoesUserDataExist(const FString& SlotName)
{
	WaitUntilEmpty();
	return UShidenUserSaveGame::DoesExist(SlotName);
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::DoesSystemDataExist()
{
	WaitUntilEmpty();
	return UShidenSystemSaveGame::DoesExist();
}

SHIDENCORE_API bool UShidenSaveBlueprintLibrary::DoesPredefinedSystemDataExist()
{
	WaitUntilEmpty();
	return UShidenPredefinedSystemSaveGame::DoesExist();
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::ClearLoadedSystemData()
{
	UShidenVariableBlueprintLibrary::ResetSystemVariables();
}

SHIDENCORE_API void UShidenSaveBlueprintLibrary::ClearLoadedPredefinedSystemData(const UObject* WorldContextObject)
{
	UShidenVariableBlueprintLibrary::ResetPredefinedSystemVariables(WorldContextObject);
}

void UShidenSaveBlueprintLibrary::ClearLoadedUserData()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	UShidenVariableBlueprintLibrary::ResetUserVariables();
	UShidenVariableBlueprintLibrary::ResetAllLocalVariables();
	ShidenSubsystem->ScenarioProperties.Empty();
	ShidenSubsystem->ScenarioProgressStack.Empty();
}
