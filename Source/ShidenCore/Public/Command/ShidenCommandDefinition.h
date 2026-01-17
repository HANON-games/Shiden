// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Math/Color.h"
#include "ShidenCommandDefinition.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenConditionalMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command|Editor")
	FString Condition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command|Editor")
	FText Message;
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCommandArgumentEditorSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command",
		Meta = (AllowedClasses = "/Script/Blutility.EditorUtilityWidgetBlueprint", ExactClass = false))
	FSoftObjectPath TemplateWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TMap<FString, FString> TemplateParameters = TMap<FString, FString>();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString VisibilityCondition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TArray<FShidenConditionalMessage> WarningMessages;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TArray<FShidenConditionalMessage> ErrorMessages;
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCommandArgument
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FName ArgName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FString DefaultValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	bool bIsAssetToBeLoaded = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command|Editor")
	FShidenCommandArgumentEditorSettings EditorSettings;
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCommandStyle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Style")
	FLinearColor FontColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Style")
	FLinearColor ItemNormalColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Style")
	FLinearColor ItemHoveredColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Command Style")
	FLinearColor ItemPressedColor;

	FShidenCommandStyle()
		: FontColor(FLinearColor(1.0f, 1.0f, 1.0f))
		  , ItemNormalColor(FLinearColor(0.2f, 0.2f, 0.2f))
		  , ItemHoveredColor(FLinearColor(0.35f, 0.35f, 0.35f))
		  , ItemPressedColor(FLinearColor(0.35f, 0.35f, 0.35f))
	{
	}

	FShidenCommandStyle(const FLinearColor FontColor, const FLinearColor ItemNormalColor, const FLinearColor ItemHoveredColor,
	                    const FLinearColor ItemPressedColor)
		: FontColor(FontColor)
		  , ItemNormalColor(ItemNormalColor)
		  , ItemHoveredColor(ItemHoveredColor)
		  , ItemPressedColor(ItemPressedColor)
	{
	}
};

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenCommandDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FText Note;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Editor")
	FShidenCommandStyle Style;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	bool bCanCallInMacro = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	FSoftObjectPath CommandSoftObjectPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Command")
	TArray<FShidenCommandArgument> Args;
};
