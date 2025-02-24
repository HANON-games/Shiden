// Copyright (c) 2024 HANON. All Rights Reserved.

// ReSharper disable CppExpressionWithoutSideEffects
#include "Utility/ShidenCoreFunctionLibrary.h"
#include "DelayAction.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Command/ShidenCommandDefinitions.h"
#include "Engine/AssetManager.h"
#include "Misc/OutputDeviceNull.h"
#include "HAL/PlatformApplicationMisc.h"
#include "System/ShidenBacklogItem.h"
#include "Async/Async.h"
#include "Scenario/ShidenScenarioFunctionLibrary.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundBase.h"

SHIDENCORE_API void UShidenCoreFunctionLibrary::CopyToClipboard(const FString& Str)
{
	FPlatformApplicationMisc::ClipboardCopy(*Str);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetFromClipboard(FString& Dest)
{
	FPlatformApplicationMisc::ClipboardPaste(Dest);
}

SHIDENCORE_API int32 UShidenCoreFunctionLibrary::GetParsedLength(const FString& Text)
{
	FString ResultText = Text;

	ResultText.ReplaceInline(TEXT("&quot;"), TEXT("\""), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&amp;"), TEXT("&"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&apos;"), TEXT("'"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&lt;"), TEXT("<"), ESearchCase::CaseSensitive);

	// pattern like <img id="value"/>
	const FRegexPattern Pattern = FRegexPattern(FString(TEXT("(<[\\w\\d\\.-]+((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)))")));
	FRegexMatcher Matcher(Pattern, Text);

	while (Matcher.FindNext())
	{
		FString Str = Matcher.GetCaptureGroup(1);
		ResultText.ReplaceInline(*Str, TEXT("o"), ESearchCase::CaseSensitive);
	}

	// pattern like <tagName> ... </>
	const FRegexPattern Pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher Matcher2(Pattern2, Text);

	while (Matcher2.FindNext())
	{
		FString Str1 = Matcher2.GetCaptureGroup(0);
		FString Str2 = Matcher2.GetCaptureGroup(1);
		ResultText.ReplaceInline(*Str1, *Str2, ESearchCase::CaseSensitive);
	}

	return ResultText.Len();
}

struct FTextPosition
{
	int32 OpenTagStart;
	int32 CloseTagEnd;
	int32 ContentStart;
	int32 ContentEnd;
};

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCharactersWithParsedLength(const FString& Text, const int32 Len)
{
	FString ResultText = Text;

	ResultText.ReplaceInline(TEXT("&quot;"), TEXT("\""), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&amp;"), TEXT("&"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&apos;"), TEXT("'"), ESearchCase::CaseSensitive);
	ResultText.ReplaceInline(TEXT("&lt;"), TEXT("<"), ESearchCase::CaseSensitive);

	TArray<FTextPosition> Pos;

	// pattern like <img id="value"/>
	const FRegexPattern Pattern = FRegexPattern(FString(TEXT("(<[\\w\\d\\.-]+((?: (?:[\\w\\d\\.-]+=(?>\".*?\")))+)?(?:(?:/>)))")));
	FRegexMatcher Matcher(Pattern, Text);

	while (Matcher.FindNext())
	{
		const int32 Str1 = Matcher.GetMatchBeginning();
		const int32 Str2 = Matcher.GetMatchEnding();
		Pos.Add({Str1, Str2, -1, -1});
	}

	// pattern like <tagName> ... </>
	const FRegexPattern Pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher Matcher2(Pattern2, Text);

	while (Matcher2.FindNext())
	{
		const int32 Str1 = Matcher2.GetMatchBeginning();
		const int32 Str2 = Matcher2.GetMatchEnding();

		Pos.Add({Str1, Str2, Matcher2.GetCaptureGroupBeginning(1), Matcher2.GetCaptureGroupEnding(1)});
	}

	if (Pos.Num() == 0)
	{
		return Text.Left(Len);
	}

	bool bIsInTag = false;
	int32 ResultLen = 0;

	for (int32 Index = 0; Index < Len; Index++)
	{
		for (const auto& [OpenTagStart, CloseTagEnd, ContentStart, ContentEnd] : Pos)
		{
			if (OpenTagStart == ResultLen)
			{
				if (ContentStart == -1)
				{
					// TODO: Image tag is treated as 1 character, so adjust by -1.
					// Make it possible to process each tag separately.
					ResultLen = CloseTagEnd - 1;
					break;
				}
				bIsInTag = true;
				ResultLen = ContentStart;
				break;
			}
			if (ContentEnd == ResultLen)
			{
				bIsInTag = false;
				ResultLen = CloseTagEnd;
				break;
			}
		}
		ResultLen++;
	}

	ResultText = ResultText.Left(ResultLen);

	if (bIsInTag)
	{
		ResultText += TEXT("</>");
	}

	return ResultText;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::CallFunctionByName(UObject* TargetObject, const FString& FunctionName, const FString& Parameters)
{
	if (TargetObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("targetObject is nullptr"));
		return;
	}

	const FString TrimmedParam = Parameters.TrimStartAndEnd();
	const FString Cmd = TrimmedParam.IsEmpty()
		                    ? FunctionName
		                    : FString::Printf(TEXT("%s %s"), *FunctionName, *Parameters);
	FOutputDeviceNull Null;

	TargetObject->CallFunctionByNameWithArguments(*Cmd, Null, nullptr, true);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::MultiThreadDelay(UObject* WorldContextObject, const float Duration, const FLatentActionInfo LatentInfo)
{
	if (const TObjectPtr<UWorld> World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelayAction(Duration, LatentInfo));
	}
}

SHIDENCORE_API UClass* UShidenCoreFunctionLibrary::ConstructClassFromSoftObjectPath(const FSoftObjectPath& SoftObjectPath)
{
	if (SoftObjectPath.IsNull())
	{
		return nullptr;
	}
	const FString Path = SoftObjectPath.GetAssetPathString() + "_C";
	return StaticLoadClass(UObject::StaticClass(), nullptr, *Path, nullptr, LOAD_None, nullptr);
}

void FlipBitmapVertical(TArray<FColor>& Bitmap, const int32 Width, const int32 Height)
{
	for (int32 Y = 0; Y < Height / 2; ++Y)
	{
		const int32 IndexTop = Y * Width;
		const int32 IndexBottom = (Height - 1 - Y) * Width;

		for (int32 X = 0; X < Width; ++X)
		{
			const FColor Temp = Bitmap[IndexTop + X];
			Bitmap[IndexTop + X] = Bitmap[IndexBottom + X];
			Bitmap[IndexBottom + X] = Temp;
		}
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetScenarioDataAsset(const FString& ObjectPath, UShidenScenario*& Scenario, bool& bSuccess)
{
	Scenario = nullptr;
	bSuccess = false;

	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	const FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(ObjectPath));

	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AssetData is invalid for ObjectPath: %s"), *ObjectPath);
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	const TObjectPtr<UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load asset from ObjectPath: %s"), *ObjectPath);
		return;
	}

	const TObjectPtr<UShidenScenario> ShidenScenario = Cast<UShidenScenario>(LoadedAsset);
	if (!ShidenScenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded asset to UShidenScenario"));
		return;
	}

	Scenario = ShidenScenario;
	bSuccess = true;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetOrLoadAsset(const FString& ObjectPath, UObject*& Asset, bool& bSuccess)
{
	Asset = nullptr;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (const TObjectPtr<UObject>* AssetPtr = ShidenSubsystem->AssetCache.Find(ObjectPath))
	{
		Asset = *AssetPtr;
		bSuccess = true;
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	Asset = Streamable.LoadSynchronous(ObjectPath, false);

	if (!Asset)
	{
		bSuccess = false;
		return;
	}

	bSuccess = true;
	ShidenSubsystem->AssetCache.Add(ObjectPath, Asset);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UnloadAssets(const bool bForceGC)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();

	if (bForceGC)
	{
		GEngine->ForceGarbageCollection(true);
	}
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::MakeErrorMessage(const FGuid& ScenarioId, const int32 Index,
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

	const FString LineNumber = Index == -1 ? TEXT("") : TEXT(" L") + FString::FromInt(Index + 1);

	const FString Command = CommandName.IsEmpty() ? TEXT("") : CommandName + TEXT(": ");

	return FString::Printf(TEXT("[%s%s] %s%s"), *LeftS, *LineNumber, *Command, *ErrorMessage);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::AddBacklogItem(const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Add(FShidenBacklogItem{Command, AdditionalProperties});
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UpdateBacklogItem(const int32 Index, const FShidenCommand& Command, const TMap<FString, FString>& AdditionalProperties)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->BacklogItems.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid backlog index: %d"), Index);
		return;
	}

	FShidenBacklogItem& BacklogItem = ShidenSubsystem->BacklogItems[Index];
	BacklogItem.Command = Command;
	BacklogItem.AdditionalProperties = AdditionalProperties;
}

SHIDENCORE_API TArray<FShidenBacklogItem>& UShidenCoreFunctionLibrary::GetBacklogItems()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->BacklogItems;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearBacklogItems()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Empty();
}

SHIDENCORE_API TMap<FString, FShidenTextType> UShidenCoreFunctionLibrary::GetShidenTextTypes()
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

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetObjectPathFromClass(const UClass* InClass)
{
	const FTopLevelAssetPath Path = FTopLevelAssetPath(InClass);
	return Path.ToString().LeftChop(2);
}

void UShidenCoreFunctionLibrary::GetSoundTypeFromSoundBase(const USoundBase* SoundBase, EShidenSoundType& SoundType, bool& bSuccess)
{
	const TObjectPtr<USoundClass> SoundClass = SoundBase->GetSoundClass();
	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	const TObjectPtr<USoundClass> BgmSoundClass = ProjectConfig->GetBgmSoundClass();
	const TObjectPtr<USoundClass> SeSoundClass = ProjectConfig->GetSeSoundClass();
	const TObjectPtr<USoundClass> VoiceSoundClass = ProjectConfig->GetVoiceSoundClass();

	if (SoundClass == BgmSoundClass || BgmSoundClass->ChildClasses.Contains(SoundClass))
	{
		SoundType = EShidenSoundType::BGM;
		bSuccess = true;
	}
	else if (SoundClass == SeSoundClass || SeSoundClass->ChildClasses.Contains(SoundClass))
	{
		SoundType = EShidenSoundType::SE;
		bSuccess = true;
	}
	else if (SoundClass == VoiceSoundClass || VoiceSoundClass->ChildClasses.Contains(SoundClass))
	{
		SoundType = EShidenSoundType::Voice;
		bSuccess = true;
	}
	else
	{
		bSuccess = false;
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::InitCommandDefinitions()
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
			UE_LOG(LogTemp, Warning, TEXT("Failed to load CommandDefinitions from ObjectPath: %s"), *CommandDefinitionSoftObjectPath.GetAssetPathString());
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

SHIDENCORE_API const TMap<FString, FShidenCommandDefinition>& UShidenCoreFunctionLibrary::GetCommandDefinitionsCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		InitCommandDefinitions();
	}

	return ShidenSubsystem->CommandDefinitionCache;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearLoadedSystemData(const UObject* WorldContextObject)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	UShidenVariableFunctionLibrary::ResetSystemVariables();
	UShidenVariableFunctionLibrary::InitPredefinedSystemVariables(WorldContextObject);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearLoadedUserData()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	UShidenVariableFunctionLibrary::ResetUserVariables();
	UShidenVariableFunctionLibrary::ResetAllLocalVariables();
	ShidenSubsystem->ScenarioProperties.Empty();
	ShidenSubsystem->ScenarioProgressStack.Empty();
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearAllCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();
	ShidenSubsystem->CommandCache.Empty();
	ShidenSubsystem->CommandDefinitionCache.Empty();
	ShidenSubsystem->ScenarioCache.Empty();
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCommandArgument(const FShidenCommand& Command, const FString& ArgName)
{
	return Command.Args.Contains(ArgName) ? Command.Args[ArgName] : TEXT("");
}
