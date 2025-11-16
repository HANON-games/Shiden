// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Text/STextBlock.h"

/**
 * Slate widget that displays ruby text (furigana) above base text
 * Used for Japanese text with reading annotations
 */
class SHIDENCORE_API SShidenRubyTextWidget final : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SShidenRubyTextWidget)
			: _BaseText(FText::GetEmpty())
			  , _RubyText(FText::GetEmpty())
			  , _BaseTextStyle(nullptr)
			  , _RubyTextStyle(nullptr)
			  , _SizeCoefficient(0.1f)
			  , _OffsetCoefficient(-0.05f)
			  , _bContainsAllBaseText(false)
			  , _bDisplayRubyOnFirstCharacter(false)
		{
		}

		SLATE_ARGUMENT(FText, BaseText)
		SLATE_ARGUMENT(FText, RubyText)
		SLATE_ARGUMENT(const FTextBlockStyle*, BaseTextStyle)
		SLATE_ARGUMENT(const FTextBlockStyle*, RubyTextStyle)
		SLATE_ARGUMENT(float, SizeCoefficient)
		SLATE_ARGUMENT(float, OffsetCoefficient)
		SLATE_ARGUMENT(bool, bContainsAllBaseText)
		SLATE_ARGUMENT(bool, bDisplayRubyOnFirstCharacter)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

protected:
	TSharedPtr<STextBlock> BaseTextBlock;
	TSharedPtr<STextBlock> RubyTextBlock;
};
