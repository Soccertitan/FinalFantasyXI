// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ActionViewModelTypes.generated.h"


class UActionViewModel;

USTRUCT(BlueprintType)
struct FActionViewModelUpdated
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;
	
	UPROPERTY(BlueprintReadOnly)
	int32 Index = -1;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UActionViewModel> ViewModel;
};

USTRUCT()
struct FActionViewModelItem
{
	GENERATED_BODY()

	FGameplayTag InputTag;
	
	UPROPERTY()
	TObjectPtr<UActionViewModel> ViewModel;
};

USTRUCT()
struct FActionViewModelContainer
{
	GENERATED_BODY()
	
	TArray<FActionViewModelItem> Items;
};
