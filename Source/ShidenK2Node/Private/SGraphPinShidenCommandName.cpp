// Copyright (c) 2025 HANON. All Rights Reserved.

#include "SGraphPinShidenCommandName.h"
#include "Containers/Array.h"
#include "Command/ShidenCommandDefinitions.h"
#include "HAL/PlatformCrt.h"
#include "Templates/SharedPointer.h"
#include "UObject/NameTypes.h"

// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
class UEdGraphPin;

void SGraphPinShidenCommandName::Construct(const FArguments& _, UEdGraphPin* InGraphPinObj,
                                           const TObjectPtr<UShidenCommandDefinitions> InCommandDefinitions)
{
	RefreshNameList(InCommandDefinitions);
	SGraphPinNameList::Construct(SGraphPinNameList::FArguments(), InGraphPinObj, NameList);
}

SGraphPinShidenCommandName::SGraphPinShidenCommandName()
{
}

SGraphPinShidenCommandName::~SGraphPinShidenCommandName()
{
}

void SGraphPinShidenCommandName::RefreshNameList(const TObjectPtr<UShidenCommandDefinitions>& CommandDefinitions)
{
	NameList.Empty();
	if (IsValid(CommandDefinitions))
	{
		TArray<FString> Keys;
		CommandDefinitions->CommandDefinitions.GetKeys(Keys);
		for (FString& Key : Keys)
		{
			TSharedPtr<FName> RowNameItem = MakeShareable(new FName(Key));
			NameList.Add(RowNameItem);
		}
	}
}
