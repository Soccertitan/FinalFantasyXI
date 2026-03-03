// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "EquipmentManagerViewModel.generated.h"

struct FItemInstance;
class UInventoryManagerComponent;
struct FEquippedItem;
struct FGameplayTag;
class UEquippedItemViewModel;
class UItemContainerViewModel;
class UEquipmentManagerComponent;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UEquipmentManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	
	/** Finds or creates an ActionBarItem VM from InputTag. */
	UFUNCTION(BlueprintCallable, Category = "ActionBarViewModel")
	UEquippedItemViewModel* FindOrCreateEquippedItemViewModel(UPARAM(meta = (Categories = "EquipSlot")) const FGameplayTag EquipSlot);
	
protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
private:
	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UEquippedItemViewModel>> EquippedItemViewModels;
	
	UEquippedItemViewModel* InternalCreateEquippedItemViewModel(const FGameplayTag& EquipSlot);
	
	UFUNCTION()
	void OnItemEquipped(const FEquippedItem& EquippedItem);
	UFUNCTION()
	void OnItemUnequipped(const FEquippedItem& EquippedItem);
	UFUNCTION()
	void OnItemChanged(const FItemInstance& ItemInstance);
};
