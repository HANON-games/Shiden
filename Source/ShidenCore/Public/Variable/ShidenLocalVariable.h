// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariable.h"
#include "ShidenVariableDescriptor.h"
#include "ShidenVariableType.h"
#include "Scenario/ShidenScenario.h"
#include "ShidenLocalVariable.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenLocalVariable
{
	GENERATED_BODY()

	bool TryGetDefinition(const FString& ScopeKey, const FString& Name, FShidenVariableDefinition& Definition);

	void InitLocalVariable(const FString& ScopeKey, const UShidenScenario* Scenario);

	bool TryUpdate(const FString& ScopeKey, const FString& Name, bool Value,
	               bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& ScopeKey, const FString& Name, int32 Value,
	               bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& ScopeKey, const FString& Name, float Value,
	               bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& ScopeKey, const FString& Name, const FString& Value,
	               bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& ScopeKey, const FString& Name, const FVector& Value,
	               bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& ScopeKey, const FString& Name, const FVector2D& Value,
	               bool bForceUpdateReadOnly = false);

	bool Contains(const FString& ScopeKey, const FString& Name) const;

	bool TryGet(const FString& ScopeKey, const FString& Name, bool& Value);

	bool TryGet(const FString& ScopeKey, const FString& Name, int32& Value);

	bool TryGet(const FString& ScopeKey, const FString& Name, float& Value);

	bool TryGet(const FString& ScopeKey, const FString& Name, FString& Value);

	bool TryGet(const FString& ScopeKey, const FString& Name, FVector& Value);

	bool TryGet(const FString& ScopeKey, const FString& Name, FVector2D& Value);

	bool TryGetAsString(const FString& ScopeKey, const FString& Name, EShidenVariableType& Type, FString& Value);

	void ResetAll();

	int32 Num() const;

	int32 Num(const FString& ScopeKey) const;

	bool TryReset(const FString& ScopeKey, const FString& Name);

	void RemoveVariablesInCurrentScope(const FString& ScopeKey);

	void RemoveVariablesInProcess(const FString& ProcessName);

	void ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	void UpdateVariableDefinitions();

	FShidenLocalVariable()
	{
		Variables = TMap<FString, FShidenVariable>();
	}

private:
	UPROPERTY()
	TMap<FString, FShidenVariable> Variables;

	UPROPERTY()
	TMap<FString, FGuid> ScenarioIds;

	bool IsValidScope(const FString& ScopeKey) const;
};
