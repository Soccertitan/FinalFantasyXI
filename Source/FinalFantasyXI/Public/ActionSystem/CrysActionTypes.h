// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "CrysActionTypes.generated.h"

class UCrysAction;

USTRUCT(BlueprintType)
struct FCrysActionMapSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (NoResetToDefault, Categories = "Input"))
	TMap<FGameplayTag, TSoftClassPtr<UCrysAction>> ActionMap;
};

USTRUCT(BlueprintType)
struct FCrysActionContainerSaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (NoResetToDefault))
	TArray<FCrysActionMapSaveData> CrysActionMappings;
};
