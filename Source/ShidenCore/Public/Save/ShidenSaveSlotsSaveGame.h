// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "GameFramework/SaveGame.h"
#include "ShidenSaveTexture.h"
#include "ShidenSaveSlotsSaveGame.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenSaveSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FString SlotName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FString, FString> SlotMetadata;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenSaveTexture Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime CreatedAt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FDateTime UpdatedAt;

	FShidenSaveSlot(const FString& SlotName, const TMap<FString, FString>& SlotMetadata, const FShidenSaveTexture& Thumbnail,
	                const FDateTime CreatedAt, const FDateTime UpdatedAt)
		: SlotName(SlotName), SlotMetadata(SlotMetadata), Thumbnail(Thumbnail), CreatedAt(CreatedAt), UpdatedAt(UpdatedAt)
	{
	}

	FShidenSaveSlot() = default;
};

UCLASS(NotBlueprintable)
class SHIDENCORE_API UShidenSaveSlotsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FString, FShidenSaveSlot> SaveSlots;
	
	static TObjectPtr<UShidenSaveSlotsSaveGame> GetOrCreate();
	
	void Prepare(const FString& SlotName, const FShidenSaveTexture& SaveTexture, const TMap<FString, FString>& SaveSlotMetadata);

	bool TryCommit();

	static bool DoesExist();
	
	static bool TryDelete(const FString& SlotName);

private:
	constexpr static TCHAR SaveSlotsName[] = TEXT("ShidenSaveSlots");
};
