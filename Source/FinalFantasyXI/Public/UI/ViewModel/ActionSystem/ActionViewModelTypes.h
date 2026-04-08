// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "ActionViewModelTypes.generated.h"


class UActionSlotViewModel;

USTRUCT()
struct FActionViewModelContainer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TObjectPtr<UActionSlotViewModel>> Items;
};
