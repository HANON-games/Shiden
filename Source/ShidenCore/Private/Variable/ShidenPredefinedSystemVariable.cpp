// Copyright (c) 2026 HANON. All Rights Reserved.

#include "Variable/ShidenPredefinedSystemVariable.h"
#include "Config/ShidenProjectConfig.h"
#include "Variable/ShidenVariableDescriptor.h"

SHIDENCORE_API bool FShidenPredefinedSystemVariable::TryGetDefinition(const FString& Name,
                                                                      FShidenPredefinedSystemVariableDefinition& Definition) const
{
	if (const FShidenPredefinedSystemVariableDefinition* Temp = Definitions.FindByKey(Name))
	{
		Definition = *Temp;
		return true;
	}
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
SHIDENCORE_API bool FShidenPredefinedSystemVariable::TryUpdateByString(const FString& Name, const FString& Value,
                                                                       const bool bForceUpdateReadOnly /*= false*/)
{
	const FShidenPredefinedSystemVariableDefinition* Definition = Definitions.FindByKey(Name);
	if (Definition && (bForceUpdateReadOnly || !Definition->bIsReadOnly))
	{
		Definition->SetVariable(Value);
		return true;
	}
	return false;
}

SHIDENCORE_API bool FShidenPredefinedSystemVariable::Contains(const FString& Name) const
{
	return Definitions.FindByKey(Name) != nullptr;
}

SHIDENCORE_API bool FShidenPredefinedSystemVariable::TryGetAsString(const FString& Name, EShidenVariableType& Type, FString& Value) const
{
	if (const FShidenPredefinedSystemVariableDefinition* Definition = Definitions.FindByKey(Name))
	{
		Type = Definition->Type;
		Value = Definition->GetVariable();
		return true;
	}
	return false;
}

// ReSharper disable once CppMemberFunctionMayBeConst
SHIDENCORE_API void FShidenPredefinedSystemVariable::ResetAll()
{
	for (const FShidenPredefinedSystemVariableDefinition& Definition : Definitions)
	{
		Definition.SetVariable(Definition.DefaultValue);
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
SHIDENCORE_API bool FShidenPredefinedSystemVariable::TryReset(const FString& Name)
{
	if (const FShidenPredefinedSystemVariableDefinition* Definition = Definitions.FindByKey(Name))
	{
		Definition->SetVariable(Definition->DefaultValue);
		return true;
	}
	return false;
}

SHIDENCORE_API void FShidenPredefinedSystemVariable::GetNames(TArray<FString>& OutNames) const
{
	OutNames.Empty();
	for (const FShidenPredefinedSystemVariableDefinition& Definition : Definitions)
	{
		OutNames.Add(Definition.Name);
	}
}

SHIDENCORE_API int32 FShidenPredefinedSystemVariable::Num() const
{
	return Definitions.Num();
}

SHIDENCORE_API void FShidenPredefinedSystemVariable::ListDescriptors(TArray<FShidenVariableDescriptor>& VariableDescriptors) const
{
	VariableDescriptors.Empty();
	for (const FShidenPredefinedSystemVariableDefinition& Definition : Definitions)
	{
		VariableDescriptors.Add(FShidenVariableDescriptor(Definition.Name, Definition.Type, Definition.AssetPathType, Definition.GetVariable(),
		                                                  Definition.DefaultValue, Definition.bIsReadOnly));
	}
}
