// Copyright (c) 2025 HANON. All Rights Reserved.
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

class FShidenCustomGraphPanelPinFactory final : public FGraphPanelPinFactory
{
	virtual TSharedPtr<SGraphPin> CreatePin(UEdGraphPin* InPin) const override
	{
		if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_String)
		{
			const TObjectPtr<UObject> Outer = InPin->GetOuter();

			if (Outer->IsA(UK2Node_GetCommandArguments::StaticClass()) && InPin->Direction == EGPD_Input && InPin->GetName() == TEXT("CommandName"))
			{
				const UK2Node_GetCommandArguments* GetShidenCommandNode = CastChecked<UK2Node_GetCommandArguments>(Outer);

				if (const UEdGraphPin* CommandDefinitionsPin = GetShidenCommandNode->FindCommandDefinitionsPin())
				{
					if (CommandDefinitionsPin->DefaultObject != nullptr && CommandDefinitionsPin->LinkedTo.Num() == 0)
					{
						if (TObjectPtr<UShidenCommandDefinitions> CommandDefinitions = Cast<UShidenCommandDefinitions>(
							CommandDefinitionsPin->DefaultObject))
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
