// Copyright (c) 2024 HANON. All Rights Reserved.

#include "K2Node_GetCommandArguments.h"

#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintNodeSpawner.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphPin.h"
#include "EdGraph/EdGraphSchema.h"
#include "EdGraphSchema_K2.h"
#include "EditorCategoryUtils.h"
#include "Engine/MemberReference.h"
#include "Internationalization/Internationalization.h"
#include "K2Node_CallFunction.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/CompilerResultsLog.h"
#include "KismetCompiler.h"
#include "Misc/AssertionMacros.h"
#include "Styling/AppStyle.h"
#include "Templates/Casts.h"
#include "UObject/Class.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectBaseUtility.h"
#include "UObject/UnrealNames.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "ShidenCommandDefinitions.h"
#include "ShidenCommand.h"
#include "ShidenCoreFunctionLibrary.h"

class UBlueprint;

#define LOCTEXT_NAMESPACE "K2Node_GetShidenCommand"

static const FName CommandPinName(TEXT("Command"));
static const FName CommandDefinitionsPinName(TEXT("CommandDefinitions"));
static const FName CommandNamePinName(TEXT("CommandName"));

UK2Node_GetCommandArguments::UK2Node_GetCommandArguments(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NodeTooltip = LOCTEXT("NodeTooltip", "Attempts to retrieve a Command Arguments from a CommandDefinitions");
	OnCommandDefinitionsChangedHandle = FDelegateHandle();
}

void UK2Node_GetCommandArguments::AllocateDefaultPins()
{
	const UEdGraphSchema_K2* K2Schema = GetDefault<UEdGraphSchema_K2>();

	// Add CommandDefinitions pin
	UEdGraphPin* CommandDefinitionsPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Object, UShidenCommandDefinitions::StaticClass(), CommandDefinitionsPinName);
	CommandDefinitionsPin->bNotConnectable = true;
	SetPinToolTip(*CommandDefinitionsPin, LOCTEXT("CommandDefinitionsPinDescription", "The CommandDefinitions"));

	// Command Name pin
	UEdGraphPin* CommandNamePin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_String, CommandNamePinName);
	CommandNamePin->bNotConnectable = true;
	CommandNamePin->bHidden = !CommandDefinitionsPin->DefaultObject;
	SetPinToolTip(*CommandNamePin, LOCTEXT("CommandNamePinDescription", "The command name"));

	// Add Command pin
	UScriptStruct* ShidenCommandStruct = TBaseStructure<FShidenCommand>::Get();
	UEdGraphPin* CommandPin = CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Struct, ShidenCommandStruct, CommandPinName);
	SetPinToolTip(*CommandPin, LOCTEXT("CommandPinDescription", "The Command"));

	Super::AllocateDefaultPins();
}

void UK2Node_GetCommandArguments::SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const
{
	MutatablePin.PinToolTip = UEdGraphSchema_K2::TypeToText(MutatablePin.PinType).ToString();

	UEdGraphSchema_K2 const* const K2Schema = Cast<const UEdGraphSchema_K2>(GetSchema());
	if (K2Schema != nullptr)
	{
		MutatablePin.PinToolTip += TEXT(" ");
		MutatablePin.PinToolTip += K2Schema->GetPinDisplayName(&MutatablePin).ToString();
	}

	MutatablePin.PinToolTip += FString(TEXT("\n")) + PinDescription.ToString();
}

bool UK2Node_GetCommandArguments::IsOutputPinChanged(TArray<UEdGraphPin*> OldPins, UShidenCommandDefinitions* Definitions, FString CommandName)
{
	TArray<FString> OldPinNames = TArray<FString>();
	for (UEdGraphPin* OldPin : OldPins)
	{
		if (OldPin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			OldPinNames.Add(OldPin->PinName.ToString());
		}
	}

	if (Definitions)
	{
		if (!Definitions->CommandDefinitions.Contains(CommandName))
		{
			return true;
		}
		TArray<FString> Keys = TArray<FString>();
		for (auto& Arg : Definitions->CommandDefinitions[CommandName].Args)
		{
			Keys.Add(Arg.ArgName.ToString());
		}
		for (FString Key : Keys)
		{
			if (!OldPinNames.Contains(Key))
			{
				return true;
			}
		}
		for (FString OldPinName : OldPinNames)
		{
			if (!Keys.Contains(OldPinName))
			{
				return true;
			}
		}
		return false;
	} else {
		return OldPinNames.Num() > 0;
	}
}

void UK2Node_GetCommandArguments::RefreshOutputPin()
{
	TArray<UEdGraphPin*> OldPins = Pins;
	TArray<UEdGraphPin*> OldOutputPins = TArray<UEdGraphPin*>();
	UEdGraphPin* CommandDefinitionsPin = GetCommandDefinitionsPin();
	UEdGraphPin* CommandNamePin = GetCommandNamePin();
	FString CommandName = CommandNamePin->DefaultValue;
	TObjectPtr<UShidenCommandDefinitions> Definitions = Cast<UShidenCommandDefinitions>(CommandDefinitionsPin->DefaultObject);

	if (Definitions)
	{
		PreloadObject(Definitions);
	}
	
	if (!IsOutputPinChanged(OldPins, Definitions, CommandName))
	{
		return;
	}

	for (int32 i = 0; i < OldPins.Num(); i++)
	{
		UEdGraphPin* OldPin = OldPins[i];
		if (OldPin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			OldOutputPins.Add(OldPin);
			Pins.Remove(OldPin);
		}
	}

	if (Definitions)
	{
		CreateOutputPins(Definitions, CommandName);
	}

	RestoreSplitPins(OldOutputPins);
	RewireOldPinsToNewPins(OldOutputPins, Pins, nullptr);
	FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());

	ChangeAdvancedView();

	if (UEdGraph* Graph = GetGraph())
	{
		Graph->NotifyNodeChanged(this);
	}
}

void UK2Node_GetCommandArguments::ChangeAdvancedView()
{
	int32 NewOutputPinCount = 0;
	for (UEdGraphPin* Pin : Pins)
	{
		if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
		{
			NewOutputPinCount++;
		}
	}

	if (NewOutputPinCount > 7 && Pins.Num() - NewOutputPinCount < 4)
	{
		if (ENodeAdvancedPins::NoPins == AdvancedPinDisplay)
		{
			AdvancedPinDisplay = ENodeAdvancedPins::Hidden;
		}

		for (int32 PinIndex = 8; PinIndex < Pins.Num(); ++PinIndex)
		{
			if (UEdGraphPin* EdGraphPin = Pins[PinIndex])
			{
				EdGraphPin->bAdvancedView = true;
			}
		}
	}
	else
	{
		AdvancedPinDisplay = ENodeAdvancedPins::NoPins;
	}
}

void UK2Node_GetCommandArguments::OnCommandDefinitionsRowListChanged(const UShidenCommandDefinitions* CommandDefinitions)
{
	UEdGraphPin* CommandDefinitionsPin = GetCommandDefinitionsPin();
	if (CommandDefinitions && CommandDefinitionsPin && CommandDefinitions == CommandDefinitionsPin->DefaultObject)
	{
		UEdGraphPin* CommandNamePin = GetCommandNamePin();
		const bool TryRefresh = CommandNamePin && !CommandNamePin->LinkedTo.Num();
		const FString CurrentName = CommandNamePin ? *CommandNamePin->GetDefaultAsString() : TEXT("");
		TArray<FString> Keys = TArray<FString>();
		CommandDefinitions->CommandDefinitions.GetKeys(Keys);
		if (TryRefresh && CommandNamePin && !Keys.Contains(CurrentName))
		{
			FBlueprintEditorUtils::MarkBlueprintAsModified(GetBlueprint());
		}
	}
}

void UK2Node_GetCommandArguments::CreateOutputPins(const UShidenCommandDefinitions* InDefinitions, const FString CommandName)
{
	check(InDefinitions != NULL);

	const UEdGraphSchema_K2* K2Schema = GetDefault< UEdGraphSchema_K2 >();

	if (InDefinitions->CommandDefinitions.Contains(CommandName))
	{
		for (auto& Arg : InDefinitions->CommandDefinitions[CommandName].Args)
		{
			UEdGraphPin* OutputPin = CreatePin(EGPD_Output, K2Schema->PC_String, Arg.ArgName);
			SetPinToolTip(*OutputPin, FText::FromString(Arg.ArgName.ToString()));
		}
	}
}

void UK2Node_GetCommandArguments::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	if (UEdGraphPin* CommandDefinitionsPin = GetCommandDefinitionsPin(&OldPins))
	{
		if (UShidenCommandDefinitions* Definitions = Cast<UShidenCommandDefinitions>(CommandDefinitionsPin->DefaultObject))
		{
			PreloadObject(Definitions);
			if (CommandDefinitionsCache && OnCommandDefinitionsChangedHandle.IsValid())
			{
				CommandDefinitionsCache->OnCommandDefinitionsChanged.Remove(OnCommandDefinitionsChangedHandle);
			}

			OnCommandDefinitionsChangedHandle = Definitions->OnCommandDefinitionsChanged.AddUObject(this, &UK2Node_GetCommandArguments::OnDataAssetChanged);
			CommandDefinitionsCache = Definitions;
			if (UEdGraphPin* CommandNamePin = GetCommandNamePin(&OldPins))
			{
				CreateOutputPins(Definitions, CommandNamePin->DefaultValue);
				ChangeAdvancedView();
			}
		}

		if (UEdGraphPin* CommandNamePin = GetCommandNamePin())
		{
			CommandNamePin->bHidden = !CommandDefinitionsPin->DefaultObject;
		}
	}
}

void UK2Node_GetCommandArguments::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_GetCommandArguments::GetMenuCategory() const
{
	return FText::FromString(TEXT("ShidenVisualNovel|Command"));
}

void UK2Node_GetCommandArguments::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin)
	{
		if (ChangedPin->PinName == CommandDefinitionsPinName)
		{
			if (UEdGraphPin* CommandNamePin = GetCommandNamePin())
			{
				if (UShidenCommandDefinitions* CommandDefinitions = Cast<UShidenCommandDefinitions>(ChangedPin->DefaultObject)) {
					TArray<FString> Keys = TArray<FString>();
					CommandDefinitions->CommandDefinitions.GetKeys(Keys);
					if (CommandNamePin->DefaultValue.IsEmpty() || !Keys.Contains(*CommandNamePin->DefaultValue))
					{
						CommandNamePin->DefaultValue = Keys.Num() > 0 ? Keys[0] : TEXT("");
					}
					CommandNamePin->bHidden = false;

					if (CommandDefinitionsCache && OnCommandDefinitionsChangedHandle.IsValid())
					{
						CommandDefinitionsCache->OnCommandDefinitionsChanged.Remove(OnCommandDefinitionsChangedHandle);
					}

					OnCommandDefinitionsChangedHandle = CommandDefinitions->OnCommandDefinitionsChanged.AddUObject(this, &UK2Node_GetCommandArguments::OnDataAssetChanged);
					CommandDefinitionsCache = CommandDefinitions;
				}
				else
				{
					CommandNamePin->bHidden = true;
					CommandNamePin->DefaultValue = TEXT("");

					if (CommandDefinitionsCache && OnCommandDefinitionsChangedHandle.IsValid())
					{
						CommandDefinitionsCache->OnCommandDefinitionsChanged.Remove(OnCommandDefinitionsChangedHandle);
					}

					CommandDefinitionsCache = nullptr;
				}
				RefreshOutputPin();
			}
		}
		else if (ChangedPin->PinName == CommandNamePinName)
		{
			RefreshOutputPin();
		}
	}
}

void UK2Node_GetCommandArguments::OnDataAssetChanged()
{
	RefreshOutputPin();
}

FText UK2Node_GetCommandArguments::GetTooltipText() const
{
	return NodeTooltip;
}

UEdGraphPin* UK2Node_GetCommandArguments::GetCommandPin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;

	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == CommandPinName)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetCommandArguments::GetCommandDefinitionsPin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;

	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == CommandDefinitionsPinName)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_GetCommandArguments::GetCommandNamePin(const TArray<UEdGraphPin*>* InPinsToSearch /*= NULL*/) const
{
	const TArray<UEdGraphPin*>* PinsToSearch = InPinsToSearch ? InPinsToSearch : &Pins;

	UEdGraphPin* Pin = nullptr;
	for (UEdGraphPin* TestPin : *PinsToSearch)
	{
		if (TestPin && TestPin->PinName == CommandNamePinName)
		{
			Pin = TestPin;
			break;
		}
	}
	check(Pin == nullptr || Pin->Direction == EGPD_Input);
	return Pin;
}

FText UK2Node_GetCommandArguments::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return NSLOCTEXT("K2Node", "Get Command Arguments", "Get Command Arguments");
}

void UK2Node_GetCommandArguments::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);

	UEdGraphPin* OriginalGetCommandDefinitionsInPin = GetCommandDefinitionsPin();
	TObjectPtr<UShidenCommandDefinitions> Definitions = (OriginalGetCommandDefinitionsInPin != NULL) ? Cast<UShidenCommandDefinitions>(OriginalGetCommandDefinitionsInPin->DefaultObject) : NULL;
	if ((nullptr == OriginalGetCommandDefinitionsInPin) || (0 == OriginalGetCommandDefinitionsInPin->LinkedTo.Num() && nullptr == Definitions))
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("GetCommandArgumentsNoCommandDefinitions_Error", "GetCommandArguments must have a CommandDefinitions specified.").ToString(), this);
		// we break exec links so this is the only error we get
		BreakAllNodeLinks();
		return;
	}

	const FName FunctionName = GET_MEMBER_NAME_CHECKED(UShidenCoreFunctionLibrary, GetCommandArgument);
	int32 Count = 0;
	TArray<UEdGraphPin*> OutputPins = TArray< UEdGraphPin* >();
	for (UEdGraphPin* TestPin : Pins)
	{
		if (TestPin && TestPin->Direction == EEdGraphPinDirection::EGPD_Output && TestPin->LinkedTo.Num() > 0)
		{
			OutputPins.Add(TestPin);
		}
	}
	for (UEdGraphPin* OutputPin : OutputPins)
	{
		Count++;
		UK2Node_CallFunction* GetCommandArgumentFunction = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
		GetCommandArgumentFunction->SetFromFunction(UShidenCoreFunctionLibrary::StaticClass()->FindFunctionByName(FunctionName));
		GetCommandArgumentFunction->AllocateDefaultPins();
		UEdGraphPin* ArgNameInPin = GetCommandArgumentFunction->FindPinChecked(TEXT("ArgName"));
		ArgNameInPin->DefaultValue = OutputPin->PinName.ToString();
		UEdGraphPin* CommandInPin = GetCommandArgumentFunction->FindPinChecked(TEXT("Command"));
		if (Count == OutputPins.Num())
		{
			CompilerContext.MovePinLinksToIntermediate(*FindPinChecked(CommandPinName), *CommandInPin);
		}
		else
		{
			CompilerContext.CopyPinLinksToIntermediate(*FindPinChecked(CommandPinName), *CommandInPin);
		}
		UEdGraphPin* GetCommandArgumentResultPin = GetCommandArgumentFunction->GetReturnValuePin();
		CompilerContext.MovePinLinksToIntermediate(*OutputPin, *GetCommandArgumentResultPin);
	}

	BreakAllNodeLinks();
}

FSlateIcon UK2Node_GetCommandArguments::GetIconAndTint(FLinearColor& OutColor) const
{
	OutColor = GetNodeTitleColor();
	static FSlateIcon Icon(FAppStyle::GetAppStyleSetName(), "Kismet.AllClasses.FunctionIcon");
	return Icon;
}

void UK2Node_GetCommandArguments::EarlyValidation(class FCompilerResultsLog& MessageLog) const
{
	Super::EarlyValidation(MessageLog);

	const UEdGraphPin* CommandDefinitionsPin = GetCommandDefinitionsPin();
	const UEdGraphPin* CommandNamePin = GetCommandNamePin();
	if (!CommandDefinitionsPin || !CommandNamePin)
	{
		MessageLog.Error(*LOCTEXT("MissingPins", "Missing pins in @@").ToString(), this);
		return;
	}

	const UEdGraphPin* CommandPin = GetCommandPin();
	if (!CommandPin || (CommandPin->LinkedTo.Num() == 0 && !CommandPin->DefaultObject && CommandPin->SubPins.Num() == 0))
	{
		MessageLog.Error(*LOCTEXT("CommandPinDefaultValue", "The current value of the '@@' pin is invalid.").ToString(), this);
		return;
	}

	if (UShidenCommandDefinitions* CommandDefinitions = Cast<UShidenCommandDefinitions>(CommandDefinitionsPin->DefaultObject))
	{
		TArray<FString> Keys = TArray<FString>();
		CommandDefinitions->CommandDefinitions.GetKeys(Keys);
		if (!Keys.Contains(CommandNamePin->DefaultValue))
		{
			MessageLog.Error(*LOCTEXT("CommandNameNotFound", "Command name not found in CommandDefinitions. @@").ToString(), this);
		}
		else
		{
			TArray<FString> ArgKeys = TArray<FString>();
			for (auto& Arg : CommandDefinitions->CommandDefinitions[CommandNamePin->DefaultValue].Args)
			{
				ArgKeys.Add(Arg.ArgName.ToString());
			}
			for (UEdGraphPin* Pin : Pins)
			{
				if (Pin->Direction == EEdGraphPinDirection::EGPD_Output)
				{
					if (!ArgKeys.Contains(Pin->PinName.ToString()))
					{
						MessageLog.Error(*LOCTEXT("ArgNameNotFound", "@@ not found in CommandDefinitions.").ToString(), Pin);
					}
				}
			}
		}
	}
}

void UK2Node_GetCommandArguments::PreloadRequiredAssets()
{
	if (UEdGraphPin* CommandDefinitionsPin = GetCommandDefinitionsPin())
	{
		if (UShidenCommandDefinitions* CommandDefinitions = Cast<UShidenCommandDefinitions>(CommandDefinitionsPin->DefaultObject))
		{
			PreloadObject(CommandDefinitions);
		}
	}
	return Super::PreloadRequiredAssets();
}

#undef LOCTEXT_NAMESPACE