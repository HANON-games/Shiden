// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "EdGraph/EdGraphNodeUtils.h"
#include "HAL/PlatformCrt.h"
#include "Internationalization/Text.h"
#include "K2Node.h"
#include "Textures/SlateIcon.h"
#include "UObject/ObjectMacros.h"
#include "UObject/UObjectGlobals.h"
#include "Command/ShidenCommandDefinitions.h"
#include "K2Node_GetCommandArguments.generated.h"

class FBlueprintActionDatabaseRegistrar;
class FString;
class UDataTable;
class UEdGraph;
class UEdGraphPin;
class UObject;
class UScriptStruct;
struct FLinearColor;

UCLASS()
class SHIDENK2NODE_API UK2Node_GetCommandArguments : public UK2Node
{
	GENERATED_UCLASS_BODY()

	//~ Begin UEdGraphNode Interface.
	virtual void AllocateDefaultPins() override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	virtual FText GetTooltipText() const override;
	virtual void ExpandNode(FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual void EarlyValidation(FCompilerResultsLog& MessageLog) const override;
	virtual void PreloadRequiredAssets() override;
	virtual bool IsNodePure() const override { return true; }
	//~ End UK2Node Interface

	UEdGraphPin* GetCommandPin(const TArray<UEdGraphPin*>* InPinsToSearch = nullptr) const;

	UEdGraphPin* GetCommandDefinitionsPin(const TArray<UEdGraphPin*>* InPinsToSearch = nullptr) const;

	UEdGraphPin* GetCommandNamePin(const TArray<UEdGraphPin*>* InPinsToSearch = nullptr) const;

	void CreateOutputPins(const UShidenCommandDefinitions* InDefinitions, const FString& CommandName);

	void OnCommandDefinitionsRowListChanged(const UShidenCommandDefinitions* CommandDefinitions) const;
private:
	/**
	 * Takes the specified "MutatablePin" and sets its 'PinToolTip' field (according
	 * to the specified description)
	 *
	 * @param   MutatablePin	The pin you want to set tool-tip text on
	 * @param   PinDescription	A string describing the pin's purpose
	 */
	void SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const;

	void RefreshOutputPin();

	void OnDataAssetChanged();

	void ChangeAdvancedView();

	static bool IsOutputPinChanged(TArray<UEdGraphPin*> OldPins, UShidenCommandDefinitions* Definitions, const FString& CommandName);

	FText NodeTooltip;

	FDelegateHandle OnCommandDefinitionsChangedHandle;

	UPROPERTY()
	UShidenCommandDefinitions* CommandDefinitionsCache;
};