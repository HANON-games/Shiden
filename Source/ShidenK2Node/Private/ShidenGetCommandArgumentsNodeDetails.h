// Copyright (c) 2026 HANON. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtr.h"

class IDetailChildrenBuilder;
class IDetailLayoutBuilder;
class IPropertyHandle;
class UK2Node_GetCommandArguments;

enum class ECheckBoxState : uint8;

class FShidenGetCommandArgumentsNodeDetails final : public IDetailCustomization
{
public:
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	void OnGenerateElementForPropertyPin(TSharedRef<IPropertyHandle> ElementProperty, int32, IDetailChildrenBuilder& ChildrenBuilder);

	ECheckBoxState GetShowPinValueForProperty(TSharedRef<IPropertyHandle> InElementProperty) const;

	void OnShowPinChanged(ECheckBoxState InNewState, TSharedRef<IPropertyHandle> InElementProperty);

	FReply HideUnconnectedPins() const;

	TWeakObjectPtr<UK2Node_GetCommandArguments> TargetNode;
};
