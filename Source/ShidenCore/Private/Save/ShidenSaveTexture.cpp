// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Save/ShidenSaveTexture.h"

FShidenSaveTexture::FShidenSaveTexture(UTexture2D* Texture)
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
