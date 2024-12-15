// Copyright (c) 2024 HANON. All Rights Reserved.

// ReSharper disable CppExpressionWithoutSideEffects
#include "Utility/ShidenCoreFunctionLibrary.h"
#include "DelayAction.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Command/ShidenCommandDefinitions.h"
#include "Engine/AssetManager.h"
#include "Misc/OutputDeviceNull.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Save/ShidenSaveSlotsSaveGame.h"
#include "Save/ShidenSystemSaveGame.h"
#include "Editor/MainFrame/Public/Interfaces/IMainFrameModule.h"
#include "Save/ShidenUserSaveGame.h"
#include "System/ShidenBacklogItem.h"
#include "Async/Async.h"
#include "Scenario/ShidenScenarioFunctionLibrary.h"

FPipe UShidenCoreFunctionLibrary::SaveGamePipe = FPipe{ TEXT("SaveGamePipe") };

SHIDENCORE_API void UShidenCoreFunctionLibrary::CopyToClipboard(const FString Str)
{
	FPlatformApplicationMisc::ClipboardCopy(*Str);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetFromClipboard(FString& Dest)
{
	FPlatformApplicationMisc::ClipboardPaste(Dest);
}

SHIDENCORE_API int32 UShidenCoreFunctionLibrary::GetParsedLength(const FString Text)
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
		ResultText.ReplaceInline(*Str1 , *Str2, ESearchCase::CaseSensitive);
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

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCharactersWithParsedLength(const FString Text, const int32 Len)
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
		Pos.Add({ Str1, Str2, -1, -1});
	}

	// pattern like <tagName> ... </>
	const FRegexPattern Pattern2 = FRegexPattern(FString(TEXT("<.+>(.*?)</>")));
	FRegexMatcher Matcher2(Pattern2, Text);

	while (Matcher2.FindNext())
	{
		const int32 Str1 = Matcher2.GetMatchBeginning();
		const int32 Str2 = Matcher2.GetMatchEnding();

		Pos.Add({ Str1, Str2, Matcher2.GetCaptureGroupBeginning(1), Matcher2.GetCaptureGroupEnding(1)});
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

SHIDENCORE_API void UShidenCoreFunctionLibrary::CallFunctionByName(UObject* TargetObject, const FString FunctionName, const FString Parameters)
{
	if (TargetObject == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("targetObject is nullptr"));
		return;
	}

	const FString TrimmedParam = Parameters.TrimStartAndEnd();
	const FString Cmd = TrimmedParam.IsEmpty() ? FunctionName : FString::Printf(TEXT("%s %s"), *FunctionName, *Parameters);
	FOutputDeviceNull Null;

	TargetObject->CallFunctionByNameWithArguments(*Cmd, Null, nullptr, true);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::MultiThreadDelay(UObject* WorldContextObject, const float Duration, const struct FLatentActionInfo LatentInfo)
{
	if (const TObjectPtr<UWorld> World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelayAction(Duration, LatentInfo));
	}
}

SHIDENCORE_API UClass* UShidenCoreFunctionLibrary::ConstructBlueprintClassFromSoftObjectPath(const FSoftObjectPath SoftObjectPath)
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

SHIDENCORE_API UTexture2D* UShidenCoreFunctionLibrary::ConvertSaveTextureToTexture2D(const FShidenSaveTexture SaveTexture) {
	if (SaveTexture.Width == 0 || SaveTexture.Height == 0) {
		return nullptr;
	}

	UTexture2D* Texture = UTexture2D::CreateTransient(SaveTexture.Width, SaveTexture.Height, SaveTexture.Format);

	if (!Texture) {
		return nullptr;
	}

#if WITH_EDITORONLY_DATA
	Texture->MipGenSettings = TMGS_NoMipmaps;
#endif
	Texture->NeverStream = true;
	Texture->SRGB = true;
	Texture->LODGroup = TEXTUREGROUP_Pixels2D;

	FTexture2DMipMap& Mip = Texture->GetPlatformData()->Mips[0];

	void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);
	FMemory::Memcpy(TextureData, SaveTexture.Pixels.GetData(), SaveTexture.Pixels.Num());
	Mip.BulkData.Unlock();

	Texture->UpdateResource();

	return Texture;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::GetScenarioDataAsset(const FString ObjectPath, UShidenScenario*& Scenario, bool& bSuccess)
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

SHIDENCORE_API FString UShidenCoreFunctionLibrary::MakeErrorMessage(const FGuid ScenarioId, const int32 Index, const FString& CommandName, const FString& ErrorMessage)
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

TObjectPtr<UShidenUserSaveGame> UpdateUserSaveGameInstance(const FString& SlotName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UShidenCoreFunctionLibrary::DoesUserDataExist(SlotName)
		                                                         ? Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0))
		                                                         : Cast<UShidenUserSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenUserSaveGame::StaticClass()));
	SaveGameInstance->ScenarioProperties = ShidenSubsystem->CurrentScenarioProperties;
	SaveGameInstance->UserVariables = ShidenSubsystem->UserVariable;
	SaveGameInstance->LocalVariables = ShidenSubsystem->LocalVariables;
	SaveGameInstance->ScenarioProgressStack = ShidenSubsystem->ScenarioProgressStack;
	if (SaveGameInstance->CreatedAt == FDateTime::MinValue())
	{
		SaveGameInstance->CreatedAt = FDateTime::UtcNow();
	}
	SaveGameInstance->UpdatedAt = FDateTime::UtcNow();

	return SaveGameInstance;
}

TObjectPtr<UShidenSaveSlotsSaveGame> UpdateSaveSlotsSaveGameInstance(const FString& SlotName, const FShidenSaveTexture& SaveTexture, const TMap<FString, FString>& SaveSlotMetadata)
{
	TMap<FString, FShidenSaveSlot> SaveSlots = UShidenCoreFunctionLibrary::AcquireSaveSlots();
	const FDateTime CreatedAt = SaveSlots.Contains(SlotName) ? SaveSlots[SlotName].CreatedAt : FDateTime::UtcNow();
	SaveSlots.Add(SlotName, FShidenSaveSlot{ SlotName, SaveSlotMetadata, SaveTexture, CreatedAt, FDateTime::UtcNow() });
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSaveSlotsSaveGame::StaticClass()));
	SaveSlotsInstance->SaveSlots = SaveSlots;
	return SaveSlotsInstance;
}

void UShidenCoreFunctionLibrary::WaitUntilEmpty()
{
	while (true)
	{
		if (!SaveGamePipe.HasWork())
		{
			break;
		}
		FPlatformProcess::Sleep(0.0f);
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::SaveUserData(const FString SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, bool& bSuccess)
{
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		return;
	}

	if (SlotName == TEXT("ShidenSaveSlots"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSaveSlots\" as user save slot name"));
		return;
	}

	WaitUntilEmpty();

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UpdateUserSaveGameInstance(SlotName);
	bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);

	if (!bSuccess)
	{
		return;
	}
	
	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UpdateSaveSlotsSaveGameInstance(SlotName, SaveTexture, SlotMetadata);
	bSuccess = UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::AsyncSaveUserData(const FString& SlotName, UTexture2D* Thumbnail, const TMap<FString, FString>& SlotMetadata, FAsyncSaveDataDelegate SavedDelegate)
{
	if (SlotName == TEXT("ShidenSystemData"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSystemData\" as user save slot name"));
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	if (SlotName == TEXT("ShidenSaveSlots"))
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot use \"ShidenSaveSlots\" as user save slot name"));
		SavedDelegate.ExecuteIfBound(false);
		return;
	}

	const FShidenSaveTexture SaveTexture = FShidenSaveTexture(Thumbnail);
	TObjectPtr<UShidenUserSaveGame> SaveGameInstance = UpdateUserSaveGameInstance(SlotName);
	TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = UpdateSaveSlotsSaveGameInstance(SlotName, SaveTexture, SlotMetadata);

	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SlotName, SaveSlotsInstance, SaveGameInstance, SavedDelegate]
	{
		if (const bool bSaveSlotSuccess = UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0))
		{
			const bool bSaveUserDataSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0);
			AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSaveUserDataSuccess]()
			{
				SavedDelegate.ExecuteIfBound(bSaveUserDataSuccess);
			});
		}
		else
		{
			AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSaveSlotSuccess]()
			{
				SavedDelegate.ExecuteIfBound(bSaveSlotSuccess);
			});
		}
	});
}

TObjectPtr<UShidenSystemSaveGame> UpdateSystemSaveGameInstance()
{
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSystemSaveGame::StaticClass()));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	SaveGameInstance->SystemVariables = ShidenSubsystem->SystemVariable;
	SaveGameInstance->PredefinedSystemVariables = ShidenSubsystem->PredefinedSystemVariable;
	if (SaveGameInstance->CreatedAt == FDateTime::MinValue())
	{
		SaveGameInstance->CreatedAt = FDateTime::UtcNow();
	}
	SaveGameInstance->UpdatedAt = FDateTime::UtcNow();

	return SaveGameInstance;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::SaveSystemData(bool& bSuccess)
{
	WaitUntilEmpty();
	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UpdateSystemSaveGameInstance();
	bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::AsyncSaveSystemData(FAsyncSaveDataDelegate SavedDelegate)
{
	SaveGamePipe.Launch(UE_SOURCE_LOCATION, [SavedDelegate]
	{
		const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = UpdateSystemSaveGameInstance();
		const bool bSuccess = UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("ShidenSystemData"), 0);
		AsyncTask(ENamedThreads::GameThread, [SavedDelegate, bSuccess]()
		{
			SavedDelegate.ExecuteIfBound(bSuccess);
		});
	});
}

SHIDENCORE_API TMap<FString, FShidenSaveSlot> UShidenCoreFunctionLibrary::AcquireSaveSlots()
{
	WaitUntilEmpty();

	if (!DoesUserDataExist(TEXT("ShidenSaveSlots")))
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::CreateSaveGameObject(UShidenSaveSlotsSaveGame::StaticClass()))->SaveSlots;
	}

	return Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSaveSlots"), 0))->SaveSlots;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::LoadUserData(const FString SlotName)
{
	WaitUntilEmpty();

	if (!DoesUserDataExist(SlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("User data does not exist for slot name: %s"), *SlotName);
		return;
	}

	const TObjectPtr<UShidenUserSaveGame> SaveGameInstance = Cast<UShidenUserSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->CurrentScenarioProperties = SaveGameInstance->ScenarioProperties;
	ShidenSubsystem->UserVariable = SaveGameInstance->UserVariables;
	ShidenSubsystem->ScenarioProgressStack = SaveGameInstance->ScenarioProgressStack;
	ShidenSubsystem->LocalVariables = SaveGameInstance->LocalVariables;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::LoadSystemData()
{
	WaitUntilEmpty();

	if (!DoesUserDataExist(TEXT("ShidenSystemData")))
	{
		UE_LOG(LogTemp, Warning, TEXT("System data does not exist"));
		return;
	}

	const TObjectPtr<UShidenSystemSaveGame> SaveGameInstance = Cast<UShidenSystemSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSystemData"), 0));

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	ShidenSubsystem->SystemVariable = SaveGameInstance->SystemVariables;
	ShidenSubsystem->PredefinedSystemVariable = SaveGameInstance->PredefinedSystemVariables;
	// Update platform name
	ShidenSubsystem->PredefinedSystemVariable.PlatformName = UGameplayStatics::GetPlatformName();
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::DeleteUserData(const FString SlotName)
{
	WaitUntilEmpty();

	if (DoesUserDataExist(SlotName))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, 0);
	}

	if (DoesUserDataExist(TEXT("ShidenSaveSlots")))
	{
		const TObjectPtr<UShidenSaveSlotsSaveGame> SaveSlotsInstance = Cast<UShidenSaveSlotsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("ShidenSaveSlots"), 0));
		SaveSlotsInstance->SaveSlots.Remove(SlotName);
		UGameplayStatics::SaveGameToSlot(SaveSlotsInstance, TEXT("ShidenSaveSlots"), 0);
	}
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::DeleteSystemData()
{
	WaitUntilEmpty();

	if (DoesSystemDataExist())
	{
		UGameplayStatics::DeleteGameInSlot(TEXT("ShidenSystemData"), 0);
	}
}

SHIDENCORE_API bool UShidenCoreFunctionLibrary::DoesUserDataExist(const FString SlotName)
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

SHIDENCORE_API bool UShidenCoreFunctionLibrary::DoesSystemDataExist()
{
	WaitUntilEmpty();

	return UGameplayStatics::DoesSaveGameExist(TEXT("ShidenSystemData"), 0);
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::AddBacklogItem(const FShidenCommand Command, const TMap<FString, FString> AdditionalProperties)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	ShidenSubsystem->BacklogItems.Add(FShidenBacklogItem{ Command, AdditionalProperties });
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::UpdateBacklogItem(const int32 Index, const FShidenCommand Command, const TMap<FString, FString> AdditionalProperties)
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

SHIDENCORE_API TArray<FShidenBacklogItem> UShidenCoreFunctionLibrary::GetBacklogItems()
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
		{ TEXT("Default"), FShidenTextType{ true } },
		{ TEXT("Name"), FShidenTextType{ false } }
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
		FString ObjectPath = CommandDefinitionSoftObjectPath.GetAssetPathString();
		TObjectPtr<UShidenCommandDefinitions> CommandDefinitions = Cast<UShidenCommandDefinitions>(StaticLoadObject(UShidenCommandDefinitions::StaticClass(), nullptr, *ObjectPath));
		if (!CommandDefinitions)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load CommandDefinitions from ObjectPath: %s"), *ObjectPath);
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

SHIDENCORE_API TMap<FString, FShidenCommandDefinition> UShidenCoreFunctionLibrary::GetCommandDefinitionsCache()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	
	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		InitCommandDefinitions();
	}

	return ShidenSubsystem->CommandDefinitionCache;
}

SHIDENCORE_API void UShidenCoreFunctionLibrary::ClearAllTempValues()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->CurrentScenarioProperties.Empty();
	ShidenSubsystem->AssetCache.Empty();
	ShidenSubsystem->CommandCache.Empty();
	ShidenSubsystem->CommandDefinitionCache.Empty();
	ShidenSubsystem->ScenarioCache.Empty();

	for (const auto& [ProcessName, _] : ShidenSubsystem->ScenarioProgressStack)
	{
		if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
		{
			bool bSuccess = false;
			const FString LocalKey = MakeLocalVariableKeyInternal(ShidenSubsystem, ProcessName, bSuccess);
			if (!bSuccess)
			{
				continue;
			}
			ShidenSubsystem->LocalVariables.Remove(LocalKey);
		}
	}

	ShidenSubsystem->ScenarioProgressStack.Empty();
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::MakeLocalVariableKeyInternal(const UShidenSubsystem* ShidenSubsystem, const FString& ProcessName, bool& bSuccess)
{
	if (const FShidenScenarioProgressStack* Stack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName))
	{
		const int32 LastIndex = Stack->Stack.Num() - 1;
		bSuccess = true;
		return FString::Printf(TEXT("%s$%d"), *ProcessName, LastIndex);
	}
#if WITH_EDITOR
	if (const TObjectPtr<UGameViewportClient> GameViewport = GEngine->GameViewport)
	{
		if (const TObjectPtr<UWorld> World = GameViewport->GetWorld())
		{
			if (World->WorldType == EWorldType::PIE)
			{
				UE_LOG(LogTemp, Warning, TEXT("Local variable not found: Process Name \"%s\""), *ProcessName);
			}
		}
	}
#else
	UE_LOG(LogTemp, Warning, TEXT("Local variable not found: Process Name \"%s\""), *ProcessName);
#endif
	bSuccess = false;
	return FString();
}

SHIDENCORE_API FString UShidenCoreFunctionLibrary::GetCommandArgument(const FShidenCommand& Command, const FString& ArgName)
{
	return Command.Args.Contains(ArgName) ? Command.Args[ArgName] : TEXT("");
}
