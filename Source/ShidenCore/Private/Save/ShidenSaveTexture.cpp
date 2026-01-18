// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Save/ShidenSaveTexture.h"

FShidenSaveTexture::FShidenSaveTexture(UTexture2D* Texture)
{
	if (!Texture)
	{
		return;
	}

	FTexturePlatformData* PlatformData = Texture->GetPlatformData();
	if (!PlatformData || PlatformData->Mips.Num() == 0)
	{
		return;
	}

	Width = Texture->GetSizeX();
	Height = Texture->GetSizeY();
	Format = Texture->GetPixelFormat();

	FTexture2DMipMap& Mip = PlatformData->Mips[0];
	const void* TextureData = Mip.BulkData.Lock(LOCK_READ_ONLY);
	Pixels.SetNum(Mip.BulkData.GetBulkDataSize());
	FMemory::Memcpy(Pixels.GetData(), TextureData, Pixels.Num());
	Mip.BulkData.Unlock();
}
