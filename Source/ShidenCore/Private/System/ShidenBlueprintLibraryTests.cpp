#include "Misc/AutomationTest.h"
#include "System/ShidenBlueprintLibrary.h"

struct FShidenGetParsedLengthTestParameters
{
	FString InputText;
	int32 ExpectedLength;

	FString ToString() const
	{
		return FString::Printf(TEXT("%s::%d"), *InputText, ExpectedLength);
	}

	FShidenGetParsedLengthTestParameters(const FString& InputText, const int32 ExpectedLength)
		:InputText(InputText),ExpectedLength(ExpectedLength)
	{
	}

	explicit FShidenGetParsedLengthTestParameters(const FString& Parameters)
	{
		FString ExpectedLengthText;
		Parameters.Split(TEXT("::"), &InputText, &ExpectedLengthText, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
		ExpectedLength = FCString::Atoi(*ExpectedLengthText);
	}
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GetParsedLengthTest, "ShidenBlueprintLibrary.GetParsedLength", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
void GetParsedLengthTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("PlainText01");
	FShidenGetParsedLengthTestParameters Params(TEXT("Hello World!"), 12);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("NewLine01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello\r\nWorld!"), 11);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello <img id=\"value\"/> World!"), 14);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag02");
	Params = FShidenGetParsedLengthTestParameters(TEXT("<img id=\"value\"/> Hello World!"), 14);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag03");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello World! <img id=\"value\"/>"), 14);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello<wait time=\"1.0\"/> World!"), 12);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag02");
	Params = FShidenGetParsedLengthTestParameters(TEXT("<wait time=\"1.0\"/>Hello World!"), 12);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag03");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello World!<wait time=\"1.0\"/>"), 12);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("BoldTag01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello <b>Bold</> World!"), 17);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag02");
	Params = FShidenGetParsedLengthTestParameters(TEXT("<b>Bold</> Hello World!"), 17);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag03");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello World! <b>Bold</>"), 17);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag04");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hell<b>o</> World!"), 12);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("IgnoreNestedSelfClosedTags01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello <b>bold<wait time=\"1.0\"/> text</> World!"), 40);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("IgnoreNestedSelfClosedTags02");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello <b>bold <wait time=\"1.0\"/><img id=\"value\"/> text</> World!"), 58);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("IgnoreNestedSelfClosedTags03");
	Params = FShidenGetParsedLengthTestParameters(TEXT("<b>Hello bold <wait time=\"1.0\"/><img id=\"value\"/> text World!</>"), 58);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("HtmlEscape01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("&quot;&amp;&lt;&gt;"), 4);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("HtmlEscape02");
	Params = FShidenGetParsedLengthTestParameters(TEXT("&lt;wait time=\"1.0\"/>"), 18);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("HtmlEscape03");
	Params = FShidenGetParsedLengthTestParameters(TEXT("&lt;<wait time=\"1.0\"/>"), 1);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText01");
	Params = FShidenGetParsedLengthTestParameters(TEXT("<ruby t=\"るび\">ルビ</>"), 2);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText02");
	Params = FShidenGetParsedLengthTestParameters(TEXT("Hello <ruby t=\"るび\">ルビ</> World!"), 15);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText03");
	Params = FShidenGetParsedLengthTestParameters(TEXT("<ruby t=\"\">漢字</>"), 2);
	OutTestCommands.Add(Params.ToString());
}

bool GetParsedLengthTest::RunTest(const FString& Parameters)
{
	const FShidenGetParsedLengthTestParameters Params(Parameters);
	const int32 ParsedLength = UShidenBlueprintLibrary::GetParsedLength(Params.InputText);
	if (ParsedLength != Params.ExpectedLength)
	{
		AddError(FString::Printf(TEXT("Expected %d, but got %d"), Params.ExpectedLength, ParsedLength));
		return false;
	}
	return true;
}

struct FShidenGetCharactersWithParsedLengthTestParameters
{
	FString OriginalText;
	FString ExpectedText;
	int32 Length;

	FString ToString()
	{
		OriginalText.ReplaceInline(TEXT(":"), TEXT("\\:"));
		ExpectedText.ReplaceInline(TEXT(":"), TEXT("\\:"));
		return FString::Printf(TEXT("%s::%s::%d"), *OriginalText, *ExpectedText, Length);
	}

	FShidenGetCharactersWithParsedLengthTestParameters(const FString& OriginalText, const FString& ExpectedText, const int32 Length)
		:OriginalText(OriginalText), ExpectedText(ExpectedText), Length(Length)
	{
	}

	explicit FShidenGetCharactersWithParsedLengthTestParameters(const FString& Parameters)
	{
		// split into array
		TArray<FString> Params;
		Parameters.ParseIntoArray(Params, TEXT("::"), true);
		if (Params.Num() == 3)
		{
			OriginalText = Params[0];
			OriginalText.ReplaceInline(TEXT("\\:"), TEXT(":"));
			ExpectedText = Params[1];
			ExpectedText.ReplaceInline(TEXT("\\:"), TEXT(":"));
			Length = FCString::Atoi(*Params[2]);
		}
		else
		{
			OriginalText = TEXT("");
			ExpectedText = TEXT("");
			Length = 0;
		}
	}
};

IMPLEMENT_COMPLEX_AUTOMATION_TEST(GetCharactersWithParsedLengthTest, "ShidenBlueprintLibrary.GetCharactersWithParsedLength", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)
void GetCharactersWithParsedLengthTest::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	OutBeautifiedNames.Add("PlainText01");
	FShidenGetCharactersWithParsedLengthTestParameters Params(TEXT("Hello World!"), TEXT("Hello "), 6);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("PlainText02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello World!"), TEXT("Hello World!"), 12);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("NewLine01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\r\nWorld!"), TEXT("Hell"), 4);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("NewLine02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\r\nWorld!"), TEXT("Hello\r\n"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("NewLine03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\r\nWorld!"), TEXT("Hello\r\nW"), 6);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("NewLine04");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\r\nWorld!"), TEXT("Hello\r\nWo"), 7);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<img id=\"value\"/>\nWorld!"), TEXT("Hello\n"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<img id=\"value\"/>\nWorld!"), TEXT("Hello\n<img id=\"value\"/>\n"), 6);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<img id=\"value\"/>\nWorld!"), TEXT("Hello\n<img id=\"value\"/>\nW"), 7);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("ImgTag04");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<img id=\"value\"/><img id=\"value\"/>\nWorld!"), TEXT("Hello\n<img id=\"value\"/><img id=\"value\"/>\nW"), 8);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello<wait time=\"1.0\"/> World!"), TEXT("Hell"), 4);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello<wait time=\"1.0\"/> World!"), TEXT("Hello<wait time=\"1.0\"/>"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<wait time=\"1.0\"/>\nWorld!"), TEXT("Hell"), 4);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag04");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<wait time=\"1.0\"/>\nWorld!"), TEXT("Hello\n<wait time=\"1.0\"/>\n"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag05");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<wait time=\"1.0\"/><wait time=\"1.0\"/>\nWorld!"), TEXT("Hello\n<wait time=\"1.0\"/><wait time=\"1.0\"/>\n"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag06");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello\n<wait time=\"1.0\"/>\nWorld!"), TEXT("Hello\n<wait time=\"1.0\"/>\nW"), 6);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag07");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<wait time=\"1.0\"/>Hello World!"), TEXT("<wait time=\"1.0\"/>"), 0);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("WaitTag08");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<wait time=\"1.0\"/>Hello World!"), TEXT("<wait time=\"1.0\"/>H"), 1);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("BoldTag01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<b>Hello World!</>"), TEXT("<b>Hello</>"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<b>Hello World!</>"), TEXT("<b>Hello </>"), 6);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<b>Hello World!</>"), TEXT("<b>Hello World!</>"), 12);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag04");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<b>Hello World!</>test"), TEXT("<b>Hello World!</>t"), 13);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("BoldTag05");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hell<b>o</> World!"), TEXT("Hell<b>o</> W"), 7);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("MultiTags01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<img id=\"value\"/><wait time=\"1.0\"/>"), TEXT("<img id=\"value\"/><wait time=\"1.0\"/>"), 1);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("MultiTags02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<wait time=\"1.0\"/><img id=\"value\"/><wait time=\"1.0\"/>"), TEXT("<wait time=\"1.0\"/><img id=\"value\"/><wait time=\"1.0\"/>"), 1);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("IgnoreNestedSelfClosedTags01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello <b>bold<wait time=\"1.0\"/>text</> World!"), TEXT("Hello <b>bold<</>"), 11);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("IgnoreNestedSelfClosedTags02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello <b>bold<wait time=\"1.0\"/><img id=\"value\"/> text</> World!"), TEXT("Hello <b>bold<wait time=\"1.0\"/><</>"), 29);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("IgnoreNestedSelfClosedTags03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<b>Hello bold<wait time=\"1.0\"/><img id=\"value\"/>text World!</>"), TEXT("<b>Hello bold<wait time=\"1.0\"/><</>"), 29);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("HtmlEscape01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("&quot;&amp;&lt;&gt;test"), TEXT("&quot;"), 1);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("HtmlEscape02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("&quot;&amp;&lt;&gt;test"), TEXT("&quot;&amp;"), 2);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("HtmlEscape03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("&quot;&amp;&lt;&gt;test"), TEXT("&quot;&amp;&lt;"), 3);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("HtmlEscape04");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("&quot;&amp;&lt;&gt;test"), TEXT("&quot;&amp;&lt;&gt;"), 4);
	OutTestCommands.Add(Params.ToString());
	
	OutBeautifiedNames.Add("HtmlEscape05");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("&lt;wait time=\"1.0\"/>"), TEXT("&lt;wait"), 5);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("HtmlEscape06");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("&lt;<wait time=\"1.0\"/>"), TEXT("&lt;<wait time=\"1.0\"/>"), 1);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText01");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<ruby t=\"るび\">ルビ</>"), TEXT("<ruby all=\"false\" t=\"るび\">ル</>"), 1);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText02");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("<ruby t=\"るび\">ルビ</>"), TEXT("<ruby t=\"るび\">ルビ</>"), 2);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText03");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello <ruby t=\"るび\">ルビ</> World!"), TEXT("Hello <ruby all=\"false\" t=\"るび\">ル</>"), 7);
	OutTestCommands.Add(Params.ToString());

	OutBeautifiedNames.Add("RubyText04");
	Params = FShidenGetCharactersWithParsedLengthTestParameters(TEXT("Hello <ruby t=\"るび\">ルビ</> World!"), TEXT("Hello <ruby t=\"るび\">ルビ</> W"), 10);
	OutTestCommands.Add(Params.ToString());
}

bool GetCharactersWithParsedLengthTest::RunTest(const FString& Parameters)
{
	const FShidenGetCharactersWithParsedLengthTestParameters Params(Parameters);
	const FString& Actual = UShidenBlueprintLibrary::GetCharactersWithParsedLength(Params.OriginalText, Params.Length);
	if (Actual != Params.ExpectedText)
	{
		AddError(FString::Printf(TEXT("Expected %s, but got %s"), *Params.ExpectedText, *Actual));
		return false;
	}
	return true;
}
