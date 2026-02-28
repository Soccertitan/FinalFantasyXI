// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/Filter/ItemInstanceViewModelFilter.h"
#include "ItemViewModelFilter_EquipableItems.generated.h"

/**
 * Given a context that is the EquipmentManagerComponent, will filter out items that can't be equipped.
 */
UCLASS()
class FINALFANTASYXI_API UItemViewModelFilter_EquipableItems : public UItemInstanceViewModelFilter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "EquipSlot"))
	FGameplayTag EquipSlot;

protected:
	virtual bool ShouldBeginFilter(const UObject* Context, TArray<UItemInstanceViewModel*>& ViewModels) const override;
	virtual bool DoesItemInstanceViewModelPassFilter(const UObject* Context, UItemInstanceViewModel* ViewModel) const override;
};
