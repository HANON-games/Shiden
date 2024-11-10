// Copyright (c) 2024 HANON. All Rights Reserved.

#include "Command/ShidenCommandDefinitions.h"

#if WITH_EDITOR
void UShidenCommandDefinitions::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    
    if (PropertyChangedEvent.Property && 
        (PropertyChangedEvent.Property->GetFName() == FName(TEXT("Args")) || 
            PropertyChangedEvent.Property->GetFName() == FName(TEXT("ArgName"))))
    {
        OnCommandDefinitionsChanged.Broadcast();
    }
}
#endif
