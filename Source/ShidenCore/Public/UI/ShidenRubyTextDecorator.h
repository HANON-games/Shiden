// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "ShidenRubyTextDecorator.generated.h"

class ISlateStyle;

/**
 * Understands the format <ruby t="るび" size="0.7" offset="0.1">ルビ</>
 * Creates a custom widget that displays ruby text (furigana) above base text
 * 
 * Parameters:
 * - t: Ruby text (required)
 * - size: Size coefficient for ruby text (optional, range: 0.1-2.0)
 * - offset: Vertical offset coefficient (optional, range: -1.0-1.0)
 */
UCLASS(Abstract, Blueprintable, MinimalAPI)
class UShidenRubyTextDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	SHIDENCORE_API explicit UShidenRubyTextDecorator(const FObjectInitializer& ObjectInitializer);

	SHIDENCORE_API virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* Owner) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ruby Text")
	bool bDisplayRubyOnFirstCharacter = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ruby Text", meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float DefaultSizeCoefficient = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ruby Text", meta = (ClampMin = "-1.0", ClampMax = "1.0"))
	float DefaultOffsetCoefficient = -0.05f;
};
