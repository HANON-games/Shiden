// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Math/Color.h"
#include "ShidenCommandDefinition.generated.h"

USTRUCT(BlueprintType)
struct FShidenCommandArgument
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FName ArgName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString DefaultValue;
};

USTRUCT(BlueprintType)
struct FShidenCommandDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FText Note;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FLinearColor FontColor = FLinearColor(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FLinearColor ItemNormalColor = FLinearColor(0.2f, 0.2f, 0.2f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FLinearColor ItemHoveredColor = FLinearColor(0.35f, 0.35f, 0.35f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FLinearColor ItemPressedColor = FLinearColor(0.35f, 0.35f, 0.35f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	bool bCanCallInMacro = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/Engine.Blueprint"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath CommandBlueprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TArray <FShidenCommandArgument> Args;
};
