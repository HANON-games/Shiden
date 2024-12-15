// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Command/ShidenCommandDefinitions.h"
#include "SGraphPinNameList.h"
#include "UObject/SoftObjectPtr.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UShidenCommandDefinitions;
class UEdGraphPin;

class SHIDENK2NODE_API SGraphPinShidenCommandName : public SGraphPinNameList
{
public:
	SLATE_BEGIN_ARGS(SGraphPinShidenCommandName) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj, const TObjectPtr<UShidenCommandDefinitions> InCommandDefinitions);

	SGraphPinShidenCommandName();
	virtual ~SGraphPinShidenCommandName() override;

protected:
	void RefreshNameList(const TObjectPtr<UShidenCommandDefinitions>& CommandDefinitions);
};