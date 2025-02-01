// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "UObject/Interface.h"
#include "ShidenSaveSlotInterface.generated.h"

/*
 * This is an interface used in the sample implementation.
 * It is not referenced from the core implementation of Shiden, so it is not necessarily required to use it.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UShidenSaveSlotInterface : public UInterface
{
	GENERATED_BODY()
};

class SHIDENCORE_API IShidenSaveSlotInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "DisplayName,UpdatedAtText,NameText,TalkText"))
	void InitSaveSlot(const FString& SlotName, const FText& DisplayName, const UUserWidget* SaveMenuWidget,
	                  const bool bIsSaveMode, const UTexture2D* Thumbnail, const FText& UpdatedAtText,
	                  const FText& NameText, const FText& TalkText);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Shiden Visual Novel|Widget", meta = (AutoCreateRefTerm = "DisplayName"))
	void InitNoDataSaveSlot(const FString& SlotName, const FText& DisplayName, const UUserWidget* SaveMenuWidget, const bool bIsSaveMode);
};
