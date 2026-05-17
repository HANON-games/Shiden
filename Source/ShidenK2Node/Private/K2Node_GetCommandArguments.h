// Copyright (c) 2026 HANON. All Rights Reserved.

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
class FProperty;
class FString;
class UDataTable;
class UEdGraph;
// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
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
	virtual void GetNodeContextMenuActions(class UToolMenu* Menu, class UGraphNodeContextMenuContext* Context) const override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PreEditChange(FProperty* PropertyAboutToChange) override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	virtual bool IsNodeSafeToIgnore() const override { return true; }
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual void EarlyValidation(FCompilerResultsLog& MessageLog) const override;
	virtual void PreloadRequiredAssets() override;
	virtual bool IsNodePure() const override { return true; }
	virtual bool ShouldShowNodeProperties() const override { return true; }
	//~ End UK2Node Interface

	UPROPERTY(EditAnywhere, Category = PinOptions, EditFixedSize)
	TArray<FOptionalPinFromProperty> ShowPinForProperties;

	UEdGraphPin* FindCommandPin(const TArray<UEdGraphPin*>* InPinsToSearch = nullptr) const;

	UEdGraphPin* FindCommandDefinitionsPin(const TArray<UEdGraphPin*>* InPinsToSearch = nullptr) const;

	UEdGraphPin* FindCommandNamePin(const TArray<UEdGraphPin*>* InPinsToSearch = nullptr) const;

	void CreateOutputPins(const UShidenCommandDefinitions* InDefinitions, const FString& InCommandName);

	void OnCommandDefinitionsRowListChanged(const UShidenCommandDefinitions* CommandDefinitions) const;

	void HideUnconnectedOutputPins();

	bool CanHideUnconnectedOutputPins() const;

private:
	/**
	 * Takes the specified "MutatablePin" and sets its 'PinToolTip' field (according
	 * to the specified description)
	 *
	 * @param MutatablePin	 The pin you want to set tool-tip text on
	 * @param PinDescription A string describing the pin's purpose
	 */
	void SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const;

	void RefreshOutputPins();

	void OnCommandDefinitionsChanged();

	void ChangeAdvancedView();

	void RebuildOutputPinVisibility(const UShidenCommandDefinitions* InDefinitions, const FString& InCommandName);

	bool IsOutputPinVisible(const FName& PinName) const;

	bool IsOutputPinChanged(const TArray<UEdGraphPin*>& OldPins, const UShidenCommandDefinitions* InDefinitions, const FString& InCommandName) const;

	FText NodeTooltip;

	TArray<FName> OldShownPins;

	FDelegateHandle OnCommandDefinitionsChangedHandle;

	UPROPERTY()
	UShidenCommandDefinitions* CommandDefinitionsCache;
};
