// Copyright (c) 2025 HANON. All Rights Reserved.

#pragma once

#include "Save/ShidenBaseSaveGame.h"
#include "System/ShidenReadLines.h"
#include "Variable/ShidenPredefinedSystemVariable.h"
#include "ShidenPredefinedSystemSaveGame.generated.h"

UCLASS(NotBlueprintable)
class SHIDENCORE_API UShidenPredefinedSystemSaveGame : public UShidenBaseSaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	FShidenPredefinedSystemVariable PredefinedSystemVariable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shiden Visual Novel|Save Game")
	TMap<FGuid, FShidenReadLines> ScenarioReadLines;

	static TObjectPtr<UShidenPredefinedSystemSaveGame> GetOrCreate();
	
	void Apply(TObjectPtr<const UObject> WorldContextObject) const;

	void Prepare();

	bool TryCommit();

	static bool DoesExist();
	
	static bool TryDelete();

private:
	constexpr static TCHAR PredefinedSystemDataSlotName[] = TEXT("ShidenPredefinedSystemData");
};
