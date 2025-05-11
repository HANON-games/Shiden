// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"
#include "System/ShidenLoadingAssetInfo.h"

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::IsScenarioPlaying()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);
	return ShidenSubsystem->ScenarioProgressStack.Num() > 0;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::PopScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress,
                                                                 bool& bIsLastElement)
{
	ScenarioProgress = FShidenScenarioProgress();
	bIsLastElement = false;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		return;
	}

	UShidenVariableBlueprintLibrary::RemoveLocalVariablesInCurrentScope(ProcessName);

	if (ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0)
	{
		bIsLastElement = true;
		return;
	}

	ScenarioProgress = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Pop();
	bIsLastElement = ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Num() == 0;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::PushScenario(const FString& ProcessName, const UShidenScenario* Scenario)
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

	ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Push(FShidenScenarioProgress{Scenario->ScenarioId});
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RemoveScenario(const FString& ProcessName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		UShidenVariableBlueprintLibrary::RemoveLocalVariablesInProcess(ProcessName);
		ShidenSubsystem->ScenarioProgressStack.Remove(ProcessName);
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::SetCurrentScenarioIndex(const FString& ProcessName, const int32 CurrentIndex)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		ShidenSubsystem->ScenarioProgressStack[ProcessName].SetCurrentIndex(CurrentIndex);
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::PeekScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress,
                                                                  FShidenCancelInfo& CancelInfo, bool& bIsLastElement, bool& bIsMacro, bool& bSuccess)
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::ToNext(const FString& ProcessName, int& NextIndex)
{
	NextIndex = 0;
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->ScenarioProgressStack.Contains(ProcessName))
	{
		NextIndex = ++ShidenSubsystem->ScenarioProgressStack[ProcessName].Stack.Last().CurrentIndex;
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RequestCancelScenario(const FString& ProcessName, const FString& Reason,
                                                                           const EShidenCancelType CancelType)
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RequestCancelScenarioAll(const FString& Reason, const EShidenCancelType CancelType)
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::IsCancelRequested(const FString& ProcessName, bool& bIsCancelRequested,
                                                                       FString& CancelReason, EShidenCancelType& CancelType)
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

SHIDENCORE_API FGuid GenerateReadLineHashFromScenarioProgressStack(TArray<FShidenScenarioProgress>& Stack)
{
	FGuid Result;
	bool bIsFirst = true;
	UShidenScenario* Scenario;
	bool bSuccess = false;
	for (const auto& [ScenarioId, CurrentIndex] : Stack)
	{
		UShidenScenarioBlueprintLibrary::GetScenarioFromCache(ScenarioId, Scenario, bSuccess);
		if (bSuccess && Scenario->Commands.IsValidIndex(CurrentIndex))
		{
			if (bIsFirst)
			{
				Result = Scenario->Commands[CurrentIndex].CommandId;
				bIsFirst = false;
			}
			else
			{
				Result = FGuid::Combine(Result, Scenario->Commands[CurrentIndex].CommandId);
			}
		}
	}
	return Result;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::MarkAsRead()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!IsScenarioPlaying())
	{
		return;
	}

	TArray<FShidenScenarioProgress>* Stack = &ShidenSubsystem->ScenarioProgressStack["Default"].Stack;
	if (Stack->Num() == 0)
	{
		return;
	}

	auto& [ReadLineHashSet] = ShidenSubsystem->ScenarioReadLines.FindOrAdd((*Stack)[0].ScenarioId);

	const FGuid ReadLineHash = GenerateReadLineHashFromScenarioProgressStack(*Stack);

	ReadLineHashSet.Add(ReadLineHash);
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::IsRead()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!IsScenarioPlaying())
	{
		return false;
	}

	TArray<FShidenScenarioProgress>* Stack = &ShidenSubsystem->ScenarioProgressStack["Default"].Stack;
	if (Stack->Num() == 0)
	{
		return false;
	}

	const FShidenReadLines* ReadLines = ShidenSubsystem->ScenarioReadLines.Find((*Stack)[0].ScenarioId);
	if (!ReadLines)
	{
		return false;
	}

	const FGuid ReadLineHash = GenerateReadLineHashFromScenarioProgressStack(*Stack);

	return ReadLines->ReadLineHashSet.Contains(ReadLineHash);
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::CanSkipCommand()
{
	if (!IsScenarioPlaying())
	{
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	return ShidenSubsystem->PredefinedSystemVariable.SkipCondition == EShidenSkipCondition::All
		|| (ShidenSubsystem->PredefinedSystemVariable.SkipCondition == EShidenSkipCondition::ReadOnly && IsRead());
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::IsEndOfScenario(const FString& ProcessName)
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::GetScenarioFromCache(const FGuid& ScenarioId, UShidenScenario*& Scenario, bool& bSuccess)
{
	Scenario = nullptr;
	bSuccess = false;

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (TObjectPtr<UShidenScenario> TempScenario = ShidenSubsystem->ScenarioCache.FindRef(ScenarioId))
	{
		Scenario = TempScenario;
		bSuccess = true;
		return;
	}

	TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString* ScenarioPath = ShidenProjectConfig->ScenarioPaths.Find(ScenarioId);
	if (!ScenarioPath)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Error: Scenario \"%s\" is not found in scenario paths.Please restart the editor."),
		       *ScenarioId.ToString(EGuidFormats::DigitsWithHyphensInBraces));
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::GetScenarioByIdOrObjectPath(const FString& ScenarioIdOrObjectPath, FGuid& ScenarioId,
                                                                                 UShidenScenario*& Scenario, bool& bSuccess)
{
	if (FGuid::Parse(ScenarioIdOrObjectPath, ScenarioId))
	{
		GetScenarioFromCache(ScenarioId, Scenario, bSuccess);
		return;
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	TArray<FGuid> ScenarioIds = TArray<FGuid>();
	for (const TTuple<FGuid, FString>& ScenarioPath : ProjectConfig->ScenarioPaths)
	{
		if (ScenarioPath.Value == ScenarioIdOrObjectPath)
		{
			GetScenarioFromCache(ScenarioPath.Key, Scenario, bSuccess);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Error: Scenario \"%s\" is not found in scenario paths."), *ScenarioIdOrObjectPath);
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::ConstructCommand(const FString& ProcessName, const FShidenCommand& OriginalCommand,
                                                                      FShidenCommand& Command)
{
	if (OriginalCommand.PresetName.IsEmpty())
	{
		Command = UShidenVariableBlueprintLibrary::ReplaceAllVariable(ProcessName, OriginalCommand);
		return;
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString OriginalCommandName = OriginalCommand.CommandName;
	const FString OriginalPresetName = OriginalCommand.PresetName;

	const FShidenPreset* Preset = ProjectConfig->Presets.Find(OriginalPresetName);
	if (!Preset || Preset->CommandName != OriginalCommandName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Preset \"%s\" is not found. Command Name: %s"), *OriginalPresetName, *OriginalCommandName);
		Command = UShidenVariableBlueprintLibrary::ReplaceAllVariable(ProcessName, OriginalCommand);
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

	Command = UShidenVariableBlueprintLibrary::ReplaceAllVariable(ProcessName, TempCommand);
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::ConstructCommandForLoad(const FShidenVariable& TempLocalVariables,
                                                                             const FShidenCommand& OriginalCommand, FShidenCommand& Command)
{
	if (OriginalCommand.PresetName.IsEmpty())
	{
		Command = UShidenVariableBlueprintLibrary::ReplaceVariablesForLoad(TempLocalVariables, OriginalCommand);
		return;
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString OriginalCommandName = OriginalCommand.CommandName;
	const FString OriginalPresetName = OriginalCommand.PresetName;

	const FShidenPreset* Preset = ProjectConfig->Presets.Find(OriginalPresetName);

	if (!Preset || Preset->CommandName != OriginalCommandName)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Preset \"%s\" is not found. Command Name: %s"), *OriginalPresetName, *OriginalCommandName);
		Command = UShidenVariableBlueprintLibrary::ReplaceVariablesForLoad(TempLocalVariables, OriginalCommand);
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

	Command = UShidenVariableBlueprintLibrary::ReplaceVariablesForLoad(TempLocalVariables, TempCommand);
}

SHIDENCORE_API void GetCommand(UObject* Outer, const FSoftObjectPath& CommandSoftObjectPath, UShidenCommandObject*& CommandObject, bool& bSuccess)
{
	CommandObject = nullptr;
	bSuccess = false;

	const TObjectPtr<UClass> Class = UShidenBlueprintLibrary::ConstructClassFromSoftObjectPath(CommandSoftObjectPath);
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

	bSuccess = true;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::GetCommandFromCache(UObject* Outer, const FString& ProcessName,
                                                                         const FSoftObjectPath& CommandSoftObjectPath,
                                                                         UShidenCommandObject*& Command, bool& bSuccess)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	const FString CommandCacheKey = ProcessName + TEXT("::") + CommandSoftObjectPath.GetAssetPathString();
	if (const TObjectPtr<UShidenCommandObject> CommandPtr = ShidenSubsystem->CommandCache.FindRef(CommandCacheKey))
	{
		Command = CommandPtr;
		bSuccess = true;
		return;
	}

	GetCommand(Outer, CommandSoftObjectPath, Command, bSuccess);

	if (bSuccess)
	{
		ShidenSubsystem->CommandCache.Add(CommandCacheKey, Command);
	}
}

FShidenVariable ExtractReadOnlyLocalVariable(const UShidenScenario* Scenario, const TMap<FString, FString>& CommandArgs)
{
	TArray<FShidenVariableDefinition> VariableDefinitions = Scenario->LocalVariableDefinitions;
	VariableDefinitions.Append(Scenario->MacroParameterDefinitions);
	FShidenVariable TempMacroVariable(VariableDefinitions);

	if (CommandArgs.Num() == 0)
	{
		return TempMacroVariable;
	}

	for (const FShidenVariableDefinition& VariableDefinition : Scenario->MacroParameterDefinitions)
	{
		if (VariableDefinition.bIsReadOnly)
		{
			switch (VariableDefinition.Type)
			{
			case EShidenVariableType::Boolean:
				{
					if (CommandArgs.Contains(VariableDefinition.Name))
					{
						TempMacroVariable.TryUpdate(VariableDefinition.Name, CommandArgs[VariableDefinition.Name].ToBool(), true);
					}
					break;
				}
			case EShidenVariableType::String:
			case EShidenVariableType::AssetPath:
				{
					if (CommandArgs.Contains(VariableDefinition.Name))
					{
						TempMacroVariable.TryUpdate(VariableDefinition.Name, CommandArgs[VariableDefinition.Name], true);
					}
					break;
				}
			case EShidenVariableType::Integer:
				{
					if (CommandArgs.Contains(VariableDefinition.Name))
					{
						const int32 IntValue = FCString::Atoi(*CommandArgs[VariableDefinition.Name]);
						TempMacroVariable.TryUpdate(VariableDefinition.Name, IntValue, true);
					}
					break;
				}
			case EShidenVariableType::Float:
				{
					if (CommandArgs.Contains(VariableDefinition.Name))
					{
						const float FloatValue = FCString::Atof(*CommandArgs[VariableDefinition.Name]);
						TempMacroVariable.TryUpdate(VariableDefinition.Name, FloatValue, true);
					}
					break;
				}
			case EShidenVariableType::Vector2:
				{
					if (CommandArgs.Contains(VariableDefinition.Name))
					{
						FVector2d Vector2Value;
						Vector2Value.InitFromString(CommandArgs[VariableDefinition.Name]);
						TempMacroVariable.TryUpdate(VariableDefinition.Name, Vector2Value, true);
					}
					break;
				}
			case EShidenVariableType::Vector3:
				{
					if (CommandArgs.Contains(VariableDefinition.Name))
					{
						FVector Vector3Value;
						Vector3Value.InitFromString(CommandArgs[VariableDefinition.Name]);
						TempMacroVariable.TryUpdate(VariableDefinition.Name, Vector3Value, true);
					}
					break;
				}
			}
		}
	}
	return TempMacroVariable;
}

void LoadScenarioAssetPathsInternal(UObject* CallerObject, const UShidenScenario* Scenario,
                                    const FShidenVariable& TempVariable, TArray<FShidenLoadingAssetInfo>& AssetInfo,
                                    bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex,
                                    TArray<FGuid>& LoadedScenarioIds)
{
	if (LoadedScenarioIds.Contains(Scenario->ScenarioId))
	{
		return;
	}

	if (!Scenario)
	{
		ErrorMessage = TEXT("Scenario is null");
		return;
	}

	LoadedScenarioIds.AddUnique(Scenario->ScenarioId);

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		UShidenBlueprintLibrary::InitCommandDefinitions();
	}

	for (int32 Index = 0; Index < Scenario->Commands.Num(); Index++)
	{
		const FShidenCommand& OriginalCommand = Scenario->Commands[Index];
		FShidenCommand Command = FShidenCommand();
		UShidenScenarioBlueprintLibrary::ConstructCommandForLoad(TempVariable, OriginalCommand, Command);

		if (!Command.bEnabled)
		{
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
			FGuid TempScenarioId;
			bool bTempSuccess = false;
			UShidenScenario* MacroScenario = nullptr;
			UShidenScenarioBlueprintLibrary::GetScenarioByIdOrObjectPath(Command.Args["MacroName"], TempScenarioId, MacroScenario, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found."), *Command.Args["MacroName"]);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			FShidenVariable TempLocalVariable = ExtractReadOnlyLocalVariable(MacroScenario, Command.Args);
			LoadScenarioAssetPathsInternal(CallerObject, MacroScenario, TempLocalVariable, AssetInfo, bTempSuccess, ErrorMessage, ErrorScenarioId,
			                               ErrorIndex, LoadedScenarioIds);
			if (!bTempSuccess)
			{
				return;
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
			FGuid TempScenarioId;
			bool bTempSuccess = false;
			UShidenScenario* MacroScenario = nullptr;
			UShidenScenarioBlueprintLibrary::GetScenarioByIdOrObjectPath(Command.Args["MacroName"], TempScenarioId, MacroScenario, bTempSuccess);
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(
					TEXT("RunMacro: Scenario \"%s\" is not found."), *Command.Args["MacroName"]);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			FShidenVariable TempLocalVariable = ExtractReadOnlyLocalVariable(MacroScenario, Command.Args);
			LoadScenarioAssetPathsInternal(CallerObject, MacroScenario, TempLocalVariable, AssetInfo, bTempSuccess, ErrorMessage, ErrorScenarioId,
			                               ErrorIndex, LoadedScenarioIds);
			if (!bTempSuccess)
			{
				return;
			}
		}
		else
		{
			bool bTempSuccess = false;
			FShidenCommandDefinition* Definition = ShidenSubsystem->CommandDefinitionCache.Find(Command.CommandName);
			if (Definition)
			{
				UShidenCommandObject* CommandObject = nullptr;
				GetCommand(CallerObject, Definition->CommandSoftObjectPath, CommandObject, bTempSuccess);
			}
			if (!bTempSuccess)
			{
				ErrorMessage = FString::Printf(
					TEXT("Get Command \"%s\" Failed.Please check project config and command definitions."), *Command.CommandName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return;
			}
			TSet<FString> TempAssetPaths;
			for (const FShidenCommandArgument& Argument : Definition->Args)
			{
				if (Argument.bIsAssetToBeLoaded)
				{
					TempAssetPaths.Add(Command.Args.FindRef(Argument.ArgName.ToString()));
				}
			}
			TempAssetPaths.Remove(TEXT(""));
			TempAssetPaths.Remove(TEXT("None"));
			for (const FString& AssetPath : TempAssetPaths)
			{
				AssetInfo.AddUnique(FShidenLoadingAssetInfo{Index, AssetPath});
			}
		}
	}
	bSuccess = true;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::LoadScenarioAssetPaths(UObject* CallerObject, const UShidenScenario* Scenario,
                                                                            TArray<FShidenLoadingAssetInfo>& AssetInfo, bool& bSuccess,
                                                                            FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex)
{
	TArray<FGuid> LoadedScenarioIds;
	AssetInfo.Empty();
	bSuccess = false;
	ErrorMessage = TEXT("");
	ErrorScenarioId = FGuid();
	ErrorIndex = -1;
	const FShidenVariable TempLocalVariable = ExtractReadOnlyLocalVariable(Scenario, TMap<FString, FString>());
	LoadScenarioAssetPathsInternal(CallerObject, Scenario, TempLocalVariable, AssetInfo, bSuccess, ErrorMessage, ErrorScenarioId, ErrorIndex,
	                               LoadedScenarioIds);
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::LoadScenarioAssets(UObject* Outer, bool& bSuccess, FString& ErrorMessage, FGuid& ErrorScenarioId,
                                                                        int32& ErrorIndex)
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::InitFromSaveData(UShidenWidget* ShidenWidget,
                                                                      const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                      UObject* CallerObject, bool& bSuccess, FString& ErrorMessage)
{
	bSuccess = false;
	ErrorMessage = TEXT("");

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (ShidenSubsystem->CommandDefinitionCache.Num() == 0)
	{
		UShidenBlueprintLibrary::InitCommandDefinitions();
	}

	for (const auto& [CommandName, ScenarioProperties] : ShidenSubsystem->ScenarioProperties)
	{
		const FSoftObjectPath CommandPath = ShidenSubsystem->CommandDefinitionCache.FindRef(CommandName).CommandSoftObjectPath;
		UShidenCommandObject* CommandObject = nullptr;
		bool bTempSuccess = false;
		GetCommandFromCache(CallerObject, TEXT("Default"), CommandPath, CommandObject, bTempSuccess);
		if (!bTempSuccess)
		{
			ErrorMessage = FString::Printf(
				TEXT("Get Command \"%s\" Failed. Please check project config and command definitions."), *CommandName);
			return;
		}
		EShidenInitFromSaveDataStatus Status = EShidenInitFromSaveDataStatus::Complete;
		CommandObject->RestoreFromSaveData(ScenarioProperties.ScenarioProperties, ShidenWidget, ShidenManager, CallerObject, Status, ErrorMessage);
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

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(const FString& CommandName, const FString& Key, const FString& Value)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	if (!CurrentScenarioProperties.Contains(CommandName))
	{
		FShidenScenarioProperties Properties;
		Properties.ScenarioProperties = TMap<FString, FString>();
		Properties.ScenarioProperties.Add(Key, Value);
		CurrentScenarioProperties.Add(CommandName, Properties);
	}
	else
	{
		CurrentScenarioProperties[CommandName].ScenarioProperties.Add(Key, Value);
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(const FString& CommandName, const FString& Key)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	if (!ShidenSubsystem->ScenarioProperties.Contains(CommandName))
	{
		return;
	}

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	if (CurrentScenarioProperties.Contains(CommandName))
	{
		CurrentScenarioProperties[CommandName].ScenarioProperties.Remove(Key);
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::ClearAllScenarioProperties()
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->ScenarioProperties.Empty();
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::ClearScenarioProperties(const FString& CommandName)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();

	check(ShidenSubsystem);

	ShidenSubsystem->ScenarioProperties.Remove(CommandName);
}
