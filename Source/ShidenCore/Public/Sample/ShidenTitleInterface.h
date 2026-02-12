// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenTitleInterface.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnNewGameDelegate);

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnLoadGameDelegate, const FString&, SlotName);

DECLARE_DYNAMIC_DELEGATE(FOnGallerySelectedDelegate);

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenTitleInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenTitleInterface
{
	GENERATED_BODY()

public:
	/**
	 * Initializes the title screen widget with callback delegates for new game and load game actions.
	 * @param OnNewGameStarted Delegate called when starting a new game
	 * @param OnGameLoaded Delegate called when loading an existing game with the specified slot name
	 * @param OnGallerySelected Delegate called when selecting the gallery option
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "OnNewGameStarted,OnGameLoaded,OnGallerySceneSelected"))
	void InitTitle(const FOnNewGameDelegate& OnNewGameStarted, const FOnLoadGameDelegate& OnGameLoaded, const FOnGallerySelectedDelegate& OnGallerySelected);
};
