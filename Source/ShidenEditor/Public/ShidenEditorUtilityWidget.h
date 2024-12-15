// Copyright (c) 2024 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ShidenEditorUtilityWidget.generated.h"

UCLASS(BlueprintType)
class SHIDENEDITOR_API UShidenEditorUtilityWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	explicit UShidenEditorUtilityWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnPostUndo"))
	void ReceivePostUndo();

	virtual void PostEditUndo() override;
};
