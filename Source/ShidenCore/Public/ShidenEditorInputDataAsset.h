// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "UObject/NoExportTypes.h"
#include "Engine/DataAsset.h"
#include "ShidenEditorInputDataAsset.generated.h"

UCLASS(Blueprintable)
class SHIDENCORE_API UShidenEditorInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal", Meta = (AllowedClasses = "/Script/Blutility.EditorUtilityWidgetBlueprint", ExactClass = false))
	FSoftObjectPath TemplateWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SvnInternal")
	TMap<FString, FString> Parameters = TMap<FString, FString>();
};
