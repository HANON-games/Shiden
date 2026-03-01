// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "Scenario/ShidenScenario.h"
#include "ShidenGalleryScene.generated.h"

/**
 * Represents a scene entry in the gallery.
 * This struct is used in the sample implementation.
 */
USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenGalleryScene
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<USlateBrushAsset> LockedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<USlateBrushAsset> UnlockedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<UShidenScenario> Scenario;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString Condition;
};
