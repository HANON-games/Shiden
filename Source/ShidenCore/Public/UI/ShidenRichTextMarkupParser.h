// Copyright (c) 2024 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Text/ITextDecorator.h"
#include "Internationalization/Regex.h"
#include "Framework/Text/IRichTextMarkupParser.h"

class SHIDENCORE_API FShidenRichTextMarkupParser : public IRichTextMarkupParser
{
public:
	static TSharedRef<FShidenRichTextMarkupParser> Create();
	
	static TSharedRef<FShidenRichTextMarkupParser> GetStaticInstance();

public:
	virtual void Process(TArray<FTextLineParseResults>& Results, const FString& Input, FString& Output) override;

private:
	void ParseLineRanges(const FString& Input, const TArray<FTextRange>& LineRanges, TArray<FTextLineParseResults>& LineParseResultsArray) const;
	
	void HandleEscapeSequences(const FString& Input, TArray<FTextLineParseResults>& LineParseResultsArray, FString& ConcatenatedUnescapedLines) const;
	
	bool HideCharacters(const TArray<FTextLineParseResults>& LineParseResultsArray, FString& InOutConcatenatedLines) const;
	
	FRegexPattern EscapeSequenceRegexPattern;
	
	FRegexPattern ElementRegexPattern;
	
	FRegexPattern AttributeRegexPattern;

	bool bHiddenCharacters;

	int32 DisplayCharacterCount;

public:
	FShidenRichTextMarkupParser();

	void SetDisplayCharacterCount(int32 NewCount);
};