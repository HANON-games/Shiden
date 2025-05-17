// Copyright (c) 2025 HANON. All Rights Reserved.

#include "UI/ShidenRichTextBlockWaitDecorator.h"

class FRichTextWaitDecorator final : public FRichTextDecorator
{
public:
	explicit FRichTextWaitDecorator(URichTextBlock* InOwner) : FRichTextDecorator(InOwner)
	{
	}

	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		return RunParseResult.Name == TEXT("wait") && RunParseResult.MetaData.Contains(TEXT("time"));
	}

protected:
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		return nullptr;
	}
};

/////////////////////////////////////////////////////
// UShidenRichTextBlockWaitDecorator

UShidenRichTextBlockWaitDecorator::UShidenRichTextBlockWaitDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TSharedPtr<ITextDecorator> UShidenRichTextBlockWaitDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichTextWaitDecorator(InOwner));
}

/////////////////////////////////////////////////////
