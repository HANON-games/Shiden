// Copyright (c) 2024 HANON. All Rights Reserved.

#include "UI/ShidenTextWidget.h"

#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Utility/ShidenCoreFunctionLibrary.h"
#include "Variable/ShidenVariableFunctionLibrary.h"

SHIDENCORE_API void UShidenTextWidget::GetAllFullTexts_Implementation(TMap<FString, FString>& Texts)
{
	Texts = OriginalTexts;
}

SHIDENCORE_API void UShidenTextWidget::GetFullText_Implementation(const FString& TextType, FString& Text, bool& bResult)
{
	bResult = OriginalTexts.Contains(TextType);
	if (!bResult)
	{
		Text = TEXT("");
		return;
	}
	Text = OriginalTexts.FindRef(TextType);
}

SHIDENCORE_API void UShidenTextWidget::GetCurrentText_Implementation(const FString& TextType, FString& Text, bool& bResult)
{
	bResult = OriginalTexts.Contains(TextType);
	if (!bResult)
	{
		Text = TEXT("");
		return;
	}
	const FString& Original = OriginalTexts.FindRef(TextType);
	Text = UShidenCoreFunctionLibrary::GetCharactersWithParsedLength(Original, CurrentLength);
}

SHIDENCORE_API void UShidenTextWidget::SetText_Implementation(const FString& TextType, const FString& RawText, const int Length, bool& bResult)
{
	bResult = false;
	FString ParsedText = UShidenCoreFunctionLibrary::GetCharactersWithParsedLength(RawText, Length);
	const auto ParsedLength = UShidenCoreFunctionLibrary::GetParsedLength(RawText);
	const auto TextTypes = UShidenCoreFunctionLibrary::GetShidenTextTypes();

	if (TextTypes.Contains(TextType) && TextTypes.FindRef(TextType).bShouldShowClickWaitingGlyph && ParsedLength <= Length)
	{
		FString ClickWaitingGlyph;
		UShidenVariableFunctionLibrary::GetClickWaitingGlyph(ClickWaitingGlyph);
		ParsedText = ParsedText.Append(ClickWaitingGlyph);
	}	

	if (const TObjectPtr<URichTextBlock> RichTextBlock = RichTextBlocks.FindRef(TextType))
	{
		OriginalTexts.Add(TextType, RawText);
		CurrentLength = Length;
		RichTextBlock->SetText(FText::FromString(ParsedText));
		bResult = true;
	}
	else if (const TObjectPtr<UTextBlock> TextBlock = TextBlocks.FindRef(TextType))
	{
		OriginalTexts.Add(TextType, RawText);
		CurrentLength = Length;
		TextBlock->SetText(FText::FromString(ParsedText));
		bResult = true;
	}
}

SHIDENCORE_API void UShidenTextWidget::ClearText_Implementation(const FString& TextType, bool& bResult)
{
	bResult = OriginalTexts.Remove(TextType) > 0;
	if (bResult)
	{
		if (const TObjectPtr<URichTextBlock> RichTextBlock = RichTextBlocks.FindRef(TextType))
		{
			RichTextBlock->SetText(FText::FromString(TEXT("")));
			return;
		}

		if (const TObjectPtr<UTextBlock> TextBlock = TextBlocks.FindRef(TextType))
		{
			TextBlock->SetText(FText::FromString(TEXT("")));
			return;
		}
		
		bResult = false;
	}
}

SHIDENCORE_API void UShidenTextWidget::ClearAllText_Implementation()
{
	OriginalTexts.Empty();
	for (const auto& RichTextBlock : RichTextBlocks)
	{
		if (const TObjectPtr<URichTextBlock> RichTextBlockValue = RichTextBlock.Value)
		{
			RichTextBlockValue->SetText(FText::FromString(TEXT("")));
		}
	}
	for (const auto& TextBlock : TextBlocks)
	{
		if (const TObjectPtr<UTextBlock> TextBlockValue = TextBlock.Value)
		{
			TextBlockValue->SetText(FText::FromString(TEXT("")));
		}
	}
}

SHIDENCORE_API void UShidenTextWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	TArray<UWidget*> Children;
	WidgetTree->GetAllWidgets(Children);
	for (TObjectPtr <UWidget> Child : Children) {
		if (TObjectPtr <URichTextBlock> RichTextBlock = Cast<URichTextBlock>(Child)) {
			RichTextBlocks.Add(Child->GetName(), RichTextBlock);
		}
		else if (TObjectPtr <UTextBlock> TextBlock = Cast<UTextBlock>(Child)) {
			TextBlocks.Add(Child->GetName(), TextBlock);
		}
	}
}
