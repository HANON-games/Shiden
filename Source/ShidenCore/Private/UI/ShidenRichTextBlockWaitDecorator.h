// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockDecorator.h"
#include "ShidenRichTextBlockWaitDecorator.generated.h"

class ISlateStyle;

/**
 * Understands the format <wait time="1.1"/>
 */
UCLASS(Abstract, Blueprintable, MinimalAPI)
class UShidenRichTextBlockWaitDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	SHIDENCORE_API explicit UShidenRichTextBlockWaitDecorator(const FObjectInitializer& ObjectInitializer);

	SHIDENCORE_API virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
};