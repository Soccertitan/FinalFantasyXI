// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "EquipmentManagerViewModel.generated.h"

class UItemInstanceViewModelFilter_EquipableItems;
class UItemInstanceViewModel;
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
	UEquipmentManagerViewModel();
	
	/** Finds or creates an ActionBarItem VM from InputTag. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|EquipSystem")
	UEquippedItemViewModel* FindOrCreateEquippedItemViewModel(UPARAM(meta = (Categories = "EquipSlot")) const FGameplayTag EquipSlot);
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Viewmodel|EquipSystem")
	TArray<UItemContainerViewModel*> GetAllowedItemContainers() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category = "Viewmodel|EquipSystem")
	TArray<UItemInstanceViewModel*> GetEquippableItems(UPARAM(meta = (Categories = "EquipSlot")) const FGameplayTag EquipSlot, UItemContainerViewModel* ItemContainerViewModel) const;
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|EquipSystem")
	void TryEquipItem(UPARAM(meta = (Categories = "EquipSlot")) FGameplayTag EquipSlot, UItemInstanceViewModel* ItemInstanceViewModel);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|EquipSystem")
	void TryUnequipItem(UPARAM(meta = (Categories = "EquipSlot")) FGameplayTag EquipSlot);
	
protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
private:
	UPROPERTY(EditDefaultsOnly, Instanced, NoClear)
	TObjectPtr<UItemInstanceViewModelFilter_EquipableItems> EquippableItemsFilter;

	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;
	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UEquippedItemViewModel>> EquippedItemViewModels;
	UPROPERTY()
	TArray<TObjectPtr<UItemContainerViewModel>> AllowedItemContainerViewModels;
	
	UEquippedItemViewModel* InternalCreateEquippedItemViewModel(const FGameplayTag& EquipSlot);
	
	UFUNCTION()
	void OnItemEquipped(const FEquippedItem& EquippedItem);
	UFUNCTION()
	void OnItemUnequipped(const FEquippedItem& EquippedItem);
	UFUNCTION()
	void OnItemChanged(const FItemInstance& ItemInstance);
};
