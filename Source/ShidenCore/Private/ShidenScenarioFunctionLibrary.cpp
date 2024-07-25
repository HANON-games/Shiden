// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenScenarioFunctionLibrary.h"
#include <ShidenLoadingAssetInfo.h>

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::IsScenarioPlaying()
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return false;
	}

	return ShidenSubsystem->ScenarioProgressStack.Num() > 0;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::PopScenario(const FString ProcessName, FShidenScenarioProgress& ScenarioProgress, bool& bIsLastElement)
{
	ScenarioProgress = FShidenScenarioProgress();
	bIsLastElement = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		return;
	}

	UShidenVariableFunctionLibrary::RemoveCurrentLocalVariables(ProcessName);

	ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Pop();
	bIsLastElement = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() > 0;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::PushScenario(const FString ProcessName, const UShidenScenario* Scenario)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	ShidenSubsystem->ScenarioCache.Add(Scenario->ScenarioId, const_cast<UShidenScenario*>(Scenario));

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		ShidenSubsystem->ScenarioProgressStack.Add(ProcessName, FShidenScenarioProgressStack());
	}

	ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Push(FShidenScenarioProgress{ Scenario->ScenarioId });
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::RemoveScenario(const FString ProcessName)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		UShidenVariableFunctionLibrary::RemoveCurrentLocalVariables(ProcessName);
		ShidenSubsystem->ScenarioProgressStack.Remove(ProcessName);
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::SetCurrentScenarioIndex(const FString ProcessName, const int32 currentIndex)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().CurrentIndex = currentIndex;
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::PeekScenario(const FString ProcessName, FShidenScenarioProgress& ScenarioProgress, FShidenCancelInfo& CancelInfo, bool& bIsLastElement, bool& bIsMacro, bool& bSuccess)
{
	ScenarioProgress = FShidenScenarioProgress();
	CancelInfo = FShidenCancelInfo();
	bIsLastElement = false;
	bIsMacro = false;
	bSuccess = false;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		NextIndex = ++ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().CurrentIndex;
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::RequestCancelScenario(const FString ProcessName, const FString Reason, const EShidenCancelType CancelType)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

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
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	for (auto& Progress : ShidenSubsystem->ScenarioProgressStack)
	{
		if (!Progress.Value.CancelInfo.bIsRequested)
		{
			Progress.Value.CancelInfo.bIsRequested = true;
			Progress.Value.CancelInfo.Reason = Reason;
			Progress.Value.CancelInfo.Type = CancelType;
		}
	}
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::IsCancelRequested(const FString ProcessName, bool& bIsCancelRequested, FString& CancelReason, EShidenCancelType& CancelType)
{
	bIsCancelRequested = false;
	CancelReason = TEXT("");
	CancelType = EShidenCancelType::None;

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

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

SHIDENCORE_API void UShidenScenarioFunctionLibrary::MarkReadLine(const FString ProcessName, const FGuid CommandGuid)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return;
	}

	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	UShidenScenarioFunctionLibrary::PeekScenario(ProcessName, ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return;
	}

	FShidenReadLines ReadLines = ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines.FindOrAdd(ScenarioProgress.ScenarioId);
	ReadLines.Ids.Add(CommandGuid);
}

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::IsAlreadyRead(const FString ProcessName, const FGuid CommandGuid)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		return false;
	}

	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	UShidenScenarioFunctionLibrary::PeekScenario(ProcessName, ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return false;
	}

	FShidenReadLines* ReadLines = ShidenSubsystem->PredefinedSystemVariable.MarkedReadLines.Find(ScenarioProgress.ScenarioId);
	if (!ReadLines)
	{
		return false;
	}

	return ReadLines->Ids.Contains(CommandGuid);
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::CanSkipCommand(bool& bReturnValue)
{
	bReturnValue = false;

	if (!IsScenarioPlaying())
	{
		return;
	}

	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	bool bIsLastElement = false;
	bool bIsMacro = false;
	bool bSuccess = false;

	PeekScenario(TEXT("Default"), ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro, bSuccess);
	if (!bSuccess)
	{
		return;
	}

	UShidenScenario* Scenario = nullptr;
	UShidenScenarioFunctionLibrary::GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
	if (!bSuccess)
	{
		return;
	}

	bool bSkipUnread;
	UShidenVariableFunctionLibrary::GetSkipUnread(bSkipUnread);

	bReturnValue = IsAlreadyRead(TEXT("Default"), Scenario->Commands[ScenarioProgress.CurrentIndex].CommandId) || bSkipUnread;
}

SHIDENCORE_API bool UShidenScenarioFunctionLibrary::IsEndOfScenario(const FString ProcessName)
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
	UShidenScenarioFunctionLibrary::GetScenarioFromCache(ScenarioProgress.ScenarioId, Scenario, bSuccess);
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

	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem) {
		return;
	}

	TObjectPtr <UShidenScenario>* TempScenario = ShidenSubsystem->ScenarioCache.Find(ScenarioId);
	if (TempScenario) {
		Scenario = *TempScenario;
		bSuccess = true;
		return;
	}

	const UShidenProjectConfig* ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

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
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), ScenarioPath);
		return;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	TObjectPtr <UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), ScenarioPath);
		return;
	}

	Scenario = Cast<UShidenScenario>(LoadedAsset);
	if (!Scenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), ScenarioPath);
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

	bool bParseSuccess = FGuid::Parse(ScenarioIdOrObjectPath, ScenarioId);
	if (bParseSuccess)
	{
		GetScenarioFromCache(ScenarioId, Scenario, bSuccess);
		return;
	}

	TObjectPtr <const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	TArray <FGuid> ScenarioIds = TArray <FGuid>();
	for (auto& ScenarioPath : ProjectConfig->ScenarioPaths)
	{
		if (ScenarioPath.Value == ScenarioIdOrObjectPath)
		{
			GetScenarioFromCache(ScenarioPath.Key, Scenario, bSuccess);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Error: Scenario \"%s\" is not found in scenario paths."), *ScenarioIdOrObjectPath);
	return;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::ConstructCommand(const FString ProcessName, const FShidenCommand OriginalCommand, FShidenCommand& Command)
{
	if (OriginalCommand.PresetName.IsEmpty())
	{
		Command = UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(ProcessName, OriginalCommand);
		return;
	}

	const UShidenProjectConfig* ProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString OriginalCommandName = OriginalCommand.CommandName;
	const FString OriginalPresetName = OriginalCommand.PresetName;

	const FShidenPreset* Preset = ProjectConfig->Presets.FindByPredicate([OriginalCommandName, OriginalPresetName](const FShidenPreset& Preset) {
		return Preset.CommandName == OriginalCommandName && Preset.PresetName == OriginalPresetName;
	});
	if (!Preset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Preset \"%s\" is not found."), *OriginalCommand.PresetName);
		Command = UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(ProcessName, OriginalCommand);
		return;
	}

	FShidenCommand TempCommand = FShidenCommand
	{
		OriginalCommand.CommandName,
		OriginalCommand.PresetName,
		OriginalCommand.Arg1.IsEmpty() ? Preset->Arg1 : OriginalCommand.Arg1,
		OriginalCommand.Arg2.IsEmpty() ? Preset->Arg2 : OriginalCommand.Arg2,
		OriginalCommand.Arg3.IsEmpty() ? Preset->Arg3 : OriginalCommand.Arg3,
		OriginalCommand.Arg4.IsEmpty() ? Preset->Arg4 : OriginalCommand.Arg4,
		OriginalCommand.Arg5.IsEmpty() ? Preset->Arg5 : OriginalCommand.Arg5,
		OriginalCommand.Arg6.IsEmpty() ? Preset->Arg6 : OriginalCommand.Arg6,
		OriginalCommand.Arg7.IsEmpty() ? Preset->Arg7 : OriginalCommand.Arg7,
		OriginalCommand.Arg8.IsEmpty() ? Preset->Arg8 : OriginalCommand.Arg8,
		OriginalCommand.Arg9.IsEmpty() ? Preset->Arg9 : OriginalCommand.Arg9,
		OriginalCommand.Arg10.IsEmpty() ? Preset->Arg10 : OriginalCommand.Arg10,
		OriginalCommand.CommandId,
		OriginalCommand.bEnabled
	};

	Command = UShidenVariableFunctionLibrary::ReplaceAllTextWithVariable(ProcessName, OriginalCommand);
	return;
}

SHIDENCORE_API void GetCommand(UObject* Outer, const FSoftObjectPath CommandSoftObjectPath, UShidenCommandObject*& CommandObject, bool& Success)
{
	CommandObject = nullptr;
	Success = false;

	TObjectPtr<UClass> Class = UShidenCoreFunctionLibrary::ConstructBlueprintClassFromSoftObjectPath(CommandSoftObjectPath);
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
	return;
}

SHIDENCORE_API void UShidenScenarioFunctionLibrary::GetCommandFromCache(UObject* Outer, const FString ProcessName, const FSoftObjectPath CommandSoftObjectPath, UShidenCommandObject*& Command, bool& bSuccess)
{
	TObjectPtr <UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		bSuccess = false;
		return;
	}

	FString CommandCacheKey = ProcessName + TEXT("::") + CommandSoftObjectPath.GetAssetPathString();
	TObjectPtr <UShidenCommandObject>* CommandPtr = ShidenSubsystem->CommandCache.Find(CommandCacheKey);
	if (CommandPtr)
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
	return;
}

SHIDENCORE_API void LoadScenarioAssetPaths(UObject* Outer, const UShidenScenario* Scenario, TArray <FShidenLoadingAssetInfo>& AssetInfo, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex)
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
	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		UShidenCoreFunctionLibrary::InitCommandDefinitions();
	}

	for (int32 Index = 0; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& OriginalCommand = Scenario->Commands[Index];
		FShidenCommand Command = FShidenCommand();
		UShidenScenarioFunctionLibrary::ConstructCommand(TEXT("Default"), OriginalCommand, Command);
		
		if (!Command.bEnabled) {
			continue;
		}

		if (Command.CommandName == TEXT("RunMacro"))
		{
			if (Command.Arg1.IsEmpty())
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Arg1 is empty. Index: %d"), Index);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			FGuid TempScenarioId = FGuid();
			bool bTempSuccess = false;
			UShidenScenario* MacroScenario = nullptr;
			UShidenScenarioFunctionLibrary::GetScenarioByIdOrObjectPath(Command.Arg1, TempScenarioId, MacroScenario, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found. Index: %d"), *Command.Arg1, Index);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TArray <FShidenLoadingAssetInfo> TempAssetInfo = TArray<FShidenLoadingAssetInfo>();
			LoadScenarioAssetPaths(Outer, MacroScenario, TempAssetInfo, bTempSuccess, ErrorMessage, ErrorScenarioId, ErrorIndex);
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
			if (Command.Arg1.IsEmpty())
			{
				ErrorMessage = FString::Printf(TEXT("RunMacroAsParallel: Arg1 is empty. Index: %d"), Index);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			if (Command.Arg2.IsEmpty())
			{
				ErrorMessage = FString::Printf(TEXT("RunMacroAsParallel: Arg2 is empty. Index: %d"), Index);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			FGuid TempScenarioId = FGuid();
			bool bTempSuccess = false;
			UShidenScenario* MacroScenario = nullptr;
			UShidenScenarioFunctionLibrary::GetScenarioByIdOrObjectPath(Command.Arg2, TempScenarioId, MacroScenario, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found. Index: %d"), *Command.Arg2, Index);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TArray <FShidenLoadingAssetInfo> TempAssetInfo = TArray<FShidenLoadingAssetInfo>();
			LoadScenarioAssetPaths(Outer, MacroScenario, TempAssetInfo, bTempSuccess, ErrorMessage, ErrorScenarioId, ErrorIndex);
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
			FShidenCommandDefinition* Definition = ShidenSubsystem->CommandDefinitionCache.Find(Command.CommandName);
			GetCommand(Outer, Definition->CommandBlueprint, CommandObject, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("Get Command \"%s\" Failed.Please check project config and command definitions."), *Command.CommandName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TSet<FString> TempAssetPaths = TSet<FString>();
			CommandObject->GetAssetPaths(Command, TempAssetPaths);
			TempAssetPaths.Remove(TEXT(""));
			for (const FString& AssetPath : TempAssetPaths)
			{
				AssetInfo.AddUnique(FShidenLoadingAssetInfo{ Index, AssetPath });
			}
		}
	}

	bSuccess = true;
	return;

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
		const UShidenProjectConfig* ProjectConfig = GetDefault<UShidenProjectConfig>();
		const FString* ScenarioPath = ProjectConfig->ScenarioPaths.Find(ScenarioProgress.ScenarioId);
		ErrorMessage = FString::Printf(TEXT("Load scenario failed: %s"), ScenarioPath);
		ErrorScenarioId = ScenarioProgress.ScenarioId;
		ErrorIndex = -1;
		return;
	}

	UShidenSubsystem* ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	if (!ShidenSubsystem)
	{
		ErrorMessage = TEXT("ShidenSubsystem is null");
		ErrorScenarioId = ScenarioProgress.ScenarioId;
		ErrorIndex = -1;
		return;
	}

	ShidenSubsystem->AssetCache.Empty();

	TArray <FShidenLoadingAssetInfo> AssetInfo = TArray<FShidenLoadingAssetInfo>();
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
		const FShidenLoadingAssetInfo& LoadingAssetInfo = AssetInfo[Index];
		UObject* LoadedAsset = Streamable.LoadSynchronous(LoadingAssetInfo.ObjectPath, false);
		if (!LoadedAsset)
		{
			ErrorMessage = FString::Printf(TEXT("Load asset failed: %s"), *LoadingAssetInfo.ObjectPath);
			ErrorScenarioId = ScenarioProgress.ScenarioId;
			ErrorIndex = LoadingAssetInfo.ScenarioIndex;
			return;
		}

		ShidenSubsystem->AssetCache.Add(LoadingAssetInfo.ObjectPath, LoadedAsset);
	}

	bSuccess = true;
	return;
}

SHIDENCORE_API void MakeObjectPathFromPackagePath(const FString PackagePath, FName& ObjectPath)
{
	FString ObjectPathString;
	FString Right;
	PackagePath.Split(TEXT("."), nullptr, &Right);
	ObjectPathString = PackagePath + TEXT(".") + Right;
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
	TObjectPtr <UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load asset from ObjectPath: %s"), *ObjectPath.ToString());
		bSuccess = false;
		return;
	}

	TObjectPtr <UShidenScenario> ShidenScenario = Cast<UShidenScenario>(LoadedAsset);
	if (!ShidenScenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast loaded asset to UShidenScenario"));
		bSuccess = false;
		return;
	}

	OutScenario = ShidenScenario;
	bSuccess = true;
	return;
}