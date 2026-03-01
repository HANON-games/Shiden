// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenGalleryMenuInterface.h"
#include "UObject/Interface.h"
#include "ShidenGalleryPageInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenGalleryPageInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenGalleryPageInterface
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the gallery page widget with a callback delegate for scene selection.
	 * @param PageIndex The page number to display initially (0-based index)
	 * @param OnSceneSelected Delegate called when a scene is selected in the gallery, with the current page index and selected scenario
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnSceneSelected"))
	void InitGalleryPage(const int32 PageIndex, const FOnSceneSelectedDelegate& OnSceneSelected);
};
