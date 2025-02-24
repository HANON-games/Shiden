// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "ShidenSaveSlotsSaveGame.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ShidenSaveFunctionLibrary.generated.h"

using namespace UE::Tasks;

DECLARE_DELEGATE_OneParam(FAsyncSaveDataDelegate, bool)

UCLASS()
class SHIDENCORE_API UShidenSaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	static FPipe SaveGamePipe;

	static void WaitUntilEmpty();

public:
	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Utility")
	static UTexture2D* ConvertSaveTextureToTexture2D(const FShidenSaveTexture& SaveTexture);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game", meta = (AutoCreateRefTerm = "SlotMetadata"))
	static void SaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, bool& bSuccess);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void SaveSystemData(bool& bSuccess);

	static void AsyncSaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, FAsyncSaveDataDelegate SavedDelegate = FAsyncSaveDataDelegate());

	static void AsyncSaveSystemData(FAsyncSaveDataDelegate SavedDelegate = FAsyncSaveDataDelegate());

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static TMap<FString, FShidenSaveSlot>& AcquireSaveSlots();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void LoadUserData(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game",  meta = (WorldContext = "WorldContextObject"))
	static void LoadSystemData(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeleteUserData(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static void DeleteSystemData();

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesUserDataExist(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "Shiden Visual Novel|Save Game")
	static bool DoesSystemDataExist();
};
