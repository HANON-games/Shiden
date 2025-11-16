// Copyright (c) 2025 HANON. All Rights Reserved.

#include "Scenario/ShidenScenarioBlueprintLibrary.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/AssetManager.h"
#include "System/ShidenLoadingAssetInfo.h"

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryConvertToStringArray(const FShidenScenarioProperty& Property, TArray<FString>& Result)
{
	return Property.TryConvertToStringArray(Result);
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryConvertToStringMap(const FShidenScenarioProperty& Property, TMap<FString, FString>& Result)
{
	return Property.TryConvertToStringMap(Result);
}

SHIDENCORE_API FString UShidenScenarioBlueprintLibrary::GetValueAsString(const FShidenScenarioProperty& Property)
{
	return Property.GetValueAsString();
}

bool UShidenScenarioBlueprintLibrary::IsScenarioPlaying()
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

	FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack)
	{
		return;
	}

	UShidenVariableBlueprintLibrary::RemoveLocalVariablesInCurrentScope(ProcessName);

	if (ProgressStack->IsEmpty())
	{
		bIsLastElement = true;
		return;
	}

	ScenarioProgress = ProgressStack->Stack.Pop();
	bIsLastElement = ProgressStack->IsEmpty();
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

	FShidenScenarioProgressStack& ProgressStack = ShidenSubsystem->ScenarioProgressStack.FindOrAdd(ProcessName);
	ProgressStack.Stack.Push(FShidenScenarioProgress{Scenario->ScenarioId});
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

	if (FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName))
	{
		ProgressStack->UpdateCurrentScenarioIndex(CurrentIndex);
	}
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryPeekScenario(const FString& ProcessName, FShidenScenarioProgress& ScenarioProgress,
                                                                     FShidenCancelInfo& CancelInfo, bool& bIsLastElement, bool& bIsMacro)
{
	ScenarioProgress = FShidenScenarioProgress();
	CancelInfo = FShidenCancelInfo();
	bIsLastElement = false;
	bIsMacro = false;

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName);
	if (!ProgressStack || ProgressStack->IsEmpty())
	{
		return false;
	}

	ScenarioProgress = ProgressStack->Stack.Last();
	CancelInfo = ProgressStack->CancelInfo;
	bIsLastElement = ProgressStack->Stack.Num() == 1;
	bIsMacro = !bIsLastElement || ProcessName != TEXT("Default");
	return true;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::ToNext(const FString& ProcessName, int32& NextIndex)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	NextIndex = 0;

	if (FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName))
	{
		NextIndex = ++ProgressStack->Stack.Last().CurrentIndex;
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RequestCancelScenario(const FString& ProcessName, const FString& CancelReason,
                                                                           const EShidenCancelType CancelType)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName))
	{
		if (!ProgressStack->CancelInfo.bIsCancelRequested)
		{
			ProgressStack->CancelInfo.bIsCancelRequested = true;
			ProgressStack->CancelInfo.CancelReason = CancelReason;
			ProgressStack->CancelInfo.CancelType = CancelType;
		}
	}
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RequestCancelScenarioAll(const FString& CancelReason, const EShidenCancelType CancelType)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	for (TTuple<FString, FShidenScenarioProgressStack>& Progress : ShidenSubsystem->ScenarioProgressStack)
	{
		if (!Progress.Value.CancelInfo.bIsCancelRequested)
		{
			Progress.Value.CancelInfo.bIsCancelRequested = true;
			Progress.Value.CancelInfo.CancelReason = CancelReason;
			Progress.Value.CancelInfo.CancelType = CancelType;
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

	if (FShidenScenarioProgressStack* ProgressStack = ShidenSubsystem->ScenarioProgressStack.Find(ProcessName))
	{
		bIsCancelRequested = ProgressStack->CancelInfo.bIsCancelRequested;
		CancelReason = ProgressStack->CancelInfo.CancelReason;
		CancelType = ProgressStack->CancelInfo.CancelType;
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
	for (const auto& [ScenarioId, CurrentIndex] : Stack)
	{
		if (UShidenScenarioBlueprintLibrary::TryGetScenario(ScenarioId, Scenario) && Scenario->Commands.IsValidIndex(CurrentIndex))
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

	return ReadLines->ReadLineHashes.Contains(ReadLineHash);
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

	if (!TryPeekScenario(ProcessName, ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro))
	{
		return true;
	}

	UShidenScenario* Scenario = nullptr;
	if (!TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
	{
		// If the scenario is not found, we consider it as end of scenario
		return true;
	}

	return Scenario->Commands.Num() <= ScenarioProgress.CurrentIndex;
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryGetScenario(const FGuid& ScenarioId, UShidenScenario*& Scenario)
{
	Scenario = nullptr;

	TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	if (const TObjectPtr<UShidenScenario> TempScenario = ShidenSubsystem->ScenarioCache.FindRef(ScenarioId))
	{
		Scenario = TempScenario;
		return true;
	}

	const TObjectPtr<const UShidenProjectConfig> ShidenProjectConfig = GetDefault<UShidenProjectConfig>();

	const FString* ScenarioPath = ShidenProjectConfig->ScenarioPaths.Find(ScenarioId);
	if (!ScenarioPath)
	{
		UE_LOG(LogTemp, Warning,
		       TEXT("Error: Scenario \"%s\" is not found in scenario paths.Please restart the editor."),
		       *ScenarioId.ToString(EGuidFormats::DigitsWithHyphensInBraces));
		return false;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FSoftObjectPath(*ScenarioPath));

	if (!AssetData.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), **ScenarioPath);
		return false;
	}

	FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
	TObjectPtr<UObject> LoadedAsset = Streamable.LoadSynchronous(AssetData.ToSoftObjectPath(), false);
	if (!LoadedAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), **ScenarioPath);
		return false;
	}

	Scenario = Cast<UShidenScenario>(LoadedAsset);
	if (!Scenario)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error: Loading scenario \"%s\" failed."), **ScenarioPath);
		return false;
	}

	ShidenSubsystem->ScenarioCache.Add(ScenarioId, Scenario);
	return true;
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(const FString& ScenarioIdOrObjectPath, FGuid& ScenarioId, UShidenScenario*& Scenario)
{
	if (FGuid::Parse(ScenarioIdOrObjectPath, ScenarioId))
	{
		return TryGetScenario(ScenarioId, Scenario);
	}

	const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
	TArray<FGuid> ScenarioIds = TArray<FGuid>();
	for (const TTuple<FGuid, FString>& ScenarioPath : ProjectConfig->ScenarioPaths)
	{
		if (ScenarioPath.Value == ScenarioIdOrObjectPath)
		{
			return TryGetScenario(ScenarioPath.Key, Scenario);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Error: Scenario \"%s\" is not found in scenario paths."), *ScenarioIdOrObjectPath);
	return false;
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

SHIDENCORE_API bool TryGetCommandInternal(UObject* Outer, const FSoftObjectPath& CommandSoftObjectPath, UShidenCommandObject*& CommandObject)
{
	CommandObject = nullptr;

	const TObjectPtr<UClass> Class = UShidenBlueprintLibrary::ConstructClassFromSoftObjectPath(CommandSoftObjectPath);
	if (!Class)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load class from ObjectPath: %s"), *CommandSoftObjectPath.GetAssetPathString());
		return false;
	}

	CommandObject = NewObject<UShidenCommandObject>(Outer, Class);
	if (!CommandObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create UShidenCommandObject from ObjectPath: %s"), *CommandSoftObjectPath.GetAssetPathString());
		return false;
	}

	return true;
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryGetCommand(UObject* Outer, const FString& ProcessName, const FSoftObjectPath& CommandSoftObjectPath, UShidenCommandObject*& Command)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	const FString CommandCacheKey = ProcessName + TEXT("::") + CommandSoftObjectPath.GetAssetPathString();
	if (const TObjectPtr<UShidenCommandObject> CommandPtr = ShidenSubsystem->CommandCache.FindRef(CommandCacheKey))
	{
		Command = CommandPtr;
		return true;
	}

	if (!TryGetCommandInternal(Outer, CommandSoftObjectPath, Command))
	{
		return false;
	}

	ShidenSubsystem->CommandCache.Add(CommandCacheKey, Command);
	return true;
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
					if (const FString* ArgValue = CommandArgs.Find(VariableDefinition.Name))
					{
						TempMacroVariable.TryUpdate(VariableDefinition.Name, ArgValue->ToBool(), true);
					}
					break;
				}
			case EShidenVariableType::String:
			case EShidenVariableType::AssetPath:
				{
					if (const FString* ArgValue = CommandArgs.Find(VariableDefinition.Name))
					{
						TempMacroVariable.TryUpdate(VariableDefinition.Name, *ArgValue, true);
					}
					break;
				}
			case EShidenVariableType::Integer:
				{
					if (const FString* ArgValue = CommandArgs.Find(VariableDefinition.Name))
					{
						const int32 IntValue = FCString::Atoi(**ArgValue);
						TempMacroVariable.TryUpdate(VariableDefinition.Name, IntValue, true);
					}
					break;
				}
			case EShidenVariableType::Float:
				{
					if (const FString* ArgValue = CommandArgs.Find(VariableDefinition.Name))
					{
						const float FloatValue = FCString::Atof(**ArgValue);
						TempMacroVariable.TryUpdate(VariableDefinition.Name, FloatValue, true);
					}
					break;
				}
			case EShidenVariableType::Vector2:
				{
					if (const FString* ArgValue = CommandArgs.Find(VariableDefinition.Name))
					{
						FVector2D Vector2Value;
						if (!Vector2Value.InitFromString(*ArgValue))
						{
							UE_LOG(LogTemp, Warning, TEXT("Failed to convert %s to FVector2D."), *CommandArgs[VariableDefinition.Name]);
						}
						TempMacroVariable.TryUpdate(VariableDefinition.Name, Vector2Value, true);
					}
					break;
				}
			case EShidenVariableType::Vector3:
				{
					if (const FString* ArgValue = CommandArgs.Find(VariableDefinition.Name))
					{
						FVector Vector3Value;
						if (!Vector3Value.InitFromString(*ArgValue))
						{
							UE_LOG(LogTemp, Warning, TEXT("Failed to convert %s to FVector."), *CommandArgs[VariableDefinition.Name]);
						}
						TempMacroVariable.TryUpdate(VariableDefinition.Name, Vector3Value, true);
					}
					break;
				}
			}
		}
	}
	return TempMacroVariable;
}

bool TryLoadScenarioAssetPathsInternal(UObject* CallerObject, const UShidenScenario* Scenario, const FShidenVariable& TempVariable,
                                       TArray<FShidenLoadingAssetInfo>& AssetInfo, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex,
                                       TArray<FGuid>& LoadedScenarioIds)
{
	if (!Scenario)
	{
		ErrorMessage = TEXT("Scenario is null");
		return false;
	}

	if (LoadedScenarioIds.Contains(Scenario->ScenarioId))
	{
		return true;
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
			const FString* MacroName = Command.Args.Find("MacroName");
			if (!MacroName || MacroName->IsEmpty())
			{
				ErrorMessage = TEXT("RunMacro: MacroName is empty.");
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
			}
			FGuid TempScenarioId;
			UShidenScenario* MacroScenario = nullptr;
			if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(*MacroName, TempScenarioId, MacroScenario))
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found."), **MacroName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
			}
			FShidenVariable TempLocalVariable = ExtractReadOnlyLocalVariable(MacroScenario, Command.Args);
			if (!TryLoadScenarioAssetPathsInternal(CallerObject, MacroScenario, TempLocalVariable, AssetInfo, ErrorMessage, ErrorScenarioId,
			                                       ErrorIndex, LoadedScenarioIds))
			{
				return false;
			}
		}
		else if (Command.CommandName == TEXT("RunMacroAsParallel"))
		{
			const FString* NewProcessName = Command.Args.Find("NewProcessName");
			if (!NewProcessName || NewProcessName->IsEmpty())
			{
				ErrorMessage = TEXT("RunMacroAsParallel: NewProcessName is empty.");
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
			}
			const FString* MacroName = Command.Args.Find("MacroName");
			if (!MacroName || MacroName->IsEmpty())
			{
				ErrorMessage = TEXT("RunMacroAsParallel: MacroName is empty.");
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
			}
			FGuid TempScenarioId;
			UShidenScenario* MacroScenario = nullptr;
			if (!UShidenScenarioBlueprintLibrary::TryGetScenarioByIdOrObjectPath(*MacroName, TempScenarioId, MacroScenario))
			{
				ErrorMessage = FString::Printf(TEXT("RunMacro: Scenario \"%s\" is not found."), **MacroName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
			}
			FShidenVariable TempLocalVariable = ExtractReadOnlyLocalVariable(MacroScenario, Command.Args);
			if (!TryLoadScenarioAssetPathsInternal(CallerObject, MacroScenario, TempLocalVariable, AssetInfo, ErrorMessage, ErrorScenarioId,
			                                       ErrorIndex, LoadedScenarioIds))
			{
				return false;
			}
		}
		else
		{
			FShidenCommandDefinition* Definition = ShidenSubsystem->CommandDefinitionCache.Find(Command.CommandName);
			if (!Definition)
			{
				ErrorMessage = FString::Printf(TEXT("Command \"%s\" is not defined in command definitions."), *Command.CommandName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
			}
			UShidenCommandObject* CommandObject;
			if (!TryGetCommandInternal(CallerObject, Definition->CommandSoftObjectPath, CommandObject))
			{
				ErrorMessage = FString::Printf(
					TEXT("Get Command \"%s\" Failed.Please check project config and command definitions."), *Command.CommandName);
				ErrorScenarioId = Scenario->ScenarioId;
				ErrorIndex = Index;
				return false;
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
	return true;
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryLoadScenarioAssetPaths(UObject* CallerObject, const UShidenScenario* Scenario,
                                                                               TArray<FShidenLoadingAssetInfo>& AssetInfo, FString& ErrorMessage,
                                                                               FGuid& ErrorScenarioId, int32& ErrorIndex)
{
	TArray<FGuid> LoadedScenarioIds;
	AssetInfo.Empty();
	ErrorMessage = TEXT("");
	ErrorScenarioId = FGuid();
	ErrorIndex = INDEX_NONE;
	const FShidenVariable TempLocalVariable = ExtractReadOnlyLocalVariable(Scenario, TMap<FString, FString>());
	return TryLoadScenarioAssetPathsInternal(CallerObject, Scenario, TempLocalVariable, AssetInfo, ErrorMessage, ErrorScenarioId, ErrorIndex,
	                                         LoadedScenarioIds);
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryLoadScenarioAssets(UObject* Outer, FString& ErrorMessage, FGuid& ErrorScenarioId, int32& ErrorIndex)
{
	ErrorMessage = TEXT("");
	ErrorScenarioId = FGuid();
	ErrorIndex = INDEX_NONE;

	FShidenScenarioProgress ScenarioProgress = FShidenScenarioProgress();
	FShidenCancelInfo CancelInfo = FShidenCancelInfo();
	bool bIsLastElement = false;
	bool bIsMacro = false;

	if (!TryPeekScenario(TEXT("Default"), ScenarioProgress, CancelInfo, bIsLastElement, bIsMacro))
	{
		ErrorMessage = TEXT("Load scenario failed.");
		ErrorIndex = INDEX_NONE;
		return false;
	}

	UShidenScenario* Scenario = nullptr;
	if (!TryGetScenario(ScenarioProgress.ScenarioId, Scenario))
	{
		const TObjectPtr<const UShidenProjectConfig> ProjectConfig = GetDefault<UShidenProjectConfig>();
		const FString* ScenarioPath = ProjectConfig->ScenarioPaths.Find(ScenarioProgress.ScenarioId);
		ErrorMessage = FString::Printf(TEXT("Load scenario failed: %s"), ScenarioPath ? **ScenarioPath : TEXT("Unknown"));
		ErrorScenarioId = ScenarioProgress.ScenarioId;
		ErrorIndex = INDEX_NONE;
		return false;
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	ShidenSubsystem->AssetCache.Empty();

	TArray<FShidenLoadingAssetInfo> AssetInfo = TArray<FShidenLoadingAssetInfo>();
	FString TempErrorMessage = TEXT("");
	FGuid TempErrorScenarioId = FGuid();
	int32 TempErrorIndex = INDEX_NONE;
	if (!TryLoadScenarioAssetPaths(Outer, Scenario, AssetInfo, TempErrorMessage, TempErrorScenarioId, TempErrorIndex))
	{
		ErrorMessage = TempErrorMessage;
		ErrorScenarioId = TempErrorScenarioId;
		ErrorIndex = TempErrorIndex;
		return false;
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
			return false;
		}

		ShidenSubsystem->AssetCache.Add(ObjectPath, LoadedAsset);
	}

	return true;
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryInitFromSaveData(UShidenWidget* ShidenWidget,
                                                                         const TScriptInterface<IShidenManagerInterface>& ShidenManager,
                                                                         UObject* CallerObject, FString& ErrorMessage)
{
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
		if (!TryGetCommand(CallerObject, TEXT("Default"), CommandPath, CommandObject))
		{
			ErrorMessage = FString::Printf(
				TEXT("Get Command \"%s\" Failed. Please check project config and command definitions."), *CommandName);
			return false;
		}
		EShidenInitFromSaveDataStatus Status = EShidenInitFromSaveDataStatus::Complete;
		CommandObject->RestoreFromSaveData(ScenarioProperties.ScenarioProperties, ShidenWidget, ShidenManager, CallerObject, Status, ErrorMessage);
		if (Status == EShidenInitFromSaveDataStatus::Error)
		{
			return false;
		}
	}

	return true;
}

SHIDENCORE_API void MakeObjectPathFromPackagePath(const FString& PackagePath, FName& ObjectPath)
{
	FString Right;
	PackagePath.Split(TEXT("."), nullptr, &Right);
	const FString ObjectPathString = PackagePath + TEXT(".") + Right;
	ObjectPath = FName(*ObjectPathString);
}

SHIDENCORE_API bool UShidenScenarioBlueprintLibrary::TryFindScenarioProperty(const FString& CommandName, const FString& Key, FShidenScenarioProperty& Property)
{
	if (!IsScenarioPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterScenarioProperty: Scenario is not playing."));
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	const FShidenScenarioProperties* ScenarioProps = CurrentScenarioProperties.Find(CommandName);
	if (!ScenarioProps)
	{
		return false;
	}

	const FShidenScenarioProperty* Temp = ScenarioProps->ScenarioProperties.Find(Key);
	if (!Temp)
	{
		return false;
	}

	Property = *Temp;
	return true;
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RegisterScenarioProperty(const FString& CommandName, const FString& Key, const FString& Value)
{
	if (!IsScenarioPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterScenarioProperty: Scenario is not playing."));
	}

	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	FShidenScenarioProperties& Properties = CurrentScenarioProperties.FindOrAdd(CommandName);
	Properties.ScenarioProperties.Add(Key, FShidenScenarioProperty(Value));
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromArray(const FString& CommandName, const FString& Key, const TArray<FString>& Values)
{
	if (!IsScenarioPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterScenarioProperty: Scenario is not playing."));
	}

	// Convert the array of strings to a JSON string
	FString JsonString;
	TArray<TSharedPtr<FJsonValue>> JsonValues;
	for (const FString& Value : Values)
	{
		JsonValues.Add(MakeShareable(new FJsonValueString(Value)));
	}
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString, 0);
	if (FJsonSerializer::Serialize(JsonValues, Writer))
	{
		JsonString.ReplaceInline(TEXT("[\r\n"), TEXT("["), ESearchCase::CaseSensitive);
		JsonString.ReplaceInline(TEXT("\r\n]"), TEXT("]"), ESearchCase::CaseSensitive);
		JsonString.ReplaceInline(TEXT("\r\n"), TEXT(" "), ESearchCase::CaseSensitive);
		JsonString.ReplaceInline(TEXT("\t"), TEXT(""), ESearchCase::CaseSensitive);
	}
	RegisterScenarioProperty(CommandName, Key, JsonString);
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RegisterScenarioPropertyFromMap(const FString& CommandName, const FString& Key, const TMap<FString, FString>& Values)
{
	if (!IsScenarioPlaying())
	{
		UE_LOG(LogTemp, Warning, TEXT("RegisterScenarioProperty: Scenario is not playing."));
	}

	// Convert the map of strings to a JSON string
	FString JsonString;
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	for (const TPair<FString, FString>& Pair : Values)
	{
		JsonObject->SetStringField(Pair.Key, Pair.Value);
	}
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
	if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer))
	{
		JsonString.ReplaceInline(TEXT("{\r\n"), TEXT("{"), ESearchCase::CaseSensitive);
		JsonString.ReplaceInline(TEXT("\r\n}"), TEXT("}"), ESearchCase::CaseSensitive);
		JsonString.ReplaceInline(TEXT("\r\n"), TEXT(" "), ESearchCase::CaseSensitive);
		JsonString.ReplaceInline(TEXT("\t"), TEXT(""), ESearchCase::CaseSensitive);
	}
	RegisterScenarioProperty(CommandName, Key, JsonString);
}

SHIDENCORE_API void UShidenScenarioBlueprintLibrary::RemoveScenarioProperty(const FString& CommandName, const FString& Key)
{
	const TObjectPtr<UShidenSubsystem> ShidenSubsystem = GEngine->GetEngineSubsystem<UShidenSubsystem>();
	check(ShidenSubsystem);

	TMap<FString, FShidenScenarioProperties>& CurrentScenarioProperties = ShidenSubsystem->ScenarioProperties;

	if (FShidenScenarioProperties* ScenarioProps = CurrentScenarioProperties.Find(CommandName))
	{
		ScenarioProps->ScenarioProperties.Remove(Key);
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
