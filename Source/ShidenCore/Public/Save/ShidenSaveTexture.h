// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "PixelFormat.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "ShidenSaveTexture.generated.h"

USTRUCT(BlueprintType, Category = "Shiden Visual Novel|Save Game")
struct FShidenSaveTexture
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

	explicit FShidenSaveTexture(UTexture2D* Texture)
	{
		if (!Texture)
		{
			return;
		}

		Width = Texture->GetSizeX();
		Height = Texture->GetSizeY();
		Format = Texture->GetPixelFormat();

		FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];
		const void* TextureData = Mip.BulkData.Lock(LOCK_READ_ONLY);
		Pixels.SetNum(Mip.BulkData.GetBulkDataSize());
		FMemory::Memcpy(Pixels.GetData(), TextureData, Pixels.Num());
		Mip.BulkData.Unlock();
	}

	FShidenSaveTexture()
	{
	}
};
