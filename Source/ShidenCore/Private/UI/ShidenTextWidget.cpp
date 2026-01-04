// Copyright (c) 2025 HANON. All Rights Reserved.

#include "UI/ShidenTextWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "System/ShidenBlueprintLibrary.h"
#include "Variable/ShidenVariableBlueprintLibrary.h"

SHIDENCORE_API void UShidenTextWidget::GetAllFullTexts_Implementation(TMap<FString, FString>& Texts)
{
	Texts = OriginalTexts;
}

SHIDENCORE_API void UShidenTextWidget::GetFullText_Implementation(const FString& TextType, FString& Text, bool& bSuccess)
{
	if (const FString* OriginalText = OriginalTexts.Find(TextType))
	{
		Text = *OriginalText;
		bSuccess = true;
		return;
	}

	Text = TEXT("");
	bSuccess = false;
}

SHIDENCORE_API void UShidenTextWidget::GetCurrentText_Implementation(const FString& TextType, FString& Text, bool& bSuccess)
{
	if (const FString* OriginalText = OriginalTexts.Find(TextType))
	{
		Text = UShidenBlueprintLibrary::GetCharactersWithParsedLength(*OriginalText, CurrentLength);
		bSuccess = true;
		return;
	}

	Text = TEXT("");
	bSuccess = false;
}

SHIDENCORE_API void UShidenTextWidget::OpenWindow_Implementation(const FString& TextType, const FShidenOpenTextWindowDelegate& OnOpened, bool& bSuccess)
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	OnOpened.ExecuteIfBound();
	bSuccess = true;
}

SHIDENCORE_API void UShidenTextWidget::CloseWindow_Implementation(const FString& TextType, const FShidenCloseTextWindowDelegate& OnClosed, bool& bSuccess)
{
	// ReSharper disable once CppExpressionWithoutSideEffects
	OnClosed.ExecuteIfBound();
	bSuccess = true;
}

SHIDENCORE_API void UShidenTextWidget::SetText_Implementation(const FString& TextType, const FString& RawText, const int32 Length)
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

	FString ParsedText = UShidenBlueprintLibrary::GetCharactersWithParsedLength(RawText, Length);
	const int32 ParsedLength = UShidenBlueprintLibrary::GetParsedLength(RawText);

	if (const FShidenTextType* TextTypePtr = UShidenBlueprintLibrary::GetShidenTextTypes().Find(TextType))
	{
		if (TextTypePtr->bShouldShowClickWaitingGlyph && ParsedLength <= Length)
		{
			const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
			ParsedText = ParsedText.Append(ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph);
		}
	}

	if (const TObjectPtr<URichTextBlock> RichTextBlock = RichTextBlocks.FindRef(TextType))
	{
		RichTextBlock->SetText(FText::FromString(ParsedText));
	}
	else if (const TObjectPtr<UTextBlock> TextBlock = TextBlocks.FindRef(TextType))
	{
		TextBlock->SetText(FText::FromString(ParsedText));
	}
}

SHIDENCORE_API void UShidenTextWidget::ClearText_Implementation(const FString& TextType)
{
	SetText(TextType, TEXT(""), 0);
}

SHIDENCORE_API void UShidenTextWidget::ClearAllTexts_Implementation()
{
	const TMap<FString, FShidenTextType>& TextTypes = UShidenBlueprintLibrary::GetShidenTextTypes();
	for (const TPair<FString, FShidenTextType>& Pair : TextTypes)
	{
		ClearText(Pair.Key);
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

void UShidenTextWidget::PreviewText_Implementation(const FString& TextType, const FString& Text, bool& bSuccess)
{
	if (Text.IsEmpty())
	{
		OriginalTexts.Remove(TextType);
	}
	else
	{
		OriginalTexts.Add(TextType, Text);
	}

	CurrentLength = Text.Len();

	FString ResultText = Text;

	if (const FShidenTextType* TextTypePtr = UShidenBlueprintLibrary::GetShidenTextTypes().Find(TextType))
	{
		if (TextTypePtr->bShouldShowClickWaitingGlyph)
		{
			const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
			ResultText = ResultText.Append(ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph);
		}
	}

	if (const TObjectPtr<URichTextBlock> RichTextBlock = RichTextBlocks.FindRef(TextType))
	{
		RichTextBlock->SetText(FText::FromString(ResultText));
		bSuccess = true;
		return;
	}

	if (const TObjectPtr<UTextBlock> TextBlock = TextBlocks.FindRef(TextType))
	{
		TextBlock->SetText(FText::FromString(ResultText));
		bSuccess = true;
		return;
	}

	bSuccess = false;
}
