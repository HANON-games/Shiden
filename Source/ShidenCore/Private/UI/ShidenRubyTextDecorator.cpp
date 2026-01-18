// Copyright (c) 2026 HANON. All Rights Reserved.

#include "UI/ShidenRubyTextDecorator.h"
#include "UI/SShidenRubyTextWidget.h"

class FShidenRichTextRubyDecorator final : public FRichTextDecorator
{
	float DefaultSizeCoefficient;
	float DefaultOffsetCoefficient;
	bool bDisplayRubyOnFirstCharacter = false;

public:
	explicit FShidenRichTextRubyDecorator(URichTextBlock* InOwner, const float InDefaultSizeCoefficient = 0.7f, const float InDefaultOffsetCoefficient = 0.0f, const bool bInDisplayRubyOnFirstCharacter = false)
		: FRichTextDecorator(InOwner)
		  , DefaultSizeCoefficient(InDefaultSizeCoefficient)
		  , DefaultOffsetCoefficient(InDefaultOffsetCoefficient)
		  , bDisplayRubyOnFirstCharacter(bInDisplayRubyOnFirstCharacter)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		// Support tags in format: <ruby t="るび" size="0.7" offset="0.1" all="true">ルビ</>
		return RunParseResult.Name == TEXT("ruby") && RunParseResult.MetaData.Contains(TEXT("t"));
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		const FText BaseText = RunInfo.Content;

		const FString* RubyTextPtr = RunInfo.MetaData.Find(TEXT("t"));
		const FText RubyText = RubyTextPtr ? FText::FromString(*RubyTextPtr) : FText::GetEmpty();

		float SizeCoefficient = DefaultSizeCoefficient;
		const FString* SizePtr = RunInfo.MetaData.Find(TEXT("size"));
		if (SizePtr && !SizePtr->IsEmpty())
		{
			SizeCoefficient = FMath::Clamp(FCString::Atof(**SizePtr), 0.1f, 2.0f);
		}

		float OffsetCoefficient = DefaultOffsetCoefficient;
		const FString* OffsetPtr = RunInfo.MetaData.Find(TEXT("offset"));
		if (OffsetPtr && !OffsetPtr->IsEmpty())
		{
			OffsetCoefficient = FMath::Clamp(FCString::Atof(**OffsetPtr), -1.0f, 1.0f);
		}

		const FString* AllPtr = RunInfo.MetaData.Find(TEXT("all"));
		const bool bContainsAllBaseText = !AllPtr || AllPtr->ToBool();

		return SNew(SShidenRubyTextWidget)
			.BaseText(BaseText)
			.RubyText(RubyText)
			.BaseTextStyle(&TextStyle)
			.SizeCoefficient(SizeCoefficient)
			.OffsetCoefficient(OffsetCoefficient)
			.bContainsAllBaseText(bContainsAllBaseText)
			.bDisplayRubyOnFirstCharacter(bDisplayRubyOnFirstCharacter);
	}
};

/////////////////////////////////////////////////////
// UShidenRubyTextDecorator

UShidenRubyTextDecorator::UShidenRubyTextDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> UShidenRubyTextDecorator::CreateDecorator(URichTextBlock* Owner)
{
	return MakeShareable(new FShidenRichTextRubyDecorator(Owner, DefaultSizeCoefficient, DefaultOffsetCoefficient, bDisplayRubyOnFirstCharacter));
}
