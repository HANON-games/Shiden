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
	bSuccess = OriginalTexts.Contains(TextType);
	if (!bSuccess)
	{
		Text = TEXT("");
		return;
	}
	Text = OriginalTexts.FindRef(TextType);
}

SHIDENCORE_API void UShidenTextWidget::GetCurrentText_Implementation(const FString& TextType, FString& Text, bool& bSuccess)
{
	bSuccess = OriginalTexts.Contains(TextType);
	if (!bSuccess)
	{
		Text = TEXT("");
		return;
	}
	const FString& Original = OriginalTexts.FindRef(TextType);
	Text = UShidenBlueprintLibrary::GetCharactersWithParsedLength(Original, CurrentLength);
}

SHIDENCORE_API void UShidenTextWidget::OpenWindow_Implementation(const FString& TextType, const FShidenOpenTextWindowDelegate& OnOpened, bool& bSuccess)
{
	bSuccess = true;
	// ReSharper disable once CppExpressionWithoutSideEffects
	OnOpened.ExecuteIfBound();
}

SHIDENCORE_API void UShidenTextWidget::CloseWindow_Implementation(const FString& TextType, const FShidenCloseTextWindowDelegate& OnClosed, bool& bSuccess)
{
	bSuccess = true;
	// ReSharper disable once CppExpressionWithoutSideEffects
	OnClosed.ExecuteIfBound();
}

SHIDENCORE_API void UShidenTextWidget::SetText_Implementation(const FString& TextType, const FString& RawText, const int Length)
{
	FString ParsedText = UShidenBlueprintLibrary::GetCharactersWithParsedLength(RawText, Length);
	const int32 ParsedLength = UShidenBlueprintLibrary::GetParsedLength(RawText);
	const TMap<FString, FShidenTextType> TextTypes = UShidenBlueprintLibrary::GetShidenTextTypes();

	if (TextTypes.Contains(TextType) && TextTypes.FindRef(TextType).bShouldShowClickWaitingGlyph && ParsedLength <= Length)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
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
	}
}

SHIDENCORE_API void UShidenTextWidget::ClearText_Implementation(const FString& TextType)
{
	SetText(TextType, TEXT(""), 0);
}

SHIDENCORE_API void UShidenTextWidget::ClearAllTexts_Implementation()
{
	TArray<FString> TextTypes;
	UShidenBlueprintLibrary::GetShidenTextTypes().GetKeys(TextTypes);
	for (const FString& TextType : TextTypes)
	{
		ClearText(TextType);
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
	bSuccess = false;
	const TMap<FString, FShidenTextType> TextTypes = UShidenBlueprintLibrary::GetShidenTextTypes();
	FString ResultText = Text;
	
	if (TextTypes.Contains(TextType) && TextTypes.FindRef(TextType).bShouldShowClickWaitingGlyph)
	{
		const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

		check(ShidenSubsystem);
		
		ResultText = ResultText.Append(ShidenSubsystem->PredefinedSystemVariable.ClickWaitingGlyph);
	}

	if (const TObjectPtr<URichTextBlock> RichTextBlock = RichTextBlocks.FindRef(TextType))
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
		RichTextBlock->SetText(FText::FromString(ResultText));
		bSuccess = true;
	}
	else if (const TObjectPtr<UTextBlock> TextBlock = TextBlocks.FindRef(TextType))
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
		TextBlock->SetText(FText::FromString(ResultText));
		bSuccess = true;
	}
}
