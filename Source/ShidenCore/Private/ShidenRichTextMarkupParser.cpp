// Copyright (c) 2024 HANON. All Rights Reserved.
// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShidenRichTextMarkupParser.h"

struct FUnescapeHelper
{
	TArray<FString> EscapeSequences;
	TArray<FString> UnescapedCharacters;

	FUnescapeHelper()
	{
		EscapeSequences.Add(TEXT("quot"));	UnescapedCharacters.Add(TEXT("\""));
		EscapeSequences.Add(TEXT("lt"));	UnescapedCharacters.Add(TEXT("<"));
		EscapeSequences.Add(TEXT("gt"));	UnescapedCharacters.Add(TEXT(">"));
		EscapeSequences.Add(TEXT("amp"));	UnescapedCharacters.Add(TEXT("&"));
	}

} static const UnescapeHelper;

struct FEscapeSequenceRegexPatternString
{
	// Generate a regular expression pattern string that matches each of the escape sequences as alternatives, each in its own capture group.
	static FString Get(const TArray<FString>& EscapeSequences)
	{
		FString EscapeSequenceRegexPatternString;

		for (const FString& EscapeSequence : EscapeSequences)
		{
			// Add alternation operator to regex.
			if (!(EscapeSequenceRegexPatternString.IsEmpty()))
			{
				EscapeSequenceRegexPatternString += TEXT("|");
			}

			// Add capture group for escape sequence.
			EscapeSequenceRegexPatternString += TEXT("(");
			EscapeSequenceRegexPatternString += TEXT("&");
			EscapeSequenceRegexPatternString += EscapeSequence;
			EscapeSequenceRegexPatternString += TEXT(";");
			EscapeSequenceRegexPatternString += TEXT(")");
		}

		return EscapeSequenceRegexPatternString;
	}
};

SHIDENCORE_API TSharedRef< FShidenRichTextMarkupParser > FShidenRichTextMarkupParser::Create()
{
	return MakeShareable(new FShidenRichTextMarkupParser());
}

SHIDENCORE_API TSharedRef< FShidenRichTextMarkupParser > FShidenRichTextMarkupParser::GetStaticInstance()
{
	static TSharedRef< FShidenRichTextMarkupParser > Parser = MakeShareable(new FShidenRichTextMarkupParser());
	return Parser;
}

SHIDENCORE_API FShidenRichTextMarkupParser::FShidenRichTextMarkupParser()
	: EscapeSequenceRegexPattern(FRegexPattern(FEscapeSequenceRegexPatternString::Get(UnescapeHelper.EscapeSequences)))
	, ElementRegexPattern(FRegexPattern(TEXT("<([\\w\\d\\.-]+)((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)|(?:>(.*?)</>))")))
	, AttributeRegexPattern(FRegexPattern(TEXT("([\\w\\d\\.]+)=(?>\"(.*?)\")")))
{

}

SHIDENCORE_API void FShidenRichTextMarkupParser::Process(
	TArray<FTextLineParseResults>& Results, const FString& Input, FString& Output)
{
	// Syntax analysis processing implemented by the engine
	TArray<FTextRange> LineRanges;
	FTextRange::CalculateLineRangesFromString(Input, LineRanges);
	ParseLineRanges(Input, LineRanges, Results);
	HandleEscapeSequences(Input, Results, Output);

	// Add processing to further format the formatted string
	bHiddenCharacters = FShidenRichTextMarkupParser::HideCharacters(Results, Output);
}

SHIDENCORE_API void FShidenRichTextMarkupParser::SetDisplayCharacterCount(int32 NewCount)
{
	// Specify the display position
	DisplayCharacterCount = NewCount;
}

SHIDENCORE_API bool FShidenRichTextMarkupParser::HideCharacters(
	const TArray<FTextLineParseResults>& LineParseResultsArray,
	FString& InOutConcatenatedLines) const
{
	bool bFoundToReplace = false;

	if (!InOutConcatenatedLines.IsEmpty() && DisplayCharacterCount != INDEX_NONE)
	{
		int32 CharIndex = 0;
		for (const FTextLineParseResults& LineParseResult : LineParseResultsArray)
		{
			for (const FTextRunParseResults& RunParseResult : LineParseResult.Runs)
			{
				FTextRange TargetRange =
					RunParseResult.ContentRange.IsEmpty() ?
					RunParseResult.OriginalRange : RunParseResult.ContentRange;

				bool bFoundInRunParseResult = false;

				if (RunParseResult.Name == TEXT("img"))
				{
					for (int32 i = 0; i < TargetRange.Len(); ++i)
					{
						int32 TargetIndex = TargetRange.BeginIndex + i;
						if (ensure(InOutConcatenatedLines.IsValidIndex(TargetIndex)))
						{
							if (CharIndex < DisplayCharacterCount)
							{
								// If it is a character to be displayed, do nothing
								continue;
							}

							// Replace the string that has not reached the display position with a space
							InOutConcatenatedLines[TargetIndex] = *TEXT("\u0020");
							bFoundInRunParseResult = true;
						}
					}
					CharIndex++;
				}
				else 
				{
					for (int32 i = 0; i < TargetRange.Len(); ++i)
					{
						int32 TargetIndex = TargetRange.BeginIndex + i;
						if (ensure(InOutConcatenatedLines.IsValidIndex(TargetIndex)))
						{
							if (CharIndex++ < DisplayCharacterCount)
							{
								// If it is a character to be displayed, do nothing
								continue;
							}

							// Replace the string that has not reached the display position with a space
							InOutConcatenatedLines[TargetIndex] = *TEXT("\u0020");
							bFoundInRunParseResult = true;
						}
					}

				}

				bFoundToReplace |= bFoundInRunParseResult;
			}
		}
	}

	return bFoundToReplace;
}

SHIDENCORE_API void FShidenRichTextMarkupParser::ParseLineRanges(const FString& Input, const TArray<FTextRange>& LineRanges, TArray<FTextLineParseResults>& LineParseResultsArray) const
{
	// Special regular expression pattern for matching rich text markup elements. IE: <ElementName AttributeName="AttributeValue">Content</>
	FRegexMatcher ElementRegexMatcher(ElementRegexPattern, Input);

	// Parse line ranges, creating line parse results and run parse results.
	for (int32 i = 0; i < LineRanges.Num(); ++i)
	{
		FTextLineParseResults LineParseResults;
		LineParseResults.Range = LineRanges[i];

		// Limit the element regex matcher to the current line.
		ElementRegexMatcher.SetLimits(LineParseResults.Range.BeginIndex, LineParseResults.Range.EndIndex);

		// Iterate through the line, each time trying to find a match for the element regex, adding it as a run and any intervening text as another run.
		int32 LastRunEnd = LineParseResults.Range.BeginIndex;
		while (ElementRegexMatcher.FindNext())
		{
			int32 ElementBegin = ElementRegexMatcher.GetMatchBeginning();
			int32 ElementEnd = ElementRegexMatcher.GetMatchEnding();

			FTextRange OriginalRange(ElementBegin, ElementEnd);

			// Capture Group 1 is the element name.
			int32 ElementNameBegin = ElementRegexMatcher.GetCaptureGroupBeginning(1);
			int32 ElementNameEnd = ElementRegexMatcher.GetCaptureGroupEnding(1);

			// Name
			FString ElementName = Input.Mid(ElementNameBegin, ElementNameEnd - ElementNameBegin);

			// Capture Group 2 is the attribute list.
			int32 AttributeListBegin = ElementRegexMatcher.GetCaptureGroupBeginning(2);
			int32 AttributeListEnd = ElementRegexMatcher.GetCaptureGroupEnding(2);

			// Capture Group 3 is the content.
			int32 ElementContentBegin = ElementRegexMatcher.GetCaptureGroupBeginning(3);
			int32 ElementContentEnd = ElementRegexMatcher.GetCaptureGroupEnding(3);

			FTextRange ContentRange(ElementContentBegin, ElementContentEnd);

			TMap<FString, FTextRange> Attributes;

			if (AttributeListBegin != INDEX_NONE && AttributeListEnd != INDEX_NONE)
			{
				FRegexMatcher AttributeRegexMatcher(AttributeRegexPattern, Input);
				AttributeRegexMatcher.SetLimits(AttributeListBegin, AttributeListEnd);

				// Iterate through the attribute list, each time trying to find a match for the attribute regex.
				while (AttributeRegexMatcher.FindNext())
				{
					// Capture Group 1 is the attribute key.
					int32 AttributeKeyBegin = AttributeRegexMatcher.GetCaptureGroupBeginning(1);
					int32 AttributeKeyEnd = AttributeRegexMatcher.GetCaptureGroupEnding(1);

					// Capture Group 2 is the attribute value.
					int32 AttributeValueBegin = AttributeRegexMatcher.GetCaptureGroupBeginning(2);
					int32 AttributeValueEnd = AttributeRegexMatcher.GetCaptureGroupEnding(2);

					// Attribute
					Attributes.Add(Input.Mid(AttributeKeyBegin, AttributeKeyEnd - AttributeKeyBegin), FTextRange(AttributeValueBegin, AttributeValueEnd));
				}
			}

			// Add intervening run to line.
			FTextRange InterveningRunRange(LastRunEnd, ElementBegin);
			if (!InterveningRunRange.IsEmpty())
			{
				FTextRunParseResults InterveningRunParseResults(FString(), InterveningRunRange);
				LastRunEnd = ElementBegin;
				LineParseResults.Runs.Add(InterveningRunParseResults);
			}

			// Add element run to line.
			FTextRunParseResults RunParseResults(ElementName, OriginalRange, ContentRange);
			RunParseResults.MetaData = Attributes;
			LineParseResults.Runs.Add(RunParseResults);
			LastRunEnd = ElementEnd;
		}

		// Add dangling run to line.
		FTextRange InterveningRunRange(LastRunEnd, LineParseResults.Range.EndIndex);
		if (!InterveningRunRange.IsEmpty())
		{
			FTextRunParseResults InterveningRunParseResults(FString(), InterveningRunRange);
			LastRunEnd = LineParseResults.Range.EndIndex;
			LineParseResults.Runs.Add(InterveningRunParseResults);
		}

		// Add blank, empty run if none are present.
		if (LineParseResults.Runs.Num() == 0)
		{
			FTextRunParseResults EmptyRunParseResults(FString(), LineParseResults.Range);
			LastRunEnd = EmptyRunParseResults.OriginalRange.EndIndex;
			LineParseResults.Runs.Add(EmptyRunParseResults);
		}

		LineParseResultsArray.Add(LineParseResults);
	}
}

SHIDENCORE_API void FShidenRichTextMarkupParser::HandleEscapeSequences(const FString& Input, TArray<FTextLineParseResults>& LineParseResultsArray, FString& ConcatenatedUnescapedLines) const
{
	// Modify original string to handle escape sequences that need to be replaced while updating run ranges.
	for (int32 i = 0; i < LineParseResultsArray.Num(); ++i)
	{
		FTextLineParseResults& LineParseResults = LineParseResultsArray[i];

		// Adjust begin indices for previous substitutions.
		LineParseResults.Range.BeginIndex = ConcatenatedUnescapedLines.Len();

		for (int32 j = 0; j < LineParseResults.Runs.Num(); ++j)
		{
			FTextRunParseResults& RunParseResults = LineParseResults.Runs[j];
			FRegexMatcher EscapeSequenceRegexMatcher(EscapeSequenceRegexPattern, Input);

			TArray<int32*> IndicesToUpdate;
			IndicesToUpdate.Add(&RunParseResults.OriginalRange.BeginIndex);
			for (TPair<FString, FTextRange>& Pair : RunParseResults.MetaData)
			{
				IndicesToUpdate.Add(&Pair.Value.BeginIndex);
				IndicesToUpdate.Add(&Pair.Value.EndIndex);
			}
			if (RunParseResults.ContentRange.BeginIndex != INDEX_NONE && RunParseResults.ContentRange.EndIndex != INDEX_NONE)
			{
				IndicesToUpdate.Add(&RunParseResults.ContentRange.BeginIndex);
				IndicesToUpdate.Add(&RunParseResults.ContentRange.EndIndex);
			}
			IndicesToUpdate.Add(&RunParseResults.OriginalRange.EndIndex);

			{
				const auto GetUnescapedString = [&]()
				{
					int32 LastCopiedIndex = EscapeSequenceRegexMatcher.GetBeginLimit();
					while (EscapeSequenceRegexMatcher.FindNext())
					{
						// Copy intervening characters between the end of the last copy and the beginning of this match.
						ConcatenatedUnescapedLines += Input.Mid(LastCopiedIndex, EscapeSequenceRegexMatcher.GetMatchBeginning() - LastCopiedIndex);
						LastCopiedIndex = EscapeSequenceRegexMatcher.GetMatchBeginning();

						// Identify which escape sequence was captured based on which capture group has a valid range.
						for (int32 k = 0; k < UnescapeHelper.EscapeSequences.Num(); ++k)
						{
							const int32 GroupOrdinal = 1 + k; // Groups are accessed by ordinal, not index.
							const int32 EscapeSequenceBeginIndex = EscapeSequenceRegexMatcher.GetCaptureGroupBeginning(GroupOrdinal);
							const int32 EscapeSequenceEndIndex = EscapeSequenceRegexMatcher.GetCaptureGroupEnding(GroupOrdinal);

							// Lookup and copy unescaped character in place of the escape sequence.
							if (EscapeSequenceBeginIndex != INDEX_NONE && EscapeSequenceEndIndex != INDEX_NONE)
							{
								ConcatenatedUnescapedLines += UnescapeHelper.UnescapedCharacters[k];
								break;
							}
						}

						LastCopiedIndex = EscapeSequenceRegexMatcher.GetMatchEnding();
					}

					// Copy intervening characters between the end of the last copy and the end of the run.
					ConcatenatedUnescapedLines += Input.Mid(LastCopiedIndex, EscapeSequenceRegexMatcher.GetEndLimit() - LastCopiedIndex);
				};

				int32 k;
				for (k = 0; k + 1 < IndicesToUpdate.Num(); ++k)
				{
					EscapeSequenceRegexMatcher.SetLimits(*(IndicesToUpdate[k]), *(IndicesToUpdate[k + 1]));
					*(IndicesToUpdate[k]) = ConcatenatedUnescapedLines.Len();
					GetUnescapedString();
				}
				*(IndicesToUpdate[k]) = ConcatenatedUnescapedLines.Len();
			}
		}

		// Adjust end indices for previous substitutions.
		LineParseResults.Range.EndIndex = ConcatenatedUnescapedLines.Len();
	}
}