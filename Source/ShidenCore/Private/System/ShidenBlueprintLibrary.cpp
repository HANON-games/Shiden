// Copyright (c) 2025 HANON. All Rights Reserved.

// ReSharper disable CppExpressionWithoutSideEffects
#include "System/ShidenBlueprintLibrary.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "DelayAction.h"
#include "Command/ShidenCommandDefinitions.h"
#include "Engine/AssetManager.h"
#include "HAL/PlatformApplicationMisc.h"
#include "System/ShidenBacklogItem.h"
#include "Async/Async.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundBase.h"

SHIDENCORE_API void UShidenBlueprintLibrary::CopyToClipboard(const FString& Text)
{
	FPlatformApplicationMisc::ClipboardCopy(*Text);
}

SHIDENCORE_API void UShidenBlueprintLibrary::GetClipboardContent(FString& ClipboardText)
{
	FPlatformApplicationMisc::ClipboardPaste(ClipboardText);
}

SHIDENCORE_API int32 UShidenBlueprintLibrary::GetParsedLength(const FString& Text)
{
	FString ResultText;
	ResultText.Reserve(Text.Len());

	// Replace HTML entities with 'a' to treat them as single characters
	for (int32 i = 0; i < Text.Len(); i++)
	{
		if (Text[i] == '\n' || Text[i] == '\r')
		{
			continue;
		}
		if (Text[i] == '&')
		{
			const FString Temp = Text.Mid(i + 1);
			if (Temp.StartsWith(TEXT("lt;")) || Temp.StartsWith(TEXT("gt;")))
			{
				ResultText.AppendChar(TEXT('a'));
				i += 3;
				continue;
			}
			
			if (Temp.StartsWith(TEXT("amp;")))
			{
				ResultText.AppendChar(TEXT('a'));
				i += 4;
				continue;
			}
			
			if (Temp.StartsWith(TEXT("quot;")))
			{
				ResultText.AppendChar(TEXT('a'));
				i += 5;
				continue;
			}
		}
		ResultText.AppendChar(Text[i]);
	}

	// pattern like <tagName> ... </>
	FRegexMatcher TagMatcher(GetNonSelfClosingTagPattern(), ResultText);

	while (TagMatcher.FindNext())
	{
		FString FullMatch = TagMatcher.GetCaptureGroup(0);
		FString TagContent = TagMatcher.GetCaptureGroup(2);
		// Replace all '<' characters to 'a' to avoid matching with Self ClosingTagPattern
		TagContent.ReplaceCharInline('<', 'a', ESearchCase::CaseSensitive);
		ResultText.ReplaceInline(*FullMatch, *TagContent, ESearchCase::CaseSensitive);
	}

	// pattern like <img id="value"/>
	FRegexMatcher Matcher(GetSelfClosingTagPattern(), ResultText);

	while (Matcher.FindNext())
	{
		FString FullMatch = Matcher.GetCaptureGroup(0);
		FString TagName = Matcher.GetCaptureGroup(1);
		if (TagName != TEXT("wait"))
		{
			// Treat as 1 character
			ResultText.ReplaceInline(*FullMatch, TEXT("a"), ESearchCase::CaseSensitive);
		}
		else
		{
			ResultText.ReplaceInline(*FullMatch, TEXT(""), ESearchCase::CaseSensitive);
		}
	}

	return ResultText.Len();
}

struct FTextPosition
{
	FString TagName;
	int32 OpenTagStart;
	int32 CloseTagEnd;
	int32 ContentStart;
	int32 ContentEnd;
};

SHIDENCORE_API FString UShidenBlueprintLibrary::GetCharactersWithParsedLength(const FString& Text, const int32 Length)
{
	static TArray<FTextPosition> Pos;
	static TArray<FTextPosition> WaitTagPos;
	static FString CachedText;

	if (CachedText != Text)
	{
		CachedText = Text;
		Pos.Empty();
		WaitTagPos.Empty();

		// pattern like <img id="value"/>
		FRegexMatcher Matcher(GetSelfClosingTagPattern(), Text);

		while (Matcher.FindNext())
		{
			const FString TagName = Matcher.GetCaptureGroup(1);
			const int32 TagStart = Matcher.GetMatchBeginning();
			const int32 TagEnd = Matcher.GetMatchEnding();
			if (TagName == TEXT("wait"))
			{
				WaitTagPos.Add({TagName, TagStart, TagEnd - 1, -1, -1});
			}
			else
			{
				Pos.Add({TagName, TagStart, TagEnd - 1, -1, -1});
			}
		}

		// pattern like <tagName> ... </>
		FRegexMatcher Matcher2(GetNonSelfClosingTagPattern(), Text);

		while (Matcher2.FindNext())
		{
			const FString TagName = Matcher2.GetCaptureGroup(1);
			const int32 TagStart = Matcher2.GetMatchBeginning();
			const int32 TagEnd = Matcher2.GetMatchEnding();
			Pos.Add({TagName, TagStart, TagEnd - 1, Matcher2.GetCaptureGroupBeginning(2), Matcher2.GetCaptureGroupEnding(2) - 1});
		}
	}

	int32 ResultLen = 0;

	bool bFound;
	do
	{
		bFound = false;
		if (Text.IsValidIndex(ResultLen) && (Text[ResultLen] == '\n' || Text[ResultLen] == '\r'))
		{
			ResultLen++;
			bFound = true;
		}
		for (const FTextPosition& WaitTag : WaitTagPos)
		{
			if (ResultLen == WaitTag.OpenTagStart)
			{
				ResultLen = WaitTag.CloseTagEnd + 1;
				bFound = true;
			}
		}
	}
	while (bFound);

	if (Length == 0)
	{
		return Text.Left(ResultLen);
	}

	FString ResultText = Text;
	bool bIsInTag = false;
	FString LastTagName = TEXT("");
	for (int32 Index = 0; Index < Length; Index++)
	{
		if (ResultText.IsValidIndex(ResultLen + 1) && ResultText[ResultLen] == '&')
		{
			const FString Temp = ResultText.Mid(ResultLen + 1);
			if (Temp.StartsWith(TEXT("lt;")) || Temp.StartsWith(TEXT("gt;")))
			{
				ResultLen += 3;
			}
			else if (Temp.StartsWith(TEXT("amp;")))
			{
				ResultLen += 4;
			}
			else if (Temp.StartsWith(TEXT("quot;")))
			{
				ResultLen += 5;
			}
		}
		
		for (const FTextPosition& Tag : Pos)
		{
			if (!bIsInTag && Tag.OpenTagStart == ResultLen)
			{
				LastTagName = Tag.TagName;
				if (Tag.ContentStart == -1 || Tag.ContentStart == Tag.ContentEnd)
				{
					ResultLen = Tag.CloseTagEnd;
				}
				else
				{
					bIsInTag = true;
					ResultLen = Tag.ContentStart;
				}
				break;
			}
			if (bIsInTag && Tag.ContentEnd == ResultLen)
			{
				bIsInTag = false;
				ResultLen = Tag.CloseTagEnd;
				break;
			}
		}
		
		if (!bIsInTag)
		{
			for (const FTextPosition& WaitTag : WaitTagPos)
			{
				if (ResultLen == WaitTag.OpenTagStart)
				{
					ResultLen = WaitTag.CloseTagEnd;
					break;
				}
			}
		}
		
		ResultLen++;
		do
		{
			bFound = false;
			if (ResultText.IsValidIndex(ResultLen) && (ResultText[ResultLen] == '\n' || ResultText[ResultLen] == '\r'))
			{
				ResultLen++;
				bFound = true;
			}
			if (!bIsInTag)
			{
				for (const auto& [TagName, OpenTagStart, CloseTagEnd, ContentStart, ContentEnd] : WaitTagPos)
				{
					if (ResultLen == OpenTagStart)
					{
						ResultLen = CloseTagEnd + 1;
						bFound = true;
						break;
					}
				}
			}
		}
		while (bFound);
	}

	ResultText = ResultText.Left(ResultLen);

	if (bIsInTag)
	{
		if (LastTagName == TEXT("Ruby"))
		{
			// Add all="false" to the attribute of the last ruby tag
			const int32 Index = ResultText.Find(TEXT("<ruby"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);
			if (Index != INDEX_NONE)
			{
				ResultText.InsertAt(Index + 5, TEXT(" all=\"false\""));
			}
		}
		ResultText += TEXT("</>");
	}

	return ResultText;
}

SHIDENCORE_API bool UShidenBlueprintLibrary::TryParseWaitTimeFromLastTag(const FString& RawText, const int32 Length, float& WaitTime)
{
	const FString ParsedText = GetCharactersWithParsedLength(RawText, Length);

	if (ParsedText.IsEmpty() || !ParsedText.EndsWith(TEXT("/>")))
	{
		return false;
	}

	if (FRegexMatcher MatcherWait(GetWaitTimePattern(), ParsedText); MatcherWait.FindNext())
	{
		const FString TimeStr = MatcherWait.GetCaptureGroup(1);
		WaitTime = FCString::Atof(*TimeStr);
		return true;
	}

	return false;
}

SHIDENCORE_API void UShidenBlueprintLibrary::MultiThreadDelay(UObject* WorldContextObject, const float Duration, const FLatentActionInfo LatentInfo)
{
	if (const TObjectPtr<UWorld> World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelayAction(Duration, LatentInfo));
	}
}

SHIDENCORE_API UClass* UShidenBlueprintLibrary::ConstructClassFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath)
{
	if (SoftObjectPath.IsNull() || !SoftObjectPath.IsValid())
	{
		return nullptr;
	}

	const FString AssetPathString = SoftObjectPath.GetAssetPathString();
	if (AssetPathString.IsEmpty())
	{
		return nullptr;
	}
	
	if (AssetPathString.StartsWith(TEXT("/Script/")))
	{
		return StaticLoadClass(UObject::StaticClass(), nullptr, *AssetPathString, nullptr, LOAD_None, nullptr);
	}

	const FString Path = AssetPathString + TEXT("_C");
	return StaticLoadClass(UObject::StaticClass(), nullptr, *Path, nullptr, LOAD_None, nullptr);
}

SHIDENCORE_API bool UShidenBlueprintLibrary::TryGetOrLoadAsset(const FString& ObjectPath, UObject*& Asset)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (const TObjectPtr<UObject> AssetPtr = ShidenSubsystem->AssetCache.FindRef(ObjectPath))
	{
		Asset = AssetPtr;
		return true;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Asset = Streamable.LoadSynchronous(ObjectPath, false);
	if (!Asset)
	{
		return false;
	}

	ShidenSubsystem->AssetCache.Add(ObjectPath, Asset);
	return true;
}

SHIDENCORE_API void UShidenBlueprintLibrary::UnloadAssets(const bool bForceGC)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();

	if (bForceGC)
	{
		GEngine->ForceGarbageCollection(true);
	}
}

SHIDENCORE_API FString UShidenBlueprintLibrary::MakeErrorMessage(const FGuid& ScenarioId, const int32 ScenarioIndex,
                                                                 const FString& CommandName, const FString& ErrorMessage)
{
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	if (!ProjectConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShidenProjectConfig is null"));
		return TEXT("");
	}

	const FString ScenarioPath = ProjectConfig->ScenarioPaths.FindRef(ScenarioId);

	FString LeftS, RightS;
	ScenarioPath.Split(TEXT("/"), nullptr, &RightS, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	RightS.Split(TEXT("."), &LeftS, nullptr, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	const FString LineNumber = ScenarioIndex == -1 ? TEXT("") : TEXT(" L") + FString::FromInt(ScenarioIndex + 1);
	const FString Command = CommandName.IsEmpty() ? TEXT("") : CommandName + TEXT(": ");

	return FString::Printf(TEXT("[%s%s] %s%s"), *LeftS, *LineNumber, *Command, *ErrorMessage);
}

SHIDENCORE_API void UShidenBlueprintLibrary::AddBacklogItem(const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Add(FShidenBacklogItem{Command, AdditionalProperties});
}

SHIDENCORE_API void UShidenBlueprintLibrary::UpdateBacklogItem(const int32 ScenarioIndex, const FShidenCommand& Command,
                                                               const TMap<FString, FString>& AdditionalProperties)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (!ShidenSubsystem->BacklogItems.IsValidIndex(ScenarioIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid backlog index: %d"), ScenarioIndex);
		return;
	}

	FShidenBacklogItem& BacklogItem = ShidenSubsystem->BacklogItems[ScenarioIndex];
	BacklogItem.Command = Command;
	BacklogItem.AdditionalProperties = AdditionalProperties;
}

SHIDENCORE_API TArray<FShidenBacklogItem>& UShidenBlueprintLibrary::GetBacklogItems()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	return ShidenSubsystem->BacklogItems;
}

SHIDENCORE_API void UShidenBlueprintLibrary::ClearBacklogItems()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Empty();
}

SHIDENCORE_API TMap<FString, FShidenTextType> UShidenBlueprintLibrary::GetShidenTextTypes()
{
	TMap<FString, FShidenTextType> TextTypes
	{
		{TEXT("Default"), FShidenTextType{true}},
		{TEXT("Name"), FShidenTextType{false}}
	};

	const TMap<FString, FShidenTextType> AdditionalTextTypes = GetDefault<UShidenProjectConfig>()->AdditionalTextTypes;
	TextTypes.Append(AdditionalTextTypes);
	return TextTypes;
}

SHIDENCORE_API FString UShidenBlueprintLibrary::GetObjectPathFromClass(const UClass* Class)
{
	if (!Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("Class is null"));
		return FString();
	}
	const FTopLevelAssetPath Path = FTopLevelAssetPath(Class);
	return Path.ToString().LeftChop(2);
}

SHIDENCORE_API bool UShidenBlueprintLibrary::TryGetSoundTypeFromSoundBase(const USoundBase* SoundBase, EShidenSoundType& SoundType)
{
	if (!SoundBase)
	{
		UE_LOG(LogTemp, Warning, TEXT("SoundBase is null"));
		return false;
	}
	
	const TObjectPtr<USoundClass> SoundClass = SoundBase->GetSoundClass();
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	const TObjectPtr<USoundClass> BGMSoundClass = ProjectConfig->GetBGMSoundClass();
	const TObjectPtr<USoundClass> SESoundClass = ProjectConfig->GetSESoundClass();
	const TObjectPtr<USoundClass> VoiceSoundClass = ProjectConfig->GetVoiceSoundClass();

	if (SoundClass == BGMSoundClass || BGMSoundClass->ChildClasses.Contains(SoundClass))
	{
		SoundType = EShidenSoundType::BGM;
		return true;
	}

	if (SoundClass == SESoundClass || SESoundClass->ChildClasses.Contains(SoundClass))
	{
		SoundType = EShidenSoundType::SE;
		return true;
	}
	
	if (SoundClass == VoiceSoundClass || VoiceSoundClass->ChildClasses.Contains(SoundClass))
	{
		SoundType = EShidenSoundType::Voice;
		return true;
	}

	return false;
}

SHIDENCORE_API void UShidenBlueprintLibrary::InitCommandDefinitions()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() > 0)
	{
		return;
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	for (const FSoftObjectPath& CommandDefinitionSoftObjectPath : ProjectConfig->CommandDefinitions)
	{
		TObjectPtr<UShidenCommandDefinitions> CommandDefinitions = Cast<UShidenCommandDefinitions>(CommandDefinitionSoftObjectPath.TryLoad());
		if (!CommandDefinitions)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load CommandDefinitions from ObjectPath: %s"),
			       *CommandDefinitionSoftObjectPath.GetAssetPathString());
			continue;
		}

		for (const auto& [CommandName, CommandDefinition] : CommandDefinitions->CommandDefinitions)
		{
			if (ShidenSubsystem->CommandDefinitionCache.Contains(CommandName))
			{
				UE_LOG(LogTemp, Warning, TEXT("CommandDefinition already exists for CommandName: %s"), *CommandName);
				continue;
			}

			ShidenSubsystem->CommandDefinitionCache.Add(CommandName, CommandDefinition);
		}
	}
}

SHIDENCORE_API const TMap<FString, FShidenCommandDefinition>& UShidenBlueprintLibrary::GetCommandDefinitionsCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		InitCommandDefinitions();
	}

	return ShidenSubsystem->CommandDefinitionCache;
}

SHIDENCORE_API void UShidenBlueprintLibrary::ClearAllCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();
	ShidenSubsystem->CommandCache.Empty();
	ShidenSubsystem->CommandDefinitionCache.Empty();
	ShidenSubsystem->ScenarioCache.Empty();
}

SHIDENCORE_API FString UShidenBlueprintLibrary::GetCommandArgument(const FShidenCommand& Command, const FString& ArgName)
{
	return Command.Args.FindRef(ArgName);
}

SHIDENCORE_API bool UShidenBlueprintLibrary::IsValidSoftObjectPath(const FString& ObjectPath)
{
	const FSoftObjectPath SoftObjectPath(ObjectPath);
	return ObjectPath.IsEmpty() || SoftObjectPath.IsValid();
}

SHIDENCORE_API bool UShidenBlueprintLibrary::IsAutoTextMode()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->bAutoTextMode;
}

SHIDENCORE_API void UShidenBlueprintLibrary::SetAutoTextMode(const bool bEnabled)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	ShidenSubsystem->bAutoTextMode = bEnabled;
}

FRegexPattern& UShidenBlueprintLibrary::GetSelfClosingTagPattern()
{
	static FRegexPattern SelfClosingTagPattern(TEXT("<((?:[\\w\\d\\.-]+))(?:(?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?/>"));
	return SelfClosingTagPattern;
}

FRegexPattern& UShidenBlueprintLibrary::GetNonSelfClosingTagPattern()
{
	static FRegexPattern NonSelfClosingTagPattern(TEXT("<((?:[\\w\\d\\.-]+))(?:(?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?>(.*?)</>"));
	return NonSelfClosingTagPattern;
}

FRegexPattern& UShidenBlueprintLibrary::GetWaitTimePattern()
{
	static FRegexPattern WaitTimePattern(TEXT("<wait\\stime=\"([\\d.]+)\"/>$"));
	return WaitTimePattern;
}
