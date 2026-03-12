// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "MVVMViewModelBase.h"
#include "EquippedItemViewModel.generated.h"

class UCrysGameplayTagViewModel;
class UItemInstanceViewModel;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UEquippedItemViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	FGameplayTag GetEquipSlot() const { return EquipSlot; }
	UItemInstanceViewModel* GetItemInstanceViewModel() const { return ItemInstanceViewModel; }
	UCrysGameplayTagViewModel* GetEquipSlotGameplayTagViewModel() const { return EquipSlotGameplayTagViewModel; }
	
protected:
	void SetEquipSlot(const FGameplayTag& InValue);
	void SetItemInstanceViewModel(UItemInstanceViewModel* InValue);
	void SetEquipSlotGameplayTagViewModel(UCrysGameplayTagViewModel* InValue);
	
private:
	/** The EquipSlot the item is equipped to. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "EquippedItemViewModel", meta = (AllowPrivateAccess = true))
	FGameplayTag EquipSlot;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "EquippedItemViewModel", meta = (AllowPrivateAccess = true))
	TObjectPtr<UItemInstanceViewModel> ItemInstanceViewModel;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "EquippedItemViewModel", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCrysGameplayTagViewModel> EquipSlotGameplayTagViewModel;
	
	friend class UEquipmentManagerViewModel;
};
