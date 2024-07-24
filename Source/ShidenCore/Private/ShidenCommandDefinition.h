// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Math/Color.h"
#include "ShidenCommandDefinition.generated.h"

USTRUCT(BlueprintType)
struct FShidenCommandDefinition
{
	GENERATED_BODY()

public:
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
	FText Arg1Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg1InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg1DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg2Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg2InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg2DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg3Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg3InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg3DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg4Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg4InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg4DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg5Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg5InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg5DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg6Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg6InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg6DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg7Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg7InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg7DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg8Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg8InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg8DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg9Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg9InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg9DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText Arg10Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowedClasses = "/Script/ShidenCore.ShidenEditorInputDataAsset"), Category = "Shiden Visual Novel|Command")
	FSoftObjectPath Arg10InputType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString Arg10DefaultValue;
};
