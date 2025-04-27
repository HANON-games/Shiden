// Copyright (c) 2025 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Templates/SubclassOf.h"
#include "ShidenAsyncActionHandleSaveGame.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAsyncHandleSaveGame, bool, bSuccess);

UENUM()
enum class EShidenSaveGameOperationName : uint8
{
	SaveUserData,
	SaveSystemData,
};

UCLASS()
class SHIDENCORE_API UShidenAsyncActionHandleSaveGame : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionHandleSaveGame* AsyncSaveUserData(UObject* WorldContextObject, const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata);

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "Shiden Visual Novel|Save Game", WorldContext = "WorldContextObject"))
	static UShidenAsyncActionHandleSaveGame* AsyncSaveSystemData(UObject* WorldContextObject);

	virtual void Activate() override;

	UPROPERTY(BlueprintAssignable)
	FOnAsyncHandleSaveGame Completed;

protected:
	UPROPERTY()
	EShidenSaveGameOperationName Operation;

	UPROPERTY()
	FString SlotName;

	UPROPERTY()
	UTexture2D* Thumbnail;

	UPROPERTY()
	TMap<FString, FString> SlotMetadata;

	virtual void ExecuteCompleted(bool bSuccess);
};
