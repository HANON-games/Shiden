// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "ShidenEditorInputDataAsset.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenEditorInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal", Meta = (AllowedClasses = "/Script/Blutility.EditorUtilityWidgetBlueprint", ExactClass = false))
	FSoftObjectPath TemplateWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Internal")
	TMap< FString, FString > Parameters = TMap< FString, FString >();
};
