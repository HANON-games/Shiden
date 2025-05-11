// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "GameFramework/SaveGame.h"
#include "ShidenSaveTexture.h"
#include "ShidenSaveSlotsSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FShidenSaveSlot
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

	FShidenSaveSlot()
	{
	}
};

UCLASS(Category = "Shiden Visual Novel|Save Game")
class SHIDENCORE_API UShidenSaveSlotsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FString, FShidenSaveSlot> SaveSlots;
};
