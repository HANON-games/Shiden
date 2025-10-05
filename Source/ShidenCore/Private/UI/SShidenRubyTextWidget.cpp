// Copyright (c) 2025 HANON. All Rights Reserved.

#include "UI/SShidenRubyTextWidget.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Styling/CoreStyle.h"

void SShidenRubyTextWidget::Construct(const FArguments& InArgs)
{
	FTextBlockStyle RubyStyle;
	if (InArgs._RubyTextStyle)
	{
		RubyStyle = *InArgs._RubyTextStyle;
	}
	else if (InArgs._BaseTextStyle)
	{
		RubyStyle = *InArgs._BaseTextStyle;
		RubyStyle.Font.Size = FMath::Max(8, FMath::RoundToInt(InArgs._BaseTextStyle->Font.Size * InArgs._SizeCoefficient));
	}

	const bool bShouldShowRubyText = !InArgs._RubyText.IsEmpty() && (InArgs._bDisplayRubyOnFirstCharacter || InArgs._bContainsAllBaseText);
	
	const float CalculatedRubyHeight = InArgs._BaseTextStyle
		? InArgs._BaseTextStyle->Font.Size * InArgs._SizeCoefficient
		: 12.0f * InArgs._SizeCoefficient;

	ChildSlot
	[
		SNew(SOverlay)
		
		// Ruby text layer (always present but may be collapsed)  
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Top)
		.Padding(0, -(CalculatedRubyHeight + 6.0f) + InArgs._OffsetCoefficient * (InArgs._BaseTextStyle ? InArgs._BaseTextStyle->Font.Size : 12.0f), 0, 0)
		[
			SAssignNew(RubyTextBlock, STextBlock)
			.Text(InArgs._RubyText)
			.TextStyle(&RubyStyle)
			.Justification(ETextJustify::Center)
			.Visibility(bShouldShowRubyText ? EVisibility::Visible : EVisibility::Collapsed)
		]
		
		// Base text layer - positioned to match hidden reference exactly
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Bottom)
		[
			SAssignNew(BaseTextBlock, STextBlock)
			.Text(InArgs._BaseText)
			.TextStyle(InArgs._BaseTextStyle ? InArgs._BaseTextStyle : &FCoreStyle::Get().GetWidgetStyle<FTextBlockStyle>("NormalText"))
		]
	];
}
