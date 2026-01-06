// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysViewModel.h"
#include "GameplayTagContainer.h"
#include "WeaponSetManagerViewModel.generated.h"

class UEquipmentManagerComponent;
class UItemContainerViewModel;
class UItemViewModelFilter_EquipableItems;
class UItemInstanceViewModel;
class UGameplayAbilityDefinition;
class UWeaponSetViewModel;
class UInventoryManagerComponent;
class UWeaponSetManagerComponent;
struct FWeaponSet;

/**
 * View model to change WeaponSets.
 */
UCLASS()
class FINALFANTASYXI_API UWeaponSetManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:
	UWeaponSetManagerViewModel();
	
	/**
	 * Will try and find a viewmodel at the given Index. If none is found, it will create one and return that.
	 * @param Index The index for the viewmodel.
	 */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager|ViewModel")
	UWeaponSetViewModel* FindOrCreateWeaponSetViewModel(const int32 Index = 0);

	/** Sets the active weapon set to the passed in Index value. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetManager|ViewModel")
	void SetActiveWeaponSet(const int32 Index);

	/** Sets the passed in ItemGuid as the weapon for the WeaponSet at Index. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetViewModel")
	void SetWeaponItem(UPARAM(ref) const FGuid& ItemGuid, const int32 Index);

	/** Removes the item from the WeaponSet. Internally calls SetWeaponItem with an invalid Guid. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetViewModel")
	void ClearWeaponItem(const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "WeaponSetViewModel")
	void SetAbilityInput(UPARAM(ref) const FGameplayTag& InputTag, UGameplayAbilityDefinition* AbilityDefinition, const int32 Index);

	UFUNCTION(BlueprintCallable, Category = "WeaponSetViewModel")
	void ClearAbilityInput(UPARAM(ref) const FGameplayTag& InputTag, const int32 Index);

	/** Removes the WeaponSet from the container. */
	UFUNCTION(BlueprintCallable, Category = "WeaponSetViewModel")
	void ResetWeaponSet(const int32 Index);

	/** Gets all weapons that can be equipped from the ItemContainer Weapons. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<UItemInstanceViewModel*> GetEquipableWeapons() const;

	/** Gets all abilities that can be equipped from the ItemContainer Ability. */
	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	TArray<UItemInstanceViewModel*> GetEquipableAbilities() const;

	int32 GetActiveWeaponSetIndex() const { return ActiveWeaponSetIndex; }

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	void SetActiveWeaponSetIndex(const int32 InValue);
	UItemInstanceViewModel* CreateWeaponViewModel(const FGuid& ItemGuid);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "WeaponSetViewModel", meta=(AllowPrivateAccess=true))
	int32 ActiveWeaponSetIndex = INDEX_NONE;

	/** The ItemContainer to search for from the ItemManager that contains the weapon items. */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemContainerTag_Equipment;
	/** The ItemContainer to search for from the ItemManager that contains abilities. */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ItemContainerTag_Ability;

	/** Cached WeaponSetManagerComponent from the PlayerController. */
	UPROPERTY()
	TObjectPtr<UWeaponSetManagerComponent> WeaponSetManagerComponent;
	/** Cached InventoryManagerComponent from the PlayerSate. */
	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	/** Cached EquipmentManagerComponent from the PlayerState. */
	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	UPROPERTY()
	TArray<TObjectPtr<UWeaponSetViewModel>> WeaponSetViewModels;

	UPROPERTY()
	TObjectPtr<UItemContainerViewModel> ItemContainerViewModel_Equipment;
	UPROPERTY()
	TObjectPtr<UItemContainerViewModel> ItemContainerViewModel_Abilities;

	void OnActiveWeaponSetChanged(int32 NewIndex, int32 OldIndex);
	void OnWeaponSetChanged(const FWeaponSet& WeaponSet, int32 Index);

	/** Filters out Items that cannot be equipped to the weapon slot. */
	UPROPERTY()
	TObjectPtr<UItemViewModelFilter_EquipableItems> ItemFilter_EquipableItems;
};
