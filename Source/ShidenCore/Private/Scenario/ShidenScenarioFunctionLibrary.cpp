// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Scenario/ShidenScenarioFunctionLibrary.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"
#include "System/ShidenLoadingAssetInfo.h"

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::IsScenarioPlaying()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->ScenarioProgressStack.Num() > 0;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::PopScenario(const FString ProcessName, FShidenScenarioProgress& ScenarioProgress, bool& bIsLastElement)
{
	ScenarioProgress = FShidenScenarioProgress();
	bIsLastElement = false;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		return;
	}

	UShidenVariableFunctionLibrary::RemoveCurrentLocalVariables(ProcessName);

	if (ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		bIsLastElement = true;
		return;
	}

	ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Pop();
	bIsLastElement = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() > 0;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::PushScenario(const FString ProcessName, const UShidenScenario* Scenario)
{
	if (!Scenario)
	{
		return;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->ScenarioCache.Add(Scenario->ScenarioId, const_cast<UShidenScenario*>(Scenario));

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		ShidenSubsystem->ScenarioProgressStack.Add(ProcessName, FShidenScenarioProgressStack());
	}

	ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Push(FShidenScenarioProgress{ Scenario->ScenarioId });
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::RemoveScenario(const FString ProcessName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		UShidenVariableFunctionLibrary::RemoveCurrentLocalVariables(ProcessName);
		ShidenSubsystem->ScenarioProgressStack.Remove(ProcessName);
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::SetCurrentScenarioIndex(const FString ProcessName, const int32 CurrentIndex)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().CurrentIndex = CurrentIndex;
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::PeekScenario(const FString ProcessName, FShidenScenarioProgress& ScenarioProgress, FShidenCancelInfo& CancelInfo, bool& bIsLastElement, bool& bIsMacro, bool& bSuccess)
{
	ScenarioProgress = FShidenScenarioProgress();
	CancelInfo = FShidenCancelInfo();
	bIsLastElement = false;
	bIsMacro = false;
	bSuccess = false;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName)
		|| ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		return;
	}

	ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last();
	CancelInfo = ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo;
	bIsLastElement = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 1;
	bSuccess = true;
	bIsMacro = !bIsLastElement || ProcessName != TEXT("Default");
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::ToNext(const FString ProcessName, int& NextIndex)
{
	NextIndex = 0;
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		NextIndex = ++ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().CurrentIndex;
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::RequestCancelScenario(const FString ProcessName, const FString Reason, const EShidenCancelType CancelType)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		if (!ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.bIsRequested)
		{
			ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.bIsRequested = true;
			ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.Reason = Reason;
			ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.Type = CancelType;
		}
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::RequestCancelScenarioAll(const FString Reason, const EShidenCancelType CancelType)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	for (TTuple<FString, FShidenScenarioProgressStack>& Progress : ShidenSubsystem->ScenarioProgressStack)
	{
		if (!Progress.Value.CancelInfo.bIsRequested)
		{
			Progress.Value.CancelInfo.bIsRequested = true;
			Progress.Value.CancelInfo.Reason = Reason;
			Progress.Value.CancelInfo.Type = CancelType;
		}
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::IsCancelRequested(const FString& ProcessName, bool& bIsCancelRequested, FString& CancelReason, EShidenCancelType& CancelType)
{
	bIsCancelRequested = false;
	CancelReason = TEXT("");
	CancelType = EShidenCancelType::None;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		bIsCancelRequested = ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.bIsRequested;
		CancelReason = ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.Reason;
		CancelType = ShidenSubsystem->ScenarioProgressStack[ProcessName].CancelInfo.Type;
	}
	else 
	{
		bIsCancelRequested = true;
		CancelReason = TEXT("Stack Removed");
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::MarkAsRead(const FString ProcessName, const FGuid CommandGuid)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	PeekScenario(ProcessName, ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return;
	}

	FShidenReadLines ReadLines = ShidenSubsystem->PredefinedSystemVariable.ScenarioReadLines.FindOrAdd(ScenarioProgress.ScenarioId);
	ReadLines.CommandGuids.Add(CommandGuid);
}

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::IsRead(const FString& ProcessName, const FGuid CommandGuid)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	PeekScenario(ProcessName, ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return false;
	}

	const FShidenReadLines* ReadLines = ShidenSubsystem->PredefinedSystemVariable.ScenarioReadLines.Find(ScenarioProgress.ScenarioId);
	if (!ReadLines)
	{
		return false;
	}

	return ReadLines->CommandGuids.Contains(CommandGuid);
}

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::CanSkipCommand()
{
	if (!IsScenarioPlaying())
	{
		return false;
	}

	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	PeekScenario(TEXT("Default"), ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return false;
	}

	UShidenScenario* Scenario = nullptr;
	GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
	if (!bSuccess)
	{
		return false;
	}

	bool bSkipUnread;
	UShidenVariableFunctionLibrary::GetSkipUnread(bSkipUnread);

	if (Scenario->Commands.Num() <= ScenarioProgress.CurrentIndex)
	{
		return false;
	}

	return IsRead(TEXT("Default"), Scenario->Commands[ScenarioProgress.CurrentIndex].CommandId) || bSkipUnread;
}

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::IsEndOfScenario(const FString& ProcessName)
{
	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	PeekScenario(ProcessName, ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return true;
	}

	UShidenScenario* Scenario = nullptr;
	GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
	if (!bSuccess)
	{
		return true;
	}

	return Scenario->Commands.Num() <= ScenarioProgress.CurrentIndex;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::GetScenarioFromCache(const FGuid ScenarioId, UShidenScenario*& Scenario, bool& bSuccess)
{
	Scenario = nullptr;
	bSuccess = false;

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (TObjectPtr<UShidenScenario>* TempScenario = ShidenSubsystem->ScenarioCache.Find(ScenarioId)) {
		Scenario = *TempScenario;
		bSuccess = true;
		return;
	}

	TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString* ScenarioPath = ShidenProjectConfig->ScenarioPaths.Find(ScenarioId);
	if (!ScenarioPath) {
		UE_LOG(LogTemp, Warning, TEXT("Error: Scenario \"%s\" is not found in scenario paths.Please restart the editor."), *ScenarioId.ToString(EGuidFormats::DigitsWithHyphensInBraces));
		return;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(*ScenarioPath));

	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), (TCHAR*)ScenarioPath);
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	TObjectPtr<UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), (TCHAR*)ScenarioPath);
		return;
	}

	Scenario = Cast<UShidenScenario>(LoadedAsset);
	if (!Scenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), (TCHAR*)ScenarioPath);
		return;
	}

	ShidenSubsystem->ScenarioCache.Add(ScenarioId, Scenario);
	bSuccess = true;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::GetScenarioByIdOrObjectPath(const FString ScenarioIdOrObjectPath, FGuid& ScenarioId, UShidenScenario*& Scenario, bool& bSuccess)
{
	ScenarioId = FGuid();
	Scenario = nullptr;
	bSuccess = false;

	if (FGuid::Parse(ScenarioIdOrObjectPath, ScenarioId))
	{
		GetScenarioFromCache(ScenarioId, Scenario, bSuccess);
		return;
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	TArray<FGuid> ScenarioIds = TArray<FGuid>();
	for (auto& ScenarioPath : ProjectConfig->ScenarioPaths)
	{
		if (ScenarioPath.Value == ScenarioIdOrObjectPath)
		{
			GetScenarioFromCache(ScenarioPath.Key, Scenario, bSuccess);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Error: Scenario \"%s\" is not found in scenario paths."), *ScenarioIdOrObjectPath);
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::ConstructCommand(const FString ProcessName, const FShidenCommand OriginalCommand, const bool bReplaceWithVariable, FShidenCommand& Command)
{
	if (OriginalCommand.PresetName.IsEmpty())
	{
		Command = bReplaceWithVariable
			? UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(ProcessName, OriginalCommand)
			: OriginalCommand;
		return;
	}

	TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString OriginalCommandName = OriginalCommand.CommandName;
	const FString OriginalPresetName = OriginalCommand.PresetName;

	const FShidenPreset* Preset = ProjectConfig->Presets.FindByPredicate([OriginalCommandName, OriginalPresetName](const FShidenPreset& Preset) {
		return Preset.CommandName == OriginalCommandName && Preset.PresetName == OriginalPresetName;
	});

	if (!Preset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Preset \"%s\" is not found."), *OriginalCommand.PresetName);
		Command = bReplaceWithVariable
			? UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(ProcessName, OriginalCommand)
			: OriginalCommand;
		return;
	}

	FShidenCommand TempCommand = FShidenCommand();

	TempCommand.CommandName = OriginalCommand.CommandName;
	TempCommand.PresetName = OriginalCommand.PresetName;
	TempCommand.CommandId = OriginalCommand.CommandId;
	TempCommand.bEnabled = OriginalCommand.bEnabled;
	TempCommand.Args = OriginalCommand.Args;

	TArray<FString> PresetArgKeys;

	Preset->Args.GetKeys(PresetArgKeys);
	for (const FString& Key : PresetArgKeys)
	{
		if (TempCommand.Args.FindRef(Key).IsEmpty())
		{
			TempCommand.Args.Add(Key, Preset->Args.FindRef(Key));
		}
	}

	Command = bReplaceWithVariable
		? UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(ProcessName, TempCommand)
		: TempCommand;
}

SHIDENCORE_API void GetCommand(UObject* Outer, const FSoftObjectPath& CommandSoftObjectPath, UShidenCommandObject*& CommandObject, bool& Success)
{
	CommandObject = nullptr;
	Success = false;

	const TObjectPtr<UClass> Class = UShidenCoreFunctionLibrary::ConstructBlueprintClassFromSoftObjectPath(CommandSoftObjectPath);
	if (!Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load class from ObjectPath: %s"), *CommandSoftObjectPath.GetAssetPathString());
		return;
	}

	CommandObject = NewObject<UShidenCommandObject>(Outer, Class);
	if (!CommandObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UShidenCommandObject from ObjectPath: %s"), *CommandSoftObjectPath.GetAssetPathString());
		return;
	}

	Success = true;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::GetCommandFromCache(UObject* Outer, const FString ProcessName, const FSoftObjectPath CommandSoftObjectPath, UShidenCommandObject*& Command, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const FString CommandCacheKey = ProcessName + TEXT("::") + CommandSoftObjectPath.GetAssetPathString();
	if (const TObjectPtr<UShidenCommandObject>* CommandPtr = ShidenSubsystem->CommandCache.Find(CommandCacheKey))
	{
		Command = *CommandPtr;
		bSuccess = true;
		return;
	}

	bool bTempSuccess = false;
	GetCommand(Outer, CommandSoftObjectPath, Command, bTempSuccess);

	if (!bTempSuccess)
	{
		bSuccess = false;
		return;
	}

	ShidenSubsystem->CommandCache.Add(CommandCacheKey, Command);
	bSuccess = true;
}

SHIDENCORE_API void LoadScenarioAssetPaths(UObject* CallerObject, const UShidenScenario* Scenario, TArray<FShidenLoadingAssetInfo>& AssetInfo, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex)
{
	AssetInfo.Empty();
	bSuccess = false;
	ErrorMessage = TEXT("");
	ErrorScenarioId = FGuid();
	ErrorIndex = -1;

	if (!Scenario)
	{
		ErrorMessage = TEXT("Scenario is null");
		return;
	}

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		UShidenCoreFunctionLibrary::InitCommandDefinitions();
	}

	for (int32 Index = 0; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& OriginalCommand = Scenario->Commands[Index];
		FShidenCommand Command = FShidenCommand();
		UShidenScenarioFunctionLibrary::ConstructCommand(TEXT("Default"), OriginalCommand, false, Command);
		
		if (!Command.bEnabled) {
			continue;
		}

		if (Command.CommandName == TEXT("RunMacro"))
		{
			if (!Command.Args.Contains("MacroName") || Command.Args["MacroName"].IsEmpty())
			{
				ErrorMessage = TEXT("RunMacro: MacroName is empty.");
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			FGuid TempScenarioId = FGuid();
			bool bTempSuccess = false;
			UShidenScenario* MacroScenario = nullptr;
			UShidenScenarioFunctionLibrary::GetScenarioByIdOrObjectPath(Command.Args["MacroName"], TempScenarioId, MacroScenario, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found."), *Command.Args["MacroName"]);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TArray<FShidenLoadingAssetInfo> TempAssetInfo = TArray<FShidenLoadingAssetInfo>();
			LoadScenarioAssetPaths(CallerObject, MacroScenario, TempAssetInfo, bTempSuccess, ErrorMessage, ErrorScenarioId, ErrorIndex);
			if (!bTempSuccess)
			{
				return;
			}
			for (const FShidenLoadingAssetInfo& Temp : TempAssetInfo)
			{
				AssetInfo.AddUnique(Temp);
			}
		}
		else if (Command.CommandName == TEXT("RunMacroAsParallel"))
		{
			if (!Command.Args.Contains("NewProcessName") || Command.Args["NewProcessName"].IsEmpty())
			{
				ErrorMessage = TEXT("RunMacroAsParallel: NewProcessName is empty.");
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			if (!Command.Args.Contains("MacroName") || Command.Args["MacroName"].IsEmpty())
			{
				ErrorMessage = TEXT("RunMacroAsParallel: MacroName is empty.");
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			FGuid TempScenarioId = FGuid();
			bool bTempSuccess = false;
			UShidenScenario* MacroScenario = nullptr;
			UShidenScenarioFunctionLibrary::GetScenarioByIdOrObjectPath(Command.Args["MacroName"], TempScenarioId, MacroScenario, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found."), *Command.Args["MacroName"]);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TArray<FShidenLoadingAssetInfo> TempAssetInfo;
			LoadScenarioAssetPaths(CallerObject, MacroScenario, TempAssetInfo, bTempSuccess, ErrorMessage, ErrorScenarioId, ErrorIndex);
			if (!bTempSuccess)
			{
				return;
			}
			for (const FShidenLoadingAssetInfo& Temp : TempAssetInfo)
			{
				AssetInfo.AddUnique(Temp);
			}
		}
		else
		{
			bool bTempSuccess = false;
			UShidenCommandObject* CommandObject = nullptr;
			if (FShidenCommandDefinition* Definition = ShidenSubsystem->CommandDefinitionCache.Find(Command.CommandName))
			{
				GetCommand(CallerObject, Definition->CommandBlueprint, CommandObject, bTempSuccess);
			}
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Get Command \"%s\" Failed.Please check project config and command definitions."), *Command.CommandName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TSet<FString> TempAssetPaths = TSet<FString>();
			CommandObject->GetAssetPaths(Command, TempAssetPaths, CallerObject);
			TempAssetPaths.Remove(TEXT(""));
			for (const FString& AssetPath : TempAssetPaths)
			{
				AssetInfo.AddUnique(FShidenLoadingAssetInfo{ Index, AssetPath });
			}
		}
	}

	bSuccess = true;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::LoadScenarioAssets(UObject* Outer, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex)
{
	bSuccess = false;
	ErrorMessage = TEXT("");
	ErrorScenarioId = FGuid();
	ErrorIndex = -1;

	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bTempSuccess = false;

	PeekScenario(TEXT("Default"), ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bTempSuccess);
	if (!bTempSuccess)
	{
		ErrorMessage = TEXT("Load scenario failed.");
		ErrorIndex = -1;
		return;
	}

	UShidenScenario* Scenario = nullptr;
	GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bTempSuccess);
	if (!bTempSuccess)
	{
		const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
		const FString* ScenarioPath = ProjectConfig->ScenarioPaths.Find(ScenarioProgress.ScenarioId);
		ErrorMessage = FString::Printf(TEXT("Load scenario failed: %p"), ScenarioPath);
		ErrorScenarioId = ScenarioProgress.ScenarioId;
		ErrorIndex = -1;
		return;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();

	TArray<FShidenLoadingAssetInfo> AssetInfo = TArray<FShidenLoadingAssetInfo>();
	FString TempErrorMessage = TEXT("");
	FGuid TempErrorScenarioId = FGuid();
	int32 TempErrorIndex = -1;
	LoadScenarioAssetPaths(Outer, Scenario, AssetInfo, bTempSuccess, TempErrorMessage, TempErrorScenarioId, TempErrorIndex);
	if (!bTempSuccess)
	{
		ErrorMessage = TempErrorMessage;
		ErrorScenarioId = TempErrorScenarioId;
		ErrorIndex = TempErrorIndex;
		return;
	}
	
	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();

	for (int32 Index = 0; Index < AssetInfo.Num(); Index++)
	{
		const auto& [ScenarioIndex, ObjectPath] = AssetInfo[Index];

		// In UnrealEngine, an error occurs if "{" or "}" is included in the asset name,
		// so if they are included, they are treated as variables and skipped.
		if (ObjectPath.Contains(TEXT("{")) || ObjectPath.Contains(TEXT("}")))
		{
			continue;
		}
		
		TObjectPtr<UObject> LoadedAsset = Streamable.LoadSynchronous(ObjectPath, false);
		if (!LoadedAsset)
		{
			ErrorMessage = FString::Printf(TEXT("Load asset failed: %s"), *ObjectPath);
			ErrorScenarioId = ScenarioProgress.ScenarioId;
			ErrorIndex = ScenarioIndex;
			return;
		}

		ShidenSubsystem->AssetCache.Add(ObjectPath, LoadedAsset);
	}

	bSuccess = true;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::InitFromSaveData(const UShidenWidget* Widget, const TScriptInterface<IShidenScenarioManagerInterface> ScenarioManager, UObject* CallerObject, bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		UShidenCoreFunctionLibrary::InitCommandDefinitions();
	}

	for (const auto& [CommandName, ScenarioProperties] : ShidenSubsystem->CurrentScenarioProperties)
	{
		const FSoftObjectPath CommandPath = ShidenSubsystem->CommandDefinitionCache.FindRef(CommandName).CommandBlueprint;
		UShidenCommandObject* CommandObject = nullptr;
		bool bTempSuccess = false;
		GetCommandFromCache(CallerObject, TEXT("Default"), CommandPath, CommandObject, bTempSuccess);
		if (!bTempSuccess)
		{
			ErrorMessage = FString::Printf(TEXT("Get Command \"%s\" Failed. Please check project config and command definitions."), *CommandName);
			return;
		}
		EShidenInitFromSaveDataStatus Status = EShidenInitFromSaveDataStatus::Complete;
		CommandObject->RestoreFromSaveData(ScenarioProperties.ScenarioProperties, Widget, ScenarioManager, CallerObject, Status, ErrorMessage);
		if (Status == EShidenInitFromSaveDataStatus::Error)
		{
			bSuccess = false;
			return;
		}
	}

	bSuccess = true;
}

SHIDENCORE_API void MakeObjectPathFromPackagePath(const FString& PackagePath, FName& ObjectPath)
{
	FString Right;
	PackagePath.Split(TEXT("."), nullptr, &Right);
	const FString ObjectPathString = PackagePath + TEXT(".") + Right;
	ObjectPath = FName(*ObjectPathString);
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::GetScenarioByPackagePath(const FString& PackagePath, UShidenScenario*& OutScenario, bool& bSuccess)
{
	OutScenario = nullptr;

	if (PackagePath.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("PackagePath is empty."));
		bSuccess = false;
		return;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	FName ObjectPath = FName();

	MakeObjectPathFromPackagePath(PackagePath, ObjectPath);

	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(ObjectPath.ToString()));
	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AssetData is invalid for ObjectPath: %s"), *ObjectPath.ToString());
		bSuccess = false;
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	TObjectPtr<UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load asset from ObjectPath: %s"), *ObjectPath.ToString());
		bSuccess = false;
		return;
	}

	TObjectPtr<UShidenScenario> ShidenScenario = Cast<UShidenScenario>(LoadedAsset);
	if (!ShidenScenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded asset to UShidenScenario"));
		bSuccess = false;
		return;
	}

	OutScenario = ShidenScenario;
	bSuccess = true;
}