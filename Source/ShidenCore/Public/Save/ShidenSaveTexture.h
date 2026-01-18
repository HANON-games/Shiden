// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "PixelFormat.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "ShidenSaveTexture.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenSaveTexture
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Save Game")
	int32 Width = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Save Game")
	int32 Height = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Save Game")
	TEnumAsByte<EPixelFormat> Format = PF_R8G8B8A8;

	UPROPERTY(BlueprintReadOnly, Category = "Shiden Visual Novel|Save Game")
	TArray<uint8> Pixels = TArray<uint8>();

	explicit FShidenSaveTexture(UTexture2D* Texture);

	FShidenSaveTexture() = default;
};
