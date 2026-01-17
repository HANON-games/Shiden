// Copyright (c) 2025 HANON. All Rights Reserved.

// ReSharper disable CppExpressionWithoutSideEffects
#include "System/ShidenBlueprintLibrary.h"
#include "System/ShidenStructuredLog.h"
#include "Async/Async.h"
#include "Command/ShidenCommandDefinitions.h"
#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"
#include "Config/ShidenProjectConfig.h"
#include "Dom/JsonObject.h"
#include "Engine/AssetManager.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "Serialization/JsonReader.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundClass.h"
#include "Audio/ShidenSoundInfo.h"
#include "Audio/ShidenSoundType.h"
#include "System/ShidenBacklogItem.h"
#include "System/ShidenSubsystem.h"
#include "UI/ShidenWidget.h"
#include "AudioDevice.h"
#include "DelayAction.h"
#include "DSP/VolumeFader.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TimerManager.h"
#include "Audio/ShidenSoundFadeType.h"

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
		SHIDEN_WARNING("ShidenProjectConfig is null");
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
	GEngine->GetEngineSubsystem<UShidenSubsystem>()->BacklogItems.Add(FShidenBacklogItem{Command, AdditionalProperties});
}

SHIDENCORE_API void UShidenBlueprintLibrary::UpdateBacklogItem(const int32 ScenarioIndex, const FShidenCommand& Command,
                                                               const TMap<FString, FString>& AdditionalProperties)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	if (!ShidenSubsystem->BacklogItems.IsValidIndex(ScenarioIndex))
	{
		SHIDEN_WARNING("Invalid backlog index: {index}", ScenarioIndex);
		return;
	}

	FShidenBacklogItem& BacklogItem = ShidenSubsystem->BacklogItems[ScenarioIndex];
	BacklogItem.Command = Command;
	BacklogItem.AdditionalProperties = AdditionalProperties;
}

SHIDENCORE_API TArray<FShidenBacklogItem>& UShidenBlueprintLibrary::GetBacklogItems()
{
	return GEngine->GetEngineSubsystem<UShidenSubsystem>()->BacklogItems;
}

SHIDENCORE_API void UShidenBlueprintLibrary::ClearBacklogItems()
{
	GEngine->GetEngineSubsystem<UShidenSubsystem>()->BacklogItems.Empty();
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
		SHIDEN_WARNING("Class is null");
		return FString();
	}
	const FTopLevelAssetPath Path = FTopLevelAssetPath(Class);
	return Path.ToString().LeftChop(2);
}

SHIDENCORE_API bool UShidenBlueprintLibrary::TryGetSoundTypeFromSoundBase(const USoundBase* SoundBase, EShidenSoundType& SoundType)
{
	if (!SoundBase)
	{
		SHIDEN_WARNING("SoundBase is null");
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
			SHIDEN_WARNING("Failed to load CommandDefinitions from ObjectPath: {path}",
			               *CommandDefinitionSoftObjectPath.GetAssetPathString());
			continue;
		}

		for (const auto& [CommandName, CommandDefinition] : CommandDefinitions->CommandDefinitions)
		{
			if (ShidenSubsystem->CommandDefinitionCache.Contains(CommandName))
			{
				SHIDEN_WARNING("CommandDefinition already exists for CommandName: {name}", *CommandName);
				continue;
			}

			ShidenSubsystem->CommandDefinitionCache.Add(CommandName, CommandDefinition);
		}
	}
}

SHIDENCORE_API const TMap<FString, FShidenCommandDefinition>& UShidenBlueprintLibrary::GetCommandDefinitionsCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		InitCommandDefinitions();
	}

	return ShidenSubsystem->CommandDefinitionCache;
}

SHIDENCORE_API void UShidenBlueprintLibrary::ClearAllCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	ShidenSubsystem->AssetCache.Empty();
	ShidenSubsystem->CommandCache.Empty();
	ShidenSubsystem->CommandDefinitionCache.Empty();
	ShidenSubsystem->ScenarioCache.Empty();
}

SHIDENCORE_API FString UShidenBlueprintLibrary::GetCommandArgument(const FShidenCommand& Command, const FString& ArgName)
{
	return Command.GetArg(ArgName);
}

SHIDENCORE_API bool UShidenBlueprintLibrary::IsValidSoftObjectPath(const FString& ObjectPath)
{
	const FSoftObjectPath SoftObjectPath(ObjectPath);
	return ObjectPath.IsEmpty() || SoftObjectPath.IsValid();
}

SHIDENCORE_API bool UShidenBlueprintLibrary::IsAutoTextMode()
{
	return GEngine->GetEngineSubsystem<UShidenSubsystem>()->bAutoTextMode;
}

SHIDENCORE_API void UShidenBlueprintLibrary::SetAutoTextMode(const bool bEnabled)
{
	GEngine->GetEngineSubsystem<UShidenSubsystem>()->bAutoTextMode = bEnabled;
}

SHIDENCORE_API bool UShidenBlueprintLibrary::TryStartScreenFade(const TObjectPtr<const UObject> WorldContextObject, const FString& LayerName, const float FadeDuration,
                                                                const EEasingFunc::Type FadeFunction, const FLinearColor TargetColor, const bool bIsFadeOut,
                                                                const int32 Steps, const float BlendExp, const int32 ZOrder,
                                                                const bool bSaveScenarioProperty)
{
	if (!WorldContextObject)
	{
		SHIDEN_ERROR("WorldContextObject is null");
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	// Get GameViewport from LocalPlayer (supports custom GameViewport)
	TObjectPtr<UGameViewportClient> GameViewport = nullptr;
	if (const TObjectPtr<const UWorld> World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		if (const APlayerController* PlayerController = World->GetFirstPlayerController())
		{
			if (const TObjectPtr<ULocalPlayer> LocalPlayer = PlayerController->GetLocalPlayer())
			{
				GameViewport = LocalPlayer->ViewportClient;
			}
		}
	}

	if (!GameViewport)
	{
		SHIDEN_ERROR("GameViewport is null");
		return false;
	}

	// Create or get the fade layer
	FShidenScreenFadeLayer& Layer = ShidenSubsystem->ScreenFadeLayers.FindOrAdd(LayerName);

	TObjectPtr<UBorder> FadeWidget = Layer.Widget;
	if (!FadeWidget)
	{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION <= 6
		FadeWidget = NewObject<UBorder>(GetTransientPackage());
#else
		FadeWidget = NewObject<UBorder>(GetTransientPackageAsObject());
#endif
		FadeWidget->SetBrushColor(FLinearColor::Transparent);

		const TSharedRef<SWidget> SlateWidget = FadeWidget->TakeWidget();
		GameViewport->AddViewportWidgetContent(SlateWidget, ZOrder);
		Layer.Widget = FadeWidget;
	}
	else
	{
		// Update ZOrder for existing widget
		const TSharedPtr<SWidget> CachedWidget = FadeWidget->GetCachedWidget();
		if (CachedWidget.IsValid())
		{
			// For GameViewport, need to remove and re-add with new ZOrder
			GameViewport->RemoveViewportWidgetContent(CachedWidget.ToSharedRef());
			GameViewport->AddViewportWidgetContent(CachedWidget.ToSharedRef(), ZOrder);
		}
	}

	FadeWidget->SetVisibility(ESlateVisibility::Visible);

	// Set up fade parameters
	const FLinearColor StartColor = FadeWidget->GetBrushColor().A == 0
		                                ? FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, bIsFadeOut ? 0 : 1)
		                                : FadeWidget->GetBrushColor();
	const FLinearColor ActualTargetColor = FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, bIsFadeOut ? 1 : 0);

	Layer.Params = FShidenFadeParams{
		.FadeDuration = FadeDuration,
		.StartColor = StartColor,
		.TargetColor = ActualTargetColor,
		.bIsFadeOut = bIsFadeOut,
		.EasingAlpha = 0.0f,
		.FadeFunction = FadeFunction,
		.BlendExp = BlendExp,
		.Steps = Steps,
		.OwnerProcessName = TEXT("")
	};

	// Handle instant fade (Duration == 0)
	if (FadeDuration == 0)
	{
		FadeWidget->SetBrushColor(FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, ActualTargetColor.A));
		if (!bIsFadeOut)
		{
			FadeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		// Set EasingAlpha to 1.0f to mark as completed
		Layer.Params.EasingAlpha = 1.0f;
	}
	else
	{
		// Start ticker if not already running
		if (!ShidenSubsystem->TickerHandle.IsValid())
		{
			ShidenSubsystem->TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
				FTickerDelegate::CreateUObject(ShidenSubsystem, &UShidenSubsystem::TickScreenFade));
		}
	}

	// Save scenario property immediately if requested
	if (bSaveScenarioProperty)
	{
		const FString GlobalLayerKey = TEXT("Global$") + LayerName;
		UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(TEXT("Fade"), GlobalLayerKey, {
			{TEXT("Color"), ActualTargetColor.ToString()},
			{TEXT("ZOrder"), FString::FromInt(ZOrder)},
			{TEXT("UseGlobalFade"), TEXT("true")}
		});
	}

	return true;
}

SHIDENCORE_API bool UShidenBlueprintLibrary::IsScreenFadeCompleted(const FString& LayerName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (const FShidenScreenFadeLayer* Layer = ShidenSubsystem->ScreenFadeLayers.Find(LayerName))
	{
		return Layer->Params.EasingAlpha >= 1.0f;
	}
	return true;
}

SHIDENCORE_API void UShidenBlueprintLibrary::ResetScreenFadeLayersCore(UShidenSubsystem* ShidenSubsystem)
{
	// Stop the ticker if it's running
	if (ShidenSubsystem->TickerHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(ShidenSubsystem->TickerHandle);
		ShidenSubsystem->TickerHandle.Reset();
	}

	// Remove all widgets from their parents before clearing
	for (TPair<FString, FShidenScreenFadeLayer>& Pair : ShidenSubsystem->ScreenFadeLayers)
	{
		if (Pair.Value.Widget)
		{
			Pair.Value.Widget->SetVisibility(ESlateVisibility::Collapsed);

			const TSharedPtr<SWidget> CachedWidget = Pair.Value.Widget->GetCachedWidget();
			if (CachedWidget.IsValid())
			{
				if (GEngine && GEngine->GameViewport)
				{
					GEngine->GameViewport->RemoveViewportWidgetContent(CachedWidget.ToSharedRef());
				}
			}

			Pair.Value.Widget->RemoveFromParent();
		}
	}

	ShidenSubsystem->ScreenFadeLayers.Empty();
}

SHIDENCORE_API void UShidenBlueprintLibrary::ResetScreenFadeLayers()
{
	ResetScreenFadeLayersCore(GEngine->GetEngineSubsystem<UShidenSubsystem>());
}

static void AdjustVolumeInternal(UAudioComponent* Component, const float AdjustVolumeDuration, const float AdjustVolumeLevel, const EAudioFaderCurve FadeCurve)
{
	if (!Component)
	{
		return;
	}

	if (!FMath::IsNearlyZero(AdjustVolumeLevel))
	{
		if (Component->Sound && Component->Sound->IsLooping() && !Component->IsActive()
			&& (Component->Sound->VirtualizationMode == EVirtualizationMode::Restart || Component->Sound->VirtualizationMode == EVirtualizationMode::Disabled))
		{
			Component->FadeIn(AdjustVolumeDuration, AdjustVolumeLevel, 0.0f, FadeCurve);
			return;
		}
		Component->AdjustVolume(AdjustVolumeDuration, AdjustVolumeLevel, FadeCurve);
		return;
	}

	// The UAudioComponent::AdjustVolume function treats a volume of 0 as a fade-out, after which volume adjustment is no longer possible.
	// To match the behavior of the volume adjustment function in the Config menu, if it is not EVirtualizationMode::Disabled,
	// a very small value close to 0 is specified so that it is not treated as a fade-out.
	
	constexpr float TargetVolumeLevel = UE_SMALL_NUMBER * 10;
	Component->AdjustVolume(AdjustVolumeDuration, TargetVolumeLevel, FadeCurve);

	const TObjectPtr<UWorld> World = Component->GetWorld();
	if (!World)
	{
		return;
	}

	FTimerHandle Handle;
	World->GetTimerManager()
	     .SetTimer(Handle, [Component]
	               {
		               if (!Component->IsActive())
		               {
			               return;
		               }

		               FAudioDevice* AudioDevice = Component->GetAudioDevice();
		               if (!AudioDevice)
		               {
			               return;
		               }

		               AudioDevice->SendCommandToActiveSounds(
			               Component->GetAudioComponentID(), [](FActiveSound& ActiveSound)
			               {
				               Audio::FVolumeFader& Fader = ActiveSound.ComponentVolumeFader;
				               if (Fader.GetTargetVolume() <= TargetVolumeLevel)
				               {
					               Fader.SetVolume(0.0f);
				               }
			               });
	               }, AdjustVolumeDuration, false
	     );
}

SHIDENCORE_API bool UShidenBlueprintLibrary::TryPlayGlobalBGM(const UObject* WorldContextObject, const int32 TrackId, USoundBase* Sound, const EShidenSoundFadeType FadeType,
                                                              const float Volume, const float Pitch, const float StartTime, const float FadeDuration,
                                                              const EAudioFaderCurve FadeCurve, const bool bSaveScenarioProperty, float& Duration)
{
	Duration = 0.0f;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	const FString GlobalTrackKey = TEXT("Global$") + FString::FromInt(TrackId);

	// Handle stop/fade out when sound is null
	if (!Sound)
	{
		if (const FShidenGlobalBGMInfo* BGMInfo = ShidenSubsystem->GlobalBGMComponents.Find(TrackId); BGMInfo && BGMInfo->AudioComponent)
		{
			BGMInfo->AudioComponent->FadeOut(FadeDuration, 0.0f, FadeCurve);
			ShidenSubsystem->GlobalBGMComponents.Remove(TrackId);
		}
		if (bSaveScenarioProperty)
		{
			UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(TEXT("Sound"), GlobalTrackKey);
		}
		return true;
	}

	const FString SoundPath = UKismetSystemLibrary::GetPathName(Sound);

	// Verify sound type
	EShidenSoundType SoundType;
	if (!TryGetSoundTypeFromSoundBase(Sound, SoundType) || SoundType != EShidenSoundType::BGM)
	{
		SHIDEN_WARNING("Sound asset is not of type BGM: {path}", *SoundPath);
		return false;
	}

	const FShidenGlobalBGMInfo* CurrentBGMInfo = ShidenSubsystem->GlobalBGMComponents.Find(TrackId);
	UAudioComponent* CurrentComponent = CurrentBGMInfo ? CurrentBGMInfo->AudioComponent : nullptr;

	// Get world context for spawning
	const TObjectPtr<UWorld> World = WorldContextObject ? WorldContextObject->GetWorld() : nullptr;
	if (!World)
	{
		return false;
	}

	// Build SoundInfo for internal storage
	const bool bIsFadeIn = FadeType == EShidenSoundFadeType::FadeIn;
	const float ResultStartVolume = bIsFadeIn ? 0.0f : Volume;
	const float ResultEndVolume = bIsFadeIn ? Volume : 0.0f;
	const FShidenSoundInfo SoundInfo(TrackId, EShidenSoundType::BGM, SoundPath, ResultStartVolume, ResultEndVolume, Pitch, StartTime, FadeCurve, FadeDuration);

	// Is fade out?
	if (ResultEndVolume <= 0.0f)
	{
		const bool bSamePath = CurrentComponent && CurrentComponent->Sound &&
			UKismetSystemLibrary::GetPathName(CurrentComponent->Sound).Compare(SoundPath, ESearchCase::CaseSensitive) == 0;
		UAudioComponent* Component = bSamePath
			                             ? CurrentComponent
			                             : UGameplayStatics::SpawnSound2D(World, Sound, 1.0f, Pitch, StartTime);

		if (!bSamePath)
		{
			AdjustVolumeInternal(Component, 0.0f, 1.0f, FadeCurve);
			if (CurrentComponent)
			{
				StopGlobalBGM(TrackId);
			}
			ShidenSubsystem->GlobalBGMComponents.Add(TrackId, FShidenGlobalBGMInfo{Component, SoundInfo});
		}

		AdjustVolumeInternal(Component, FadeDuration, 0.0f, FadeCurve);
		if (bSaveScenarioProperty)
		{
			UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(TEXT("Sound"), GlobalTrackKey);
		}
		Duration = Sound->GetDuration();
		return true;
	}

	// Check if same sound is already playing
	const bool bSamePath = CurrentComponent && CurrentComponent->Sound &&
		UKismetSystemLibrary::GetPathName(CurrentComponent->Sound).Compare(SoundPath, ESearchCase::CaseSensitive) == 0;

	// Fade in for new sound
	if (!bSamePath)
	{
		StopGlobalBGM(TrackId);
		const TObjectPtr<UAudioComponent> Component = UGameplayStatics::SpawnSound2D(World, Sound, 1.0f, Pitch, StartTime);
		ShidenSubsystem->GlobalBGMComponents.Add(TrackId, FShidenGlobalBGMInfo{Component, SoundInfo});
		Component->FadeIn(FadeDuration, Volume, StartTime, FadeCurve);
		if (bSaveScenarioProperty)
		{
			UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(TEXT("Sound"), GlobalTrackKey, {
				{TEXT("Path"), SoundPath},
				{TEXT("Volume"), FString::SanitizeFloat(Volume)},
				{TEXT("Pitch"), FString::SanitizeFloat(Pitch)},
				{TEXT("StartTime"), FString::SanitizeFloat(StartTime)},
				{TEXT("UseGlobalBGM"), TEXT("true")}
			});
		}
		Duration = Sound->GetDuration();
		return true;
	}

	// Volume adjustment for same sound
	ShidenSubsystem->GlobalBGMComponents[TrackId].SoundInfo = SoundInfo;
	AdjustVolumeInternal(CurrentComponent, FadeDuration, Volume, FadeCurve);
	if (bSaveScenarioProperty)
	{
		UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(TEXT("Sound"), GlobalTrackKey, {
			{TEXT("Path"), SoundPath},
			{TEXT("Volume"), FString::SanitizeFloat(Volume)},
			{TEXT("Pitch"), FString::SanitizeFloat(Pitch)},
			{TEXT("StartTime"), FString::SanitizeFloat(StartTime)},
			{TEXT("UseGlobalBGM"), TEXT("true")}
		});
	}
	Duration = Sound->GetDuration();
	return true;
}

SHIDENCORE_API void UShidenBlueprintLibrary::StopGlobalBGM(const int32 TrackId, const bool bSaveScenarioProperty)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (const FShidenGlobalBGMInfo* BGMInfo = ShidenSubsystem->GlobalBGMComponents.Find(TrackId); BGMInfo && BGMInfo->AudioComponent)
	{
		BGMInfo->AudioComponent->SetActive(false);
		ShidenSubsystem->GlobalBGMComponents.Remove(TrackId);
		if (bSaveScenarioProperty)
		{
			UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(TEXT("Sound"), TEXT("Global$") + FString::FromInt(TrackId));
		}
	}
}

SHIDENCORE_API void UShidenBlueprintLibrary::StopGlobalBGMs(const bool bSaveScenarioProperty)
{
	TArray<int32> TrackIds;
	GEngine->GetEngineSubsystem<UShidenSubsystem>()->GlobalBGMComponents.GetKeys(TrackIds);
	for (const int32 TrackId : TrackIds)
	{
		StopGlobalBGM(TrackId, bSaveScenarioProperty);
	}
}

SHIDENCORE_API void UShidenBlueprintLibrary::AdjustGlobalBGMVolume(const int32 TrackId, const float VolumeDuration, const float VolumeLevel, const EAudioFaderCurve FadeCurve)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (const FShidenGlobalBGMInfo* BGMInfo = ShidenSubsystem->GlobalBGMComponents.Find(TrackId); BGMInfo && BGMInfo->AudioComponent)
	{
		AdjustVolumeInternal(BGMInfo->AudioComponent, VolumeDuration, VolumeLevel, FadeCurve);
	}
}

SHIDENCORE_API void UShidenBlueprintLibrary::PauseGlobalBGM(const int32 TrackId, const bool bPause)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (const FShidenGlobalBGMInfo* BGMInfo = ShidenSubsystem->GlobalBGMComponents.Find(TrackId); BGMInfo && BGMInfo->AudioComponent)
	{
		BGMInfo->AudioComponent->SetPaused(bPause);
	}
}

SHIDENCORE_API void UShidenBlueprintLibrary::PauseAllGlobalBGMs(const bool bPause)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	for (const TPair<int32, FShidenGlobalBGMInfo>& Pair : ShidenSubsystem->GlobalBGMComponents)
	{
		PauseGlobalBGM(Pair.Key, bPause);
	}
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

#if WITH_EDITOR
SHIDENCORE_API bool UShidenBlueprintLibrary::TryStartScreenFadePreview(const TObjectPtr<const UShidenWidget> ShidenWidget, const FString& LayerName, const float FadeDuration,
                                                                       const EEasingFunc::Type FadeFunction, const FLinearColor TargetColor, const bool bIsFadeOut,
                                                                       const int32 Steps, const float BlendExp, const int32 ZOrder)
{
	if (!ShidenWidget || !ShidenWidget->BaseLayer)
	{
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	// Create or get the fade layer
	FShidenScreenFadeLayer& Layer = ShidenSubsystem->ScreenFadeLayers.FindOrAdd(LayerName);

	TObjectPtr<UBorder> FadeWidget = Layer.Widget;
	if (!FadeWidget)
	{
#if ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION <= 6
		FadeWidget = NewObject<UBorder>(GetTransientPackage());
#else
		FadeWidget = NewObject<UBorder>(GetTransientPackageAsObject());
#endif
		FadeWidget->SetBrushColor(FLinearColor::Transparent);

		ShidenWidget->BaseLayer->AddChild(FadeWidget);
		if (const TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot = Cast<UCanvasPanelSlot>(FadeWidget->Slot))
		{
			CanvasPanelSlot->SetAnchors(FAnchors(0, 0, 1, 1));
			CanvasPanelSlot->SetOffsets(FMargin(0, 0, 0, 0));
			CanvasPanelSlot->SetZOrder(ZOrder);
		}

		Layer.Widget = FadeWidget;
	}
	else
	{
		// Update ZOrder for existing widget
		if (const TObjectPtr<UCanvasPanelSlot> CanvasPanelSlot = Cast<UCanvasPanelSlot>(FadeWidget->Slot))
		{
			CanvasPanelSlot->SetZOrder(ZOrder);
		}
	}

	FadeWidget->SetVisibility(ESlateVisibility::Visible);

	// Set up fade parameters
	// Note: Unlike TryStartScreenFade, this uses Layer.Params.TargetColor.
	// This is to accurately represent the originally intended color transition during preview.
	// Using FadeWidget->GetBrushColor() could cause color changes that wouldn't occur in actual behavior.
	const FLinearColor StartColor = FadeWidget->GetBrushColor().A == 0
		                                ? FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, bIsFadeOut ? 0 : 1)
		                                : Layer.Params.TargetColor;
	const FLinearColor ActualTargetColor = FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, bIsFadeOut ? 1 : 0);

	Layer.Params = FShidenFadeParams{
		.FadeDuration = FadeDuration,
		.StartColor = StartColor,
		.TargetColor = ActualTargetColor,
		.bIsFadeOut = bIsFadeOut,
		.EasingAlpha = 0.0f,
		.FadeFunction = FadeFunction,
		.BlendExp = BlendExp,
		.Steps = Steps,
		.OwnerProcessName = TEXT("")
	};

	// Handle instant fade (Duration == 0)
	if (FadeDuration == 0)
	{
		FadeWidget->SetBrushColor(FLinearColor(TargetColor.R, TargetColor.G, TargetColor.B, ActualTargetColor.A));
		if (!bIsFadeOut)
		{
			FadeWidget->SetVisibility(ESlateVisibility::Collapsed);
		}
		// Set EasingAlpha to 1.0f to mark as completed
		Layer.Params.EasingAlpha = 1.0f;
		return true;
	}

	// Start ticker if not already running
	if (!ShidenSubsystem->TickerHandle.IsValid())
	{
		ShidenSubsystem->TickerHandle = FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateUObject(ShidenSubsystem, &UShidenSubsystem::TickScreenFade));
	}

	return true;
}
#endif
