// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenGalleryMenuInterface.h"
#include "ShidenGalleryScene.h"
#include "UObject/Interface.h"
#include "ShidenGallerySceneInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenGallerySceneInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenGallerySceneInterface
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the gallery scene widget with a callback delegate for scene selection.
	 * @param PageIndex The page number this scene belongs to (0-based index)
	 * @param Scenario The gallery scene data to display
	 * @param OnSceneSelected Delegate called when this scene is selected, with the current page index and selected scenario
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnSceneSelected"))
	void InitGalleryScene(const int32 PageIndex, const FShidenGalleryScene Scenario, const FOnSceneSelectedDelegate& OnSceneSelected);

	/**
	 * Returns the gallery scene data associated with this widget.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	FShidenGalleryScene GetGalleryScene();

	/**
	 * Selects this scenario and triggers the OnSceneSelected callback.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget")
	void SelectScenario();
};
