// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShidenEditorConfig.h"
#include "IContentBrowserDataModule.h"
#include "ContentBrowserDataSubsystem.h"
#include "Components/PanelWidget.h"
#include "ShidenEditorFunctionLibrary.generated.h"

UCLASS()
class SHIDENEDITOR_API UShidenEditorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SvnInternal|EditorUtility")
	static void CreateModalForSave(UClass* AssetClass, const FString DefaultPath, const FString DefaultAssetName, FString& SavePackagePath, FString& SaveAssetName, bool& bSuccess);

	UFUNCTION(BlueprintPure, Category = "SvnInternal|EditorUtility")
	static bool CanCreateFolder(FName InPath);
};
