// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "EquipmentTypes.h"
#include "Components/ActorComponent.h"
#include "Item/Item.h"
#include "StructUtils/InstancedStruct.h"
#include "EquipmentManagerComponent.generated.h"


struct FItemInstance;
class UItemDefinition;
class UHeroManagerComponent;
class UInventoryManagerComponent;

DECLARE_MULTICAST_DELEGATE(FEquipmentManagerComponentGenericSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEquipmentChangedSignature, UEquipmentManagerComponent*, EquipmentManagerComponent, const FEquippedItem&, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UEquipmentManagerComponent : public UActorComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UEquipmentManagerComponent();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void PreNetReceive() override;

	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;

	// Called on Client + Server when an item is equipped.
	UPROPERTY(BlueprintAssignable, DisplayName = "OnItemEquipped")
	FEquipmentChangedSignature OnItemEquippedDelegate;
	// Called on Client + Server when an item is unequipped.
	UPROPERTY(BlueprintAssignable, DisplayName = "OnItemUnequipped")
	FEquipmentChangedSignature OnItemUnequippedDelegate;
	// Called after all dependencies have been met and the component is ready to equip items.
	FEquipmentManagerComponentGenericSignature OnEquipmentManagerInitializedDelegate;

	/**
	 * Tries to equip an Item at the specified EquipSlot. Will call TryUnequipItem to ensure two items are not equipped
	 * in the same slot.
	 * @param EquipSlot The slot to equip the item.
	 * @param ItemGuid The guid for the item in the owner's InventoryManager.
	 */
	UFUNCTION(BlueprintCallable, Category = "EquipmentManager")
	void TryEquipItem(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot, FGuid ItemGuid);

	/**
	 * Unequips an item from the specified equip slot.
	 * @param EquipSlot The slot to unequip the item from.
	 */
	UFUNCTION(BlueprintCallable, Category = "EquipmentManager")
	void TryUnequipItem(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot);

	/** Retrieves a copy of the ItemEquipped in slot. */
	FEquippedItem GetEquippedItem(const FGameplayTag& EquipSlot) const;
	
	/** Retrieves a pointer to an ItemInstance */
	FItemInstance* GetEquippedItemInstance(const FGameplayTag& EquipSlot) const;
	
	/**
	 * @param EquipSlot The slot to find the equipment at.
	 * @return A copy of the ItemInstance from the InventoryManager.
	 */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager", DisplayName = "GetEquippedItemInstance")
	FItemInstance K2_GetEquippedItemInstance(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot) const;

	/** Checks to see if the item can be equipped. Will check the equip requirements of the item. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool CanEquipItem(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot, UPARAM(ref) const TInstancedStruct<FItem>& Item) const;

	/**
	 * Retrieves the item from the InventoryManager and checks to see if the item can be equipped.
	 * Will check the equip requirements of the item.
	 */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool CanEquipItemByItemGuid(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot, FGuid ItemGuid) const;

	/** Returns true if items can be (un)equipped. Bind to OnEquipmentManagerInitializedDelegate to be notified when ready. */
	bool IsReadyToManageEquipment() const;
	
	/* Returns true if this Component's Owner Actor has authority. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool HasAuthority() const;

protected:
	/** Called wh en an item is equipped. */
	virtual void OnItemEquipped(const FEquippedItem& EquippedItem);
	/** Called when an item is unequipped. */
	virtual void OnItemUnequipped(const FEquippedItem& EquippedItem);

	/** Unequips items if the hero can no longer meet the equip requirements. */
	UFUNCTION()
	virtual void OnHeroClassChanged(UHeroManagerComponent* InHeroManagerComponent);

	/** When an Item is removed from the InventoryManager, unequips the item if it is currently equipped. */
	UFUNCTION()
	virtual void OnItemRemoved(UInventoryManagerComponent* InInventoryManagerComponent, const FItemInstance& ItemInstance);

private:
	UPROPERTY(Replicated)
	FEquippedItemsContainer EquippedItemsContainer;

	/** Only equipment with matching tags can be equipped. */
	UPROPERTY(EditDefaultsOnly, Category = "EquipmentManager", meta = (Categories = "EquipSlot"))
	FGameplayTagContainer AllowedEquipSlots;

	/** Cached ASC from the owner. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;

	/** Cached InventoryManagerComponent from the owner. */
	UPROPERTY()
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;

	/** Cached HeroManagerComponent from the owner. */
	UPROPERTY()
	TObjectPtr<UHeroManagerComponent> HeroManagerComponent;

	/** Cached value of whether the owner is a simulated actor. */
	UPROPERTY()
	bool bCachedIsNetSimulated = false;
	void CacheIsNetSimulated();

	friend struct FEquippedItemsContainer;
	friend struct FEquippedItem;

	void Internal_EquipItem(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance);
	/** Unequips the item at the EquipSlot. Does not find and update the ItemInstance. */
	void Internal_UnequipItem(const FGameplayTag& EquipSlot);
	/** Unequips the item and updates the ItemInstance. */
	void Internal_UnequipItem(FItemInstance* ItemInstance);

	FEquipGrantedHandle ApplyItemStats(const TInstancedStruct<FItem>& Item);
	void ClearItemStats(FEquipGrantedHandle& EquipGrantedHandle);
	void ClearItemInstanceEquipmentManager(FItemInstance* ItemInstance);

	UFUNCTION(Server, Reliable)
	void Server_TryEquipItem(FGameplayTag EquipSlot, FGuid ItemGuid);

	UFUNCTION(Server, Reliable)
	void Server_TryUnequipItem(FGameplayTag EquipSlot);
};
