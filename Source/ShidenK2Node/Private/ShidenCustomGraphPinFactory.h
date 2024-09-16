// Copyright (c) 2024 HANON. All Rights Reserved.
#pragma once

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraphSchema_K2.h"
#include "HAL/PlatformCrt.h"
#include "Templates/Casts.h"
#include "UObject/Class.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "K2Node_GetCommandArguments.h"
#include "SGraphPinShidenCommandName.h"
#include "EdGraphUtilities.h"

class FShidenCustomGraphPanelPinFactory : public FGraphPanelPinFactory
{
	TSharedPtr<class SGraphPin> CreatePin(class UEdGraphPin* InPin) const override
	{
		if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_String)
		{
			UObject* Outer = InPin->GetOuter();

			if (Outer->IsA(UK2Node_GetCommandArguments::StaticClass()) && InPin->Direction == EEdGraphPinDirection::EGPD_Input && InPin->GetName() == TEXT("CommandName"))
			{
				const UK2Node_GetCommandArguments* GetShidenCommandNode = CastChecked<UK2Node_GetCommandArguments>(Outer);
				
				if (const UEdGraphPin* CommandDefinitionsPin = GetShidenCommandNode->GetCommandDefinitionsPin())
				{
					if (CommandDefinitionsPin->DefaultObject != nullptr && CommandDefinitionsPin->LinkedTo.Num() == 0)
					{
						if (auto CommandDefinitions = Cast<UShidenCommandDefinitions>(CommandDefinitionsPin->DefaultObject))
						{
							return SNew(SGraphPinShidenCommandName, InPin, CommandDefinitions);
						}
					}
				}
			}
		}

		return nullptr;
	}
};