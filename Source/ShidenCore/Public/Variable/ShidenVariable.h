// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "ShidenVariableDefinition.h"
#include "ShidenVariableDescriptor.h"
#include "ShidenVariableType.h"
#include "ShidenVariable.generated.h"

USTRUCT(BlueprintType)
struct SHIDENCORE_API FShidenVariable
{
	GENERATED_BODY()

	bool TryGetDefinition(const FString& Name, FShidenVariableDefinition& Definition);

	bool TryUpdate(const FString& Name, bool Value, bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& Name, const FString& Value, bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& Name, int32 Value, bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& Name, float Value, bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& Name, const FVector2D& Value, bool bForceUpdateReadOnly = false);

	bool TryUpdate(const FString& Name, const FVector& Value, bool bForceUpdateReadOnly = false);

	bool Contains(const FString& Name) const;

	bool TryGet(const FString& Name, bool& Value);

	bool TryGet(const FString& Name, int32& Value);

	bool TryGet(const FString& Name, float& Value);

	bool TryGet(const FString& Name, FString& Value);

	bool TryGet(const FString& Name, FVector& Value);

	bool TryGet(const FString& Name, FVector2d& Value);

	bool TryGetAsString(const FString& Name, EShidenVariableType& Type, FString& Value);

	void ResetAll();

	bool TryReset(const FString& Name);

	void GetNames(TArray<FString>& OutNames) const;

	int32 Num() const;

	void ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors);

	void UpdateVariableDefinitions(const TArray<FShidenVariableDefinition>& Definitions);

	FShidenVariable()
	{
	}

	explicit FShidenVariable(const TArray<FShidenVariableDefinition>& Definitions)
	{
		VariableDefinitions = Definitions;
		BooleanVariables = TMap<FString, bool>();
		IntegerVariables = TMap<FString, int>();
		FloatVariables = TMap<FString, float>();
		StringVariables = TMap<FString, FString>();
		Vector3Variables = TMap<FString, FVector>();
		Vector2Variables = TMap<FString, FVector2d>();
	}

private:
	bool CanUpdate(const FString& Name, const EShidenVariableType& Type, bool bForceUpdateReadOnly);

	static bool CanGet(const FShidenVariableDefinition* Definition, const FString& Name, const EShidenVariableType& Type);

	UPROPERTY()
	TMap<FString, bool> BooleanVariables;

	UPROPERTY()
	TMap<FString, int> IntegerVariables;

	UPROPERTY()
	TMap<FString, float> FloatVariables;

	UPROPERTY()
	TMap<FString, FString> StringVariables;

	UPROPERTY()
	TMap<FString, FVector> Vector3Variables;

	UPROPERTY()
	TMap<FString, FVector2D> Vector2Variables;

	UPROPERTY()
	TArray<FShidenVariableDefinition> VariableDefinitions;
};
