// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "EquipmentTypes.h"
#include "Components/ActorComponent.h"
#include "Item/Item.h"
#include "StructUtils/InstancedStruct.h"
#include "EquipmentManagerComponent.generated.h"


struct FOnAttributeChangeData;
struct FItemInstance;
class UItemDefinition;
class UHeroManagerComponent;
class UInventoryManagerComponent;

DECLARE_MULTICAST_DELEGATE(FEquipmentManagerComponentGenericSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipmentChangedSignature, const FEquippedItem&, Item);

/** Equip/Unequips items and sets the base AutoAttackDelay for equipped weapons. */
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
	virtual bool CanEquipItem(UPARAM(meta=(Categories="EquipSlot")) FGameplayTag EquipSlot, UPARAM(ref) const TInstancedStruct<FItem>& Item) const;
	
	/** Returns true if an item is blocking a slot from having an item equipped. */
	UFUNCTION(BlueprintPure, Category = "EquipmentManager")
	bool IsEquipSlotBlocked(UPARAM(meta=(Categories="EquipSlot")) const FGameplayTag EquipSlot) const;

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
	/** Called when an item is equipped. */
	virtual void OnItemEquipped(const FEquippedItem& EquippedItem);
	/** Called when an item is unequipped. */
	virtual void OnItemUnequipped(const FEquippedItem& EquippedItem);

	/** Unequips items if the hero can no longer meet the equip requirements. */
	UFUNCTION()
	virtual void OnHeroJobChanged();

	/** When an Item is removed from the InventoryManager, unequips the item if it is currently equipped. */
	UFUNCTION()
	virtual void OnItemRemovedFromInventory(const FItemInstance& ItemInstance);
	UFUNCTION()
	virtual void OnItemChangedInInventory(const FItemInstance& ItemInstance);

private:
	/** If empty, will check all ItemContainers in the Inventory Manager.*/
	UPROPERTY(EditAnywhere, meta = (Categories = "ItemContainer", NoElementDuplicate))
	TArray<FGameplayTag> AllowedItemContainers;
	
	/** WeaponData to use when no weapon is equipped in the MainHand. Scales with character level. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FWeaponData BareHandedWeaponData;

	UPROPERTY(Replicated)
	FEquippedItemsContainer EquippedItemsContainer;

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
	
	void OnHeroMainJobLevelChanged(const FOnAttributeChangeData& Data);

	friend struct FEquippedItemsContainer;
	friend struct FEquippedItem;
	
	/** Checks the allowed ItemContainers for the ItemInstance. */
	FItemInstance* FindItemByGuid(const FGuid& ItemGuid) const;

	void Internal_EquipItem(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance);
	/** Unequips the item and updates the ItemInstance. */
	void Internal_UnequipItem(FItemInstance* ItemInstance);
	void Internal_UnequipItem(const FGameplayTag& EquipSlot);

	FActiveGameplayEffectHandle ApplyEquipmentGameplayEffect(const TInstancedStruct<FItem>& Item);
	void ClearItemInstanceEquipmentManager(FItemInstance* ItemInstance);
	
	/** If the EquippedItem is a weapon, sets up the EquippedItem with the weapon data from the ItemInstance. */
	void TryInitWeapon(const FItemInstance* ItemInstance, FEquippedItem& EquippedItem);
	
	/** If the unequipped item is a weapon, refresh the AutoAttack and if applicable remove the DualWield tag. */
	void TryDeinitWeapon(const FEquippedItem& EquippedItem);
	
	/** 
	 * Gets the currently equipped weapons WeaponData and adds the delays together. If nothing is equipped in MainHand,
	 * uses the BareHandedWeaponData.
	 */
	void ApplyBaseAttackDelay();

	UFUNCTION(Server, Reliable)
	void Server_TryEquipItem(FGameplayTag EquipSlot, FGuid ItemGuid);

	UFUNCTION(Server, Reliable)
	void Server_TryUnequipItem(FGameplayTag EquipSlot);
};
