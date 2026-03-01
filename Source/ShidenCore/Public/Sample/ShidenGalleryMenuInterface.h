// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenGalleryMenuInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenGalleryMenuInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnSceneSelectedDelegate, const int32, PageIndex, const UShidenScenario*, Scenario);

DECLARE_DYNAMIC_DELEGATE(FOnReturnToTitleSelectedDelegate);

class SHIDENCORE_API IShidenGalleryMenuInterface
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the gallery menu widget with a callback delegate for scene selection.
	 * @param PageIndex The page number to initialize the gallery menu with, starting from 0
	 * @param OnSceneSelected Delegate called when a scene is selected in the gallery, with the current page index and selected scenario
	 * @param OnReturnToTitleSelected Delegate called when the "Return to Title" option is selected in the gallery
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnSceneSelected"))
	void InitGalleryMenu(const int32 PageIndex, const FOnSceneSelectedDelegate& OnSceneSelected, const FOnReturnToTitleSelectedDelegate& OnReturnToTitleSelected);
};
