// Copyright (c) 2024 HANON. All Rights Reserved.

#include "ShidenScenarioFunctionLibrary.h"

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
