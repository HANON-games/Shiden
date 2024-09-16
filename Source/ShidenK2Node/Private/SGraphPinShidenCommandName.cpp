// Copyright (c) 2024 HANON. All Rights Reserved.

#include "SGraphPinShidenCommandName.h"

#include "Containers/Array.h"
#include "ShidenCommandDefinitions.h"
#include "HAL/PlatformCrt.h"
#include "Templates/SharedPointer.h"
#include "UObject/NameTypes.h"

class UEdGraphPin;

void SGraphPinShidenCommandName::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj, class UShidenCommandDefinitions* InCommandDefinitions)
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

void SGraphPinShidenCommandName::RefreshNameList(TSoftObjectPtr<UShidenCommandDefinitions> CommandDefinitions)
{
	NameList.Empty();
	if (CommandDefinitions.IsValid())
	{
		TArray<FString> Keys = TArray<FString>();
		CommandDefinitions->CommandDefinitions.GetKeys(Keys);
		for (FString Name : Keys)
		{
			TSharedPtr<FName> RowNameItem = MakeShareable(new FName(Name));
			NameList.Add(RowNameItem);
		}
	}
}
