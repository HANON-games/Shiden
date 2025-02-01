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
	const int32 ParsedLength = UShidenCoreFunctionLibrary::GetParsedLength(RawText);
	const TMap<FString, FShidenTextType> TextTypes = UShidenCoreFunctionLibrary::GetShidenTextTypes();

	if (TextTypes.Contains(TextType) && TextTypes.FindRef(TextType).bShouldShowClickWaitingGlyph && ParsedLength <= Length)
	{
		const UShidenSubsystem* ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

		check(ShidenSubsystem);
		
		ParsedText = ParsedText.Append(ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph);
	}

	if (const TObjectPtr<URichTextBlock> RichTextBlock = RichTextBlocks.FindRef(TextType))
	{
		if (RawText.IsEmpty())
		{
			OriginalTexts.Remove(TextType);
		}
		else
		{
			OriginalTexts.Add(TextType, RawText);
		}
		CurrentLength = Length;
		RichTextBlock->SetText(FText::FromString(ParsedText));
		bResult = true;
	}
	else if (const TObjectPtr<UTextBlock> TextBlock = TextBlocks.FindRef(TextType))
	{
		if (RawText.IsEmpty())
		{
			OriginalTexts.Remove(TextType);
		}
		else
		{
			OriginalTexts.Add(TextType, RawText);
		}
		CurrentLength = Length;
		TextBlock->SetText(FText::FromString(ParsedText));
		bResult = true;
	}
}

SHIDENCORE_API void UShidenTextWidget::ClearText_Implementation(const FString& TextType, bool& bResult)
{
	SetText(TextType, TEXT(""), 0, bResult);
}

SHIDENCORE_API void UShidenTextWidget::ClearAllTexts_Implementation()
{
	TArray<FString> TextTypes;
	UShidenCoreFunctionLibrary::GetShidenTextTypes().GetKeys(TextTypes);
	bool bResult = false;
	for (const FString& TextType : TextTypes)
	{
		ClearText(TextType, bResult);
	}
}

SHIDENCORE_API void UShidenTextWidget::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> Children;
	WidgetTree->GetAllWidgets(Children);
	for (const TObjectPtr<UWidget> Child : Children)
	{
		if (TObjectPtr<URichTextBlock> RichTextBlock = Cast<URichTextBlock>(Child))
		{
			RichTextBlocks.Add(Child->GetName(), RichTextBlock);
		}
		else if (TObjectPtr<UTextBlock> TextBlock = Cast<UTextBlock>(Child))
		{
			TextBlocks.Add(Child->GetName(), TextBlock);
		}
	}
}
