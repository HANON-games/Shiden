// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ShidenAddNewContextMenu.generated.h"

UCLASS()
class SHIDENEDITOR_API UShidenAddNewContextMenu : public UToolMenuEntryScript
{
	GENERATED_BODY()

public:
	static void RegisterMenuExtensions(IModuleInterface* Owner);

private:
	static UObject* CreateAsset(const FString& Path, const FString& NewAssetName);

	static UObject* CreateAssetFromClass(UClass* AssetClass, const FString& NewAssetName);

	static bool TryDecideNewPackageName(const FString& NewAssetName, FString& OutAssetPath);
};
