// Copyright Soccertitan 2025


#include "EquipmentSystem/EquipmentManagerComponent.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysNativeGameplayTags.h"
#include "CrysLogChannels.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryGameplayTags.h"
#include "InventoryManagerComponent.h"
#include "InventoryTypes.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/JobAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "JobSystem/JobManagerComponent.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "System/CrysAssetManager.h"


UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	AllowedItemContainers.AddTag(FInventoryGameplayTags::Get().ItemContainer_Default);
}

void UEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquippedItemsContainer);
}

void UEquipmentManagerComponent::OnRegister()
{
	Super::OnRegister();
	CacheIsNetSimulated();
	EquippedItemsContainer.Owner = this;
}

void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheIsNetSimulated();
}

void UEquipmentManagerComponent::PreNetReceive()
{
	Super::PreNetReceive();
	CacheIsNetSimulated();
}

void UEquipmentManagerComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	AbilitySystemComponent = NewAbilitySystemComponent;
	JobManagerComponent = UJobSystemBlueprintFunctionLibrary::GetJobManagerComponent(GetOwner());
	InventoryManagerComponent = UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(GetOwner());

	if (IsReadyToManageEquipment())
	{
		JobManagerComponent->OnMainJobChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnMainJobChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UPrimaryAttributeSet::GetLevelAttribute()).AddUObject(this, &UEquipmentManagerComponent::OnLevelChanged);
		float MainJobLevel = AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute());
		FOnAttributeChangeData Data;
		Data.NewValue = MainJobLevel;
		OnLevelChanged(Data);
		InventoryManagerComponent->OnItemRemovedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemRemovedFromInventory);
		InventoryManagerComponent->OnItemChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemChangedInInventory);
		OnEquipmentManagerInitializedDelegate.Broadcast();
	}
}

void UEquipmentManagerComponent::TryEquipItem(FGameplayTag EquipSlot, const FItemInstanceHandle& Handle)
{
	if (!EquipSlot.IsValid() || !Handle.IsValid() || !InventoryManagerComponent)
	{
		return;
	}

	FItemInstance* ItemInstance = FindItem(Handle);
	if (!ItemInstance)
	{
		return;
	}

	if (!CanEquipItem(EquipSlot, ItemInstance->GetItem()))
	{
		return;
	}

	if (!HasAuthority())
	{
		Server_TryEquipItem(EquipSlot, Handle);
		return;
	}

	TryUnequipItem(EquipSlot);

	// Unequips the item from an EquipmentManagerComponent if it's already equipped there.
	FItemShard_Equipment* ItemShard_Equipment = ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>();
	if (UEquipmentManagerComponent* EquippedTo = ItemShard_Equipment->GetEquipmentManagerComponent())
	{
		EquippedTo->UnequipItemInternal(ItemInstance);
	}
	
	EquipItemInternal(EquipSlot, ItemInstance);
}

void UEquipmentManagerComponent::TryUnequipItem(FGameplayTag EquipSlot)
{
	if (!IsReadyToManageEquipment() || !EquipSlot.IsValid())
	{
		return;
	}

	if (!HasAuthority())
	{
		Server_TryUnequipItem(EquipSlot);
		return;
	}

	if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByEquipSlot(EquipSlot))
	{
		if (FItemInstance* ItemInstance = InventoryManagerComponent->FindItem(EquippedItem->ItemInstanceHandle))
		{
			UnequipItemInternal(ItemInstance);
		}
	}
}

FEquippedItem UEquipmentManagerComponent::GetEquippedItem(const FGameplayTag& EquipSlot) const
{
	if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByEquipSlot(EquipSlot))
	{
		return *EquippedItem;
	}
	return FEquippedItem();
}

FItemInstance* UEquipmentManagerComponent::GetEquippedItemInstance(const FGameplayTag& EquipSlot) const
{
	if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByEquipSlot(EquipSlot))
	{
		if (InventoryManagerComponent)
		{
			return InventoryManagerComponent->FindItem(EquippedItem->ItemInstanceHandle);
		}
	}
	return nullptr;
}

FItemInstance UEquipmentManagerComponent::K2_GetEquippedItemInstance(FGameplayTag EquipSlot) const
{
	if (FItemInstance* ItemInstance = GetEquippedItemInstance(EquipSlot))
	{
		return *ItemInstance;
	}
	return FItemInstance();
}

bool UEquipmentManagerComponent::CanEquipItem(FGameplayTag EquipSlot, const TInstancedStruct<FItem>& Item) const
{
	if (!IsReadyToManageEquipment())
	{
		return false;
	}

	if (!Item.IsValid())
	{
		return false;
	}

	const UItemDefinition* ItemDefinition = UInventoryBlueprintFunctionLibrary::GetItemDefinition(Item);
	if (!ItemDefinition)
	{
		return false;
	}

	const FItemShard_Equipment* ItemShard_Equipment = Item.Get().FindShardByType<FItemShard_Equipment>();
	if (!ItemShard_Equipment)
	{
		return false;
	}

	const FItemFragment_Equipment* ItemFragment_Equipment = ItemDefinition->FindFragmentByType<FItemFragment_Equipment>();
	if (!ItemFragment_Equipment)
	{
		return false;
	}

	if (!EquipSlot.MatchesTag(ItemFragment_Equipment->EquipSlot))
	{
		return false;
	}

	if (IsEquipSlotBlocked(EquipSlot))
	{
		return false;
	}

	// Don't allow a weapon to be equipped in the SubHand slot unless the character is allowed to Dual Wield and has
	// a weapon equipped in the MainHand slot that does not block SubHand.
	const FItemFragment_Weapon* ItemFragment_Weapon = ItemDefinition->FindFragmentByType<FItemFragment_Weapon>();
	if (ItemFragment_Weapon && EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Sub)
	{
		const bool bBlockDualWield = AbilitySystemComponent->GetGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWield) == 0;
		if (bBlockDualWield)
		{
			return false;
		}
	}

	if (ItemFragment_Equipment->LevelRequirement > 0)
	{
		bool bSuccess = false;
		const float AttributeValue = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
			AbilitySystemComponent, UJobAttributeSet::GetMainJobLevelAttribute(), 
			EGameplayModEvaluationChannel::Channel0 /** Base value. */,
			FGameplayTagContainer(),FGameplayTagContainer(), bSuccess);
		if (bSuccess == false || AttributeValue < ItemFragment_Equipment->LevelRequirement)
		{
			return false;
		}
	}

	if (ItemFragment_Equipment->Jobs.IsValid())
	{
		if (UJobDefinition* JobDefinition = JobManagerComponent->GetMainJob())
		{
			if (!ItemFragment_Equipment->Jobs.HasTag(JobDefinition->JobTag))
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool UEquipmentManagerComponent::IsEquipSlotBlocked(const FGameplayTag EquipSlot) const
{
	if (!EquipSlot.IsValid())
	{
		return true;
	}

	// Don't allow equipping the item if the slot is blocked by a currently equipped item.
	for (const FEquippedItem& EquippedItem : EquippedItemsContainer.Items)
	{
		if (EquippedItem.BlockedEquipSlots.HasTag(EquipSlot))
		{
			return true;
		}
	}

	return false;
}

bool UEquipmentManagerComponent::CanEquipItemByHandle(FGameplayTag EquipSlot, const FItemInstanceHandle& Handle) const
{
	if (IsReadyToManageEquipment())
	{
		if (FItemInstance* ItemInstance = FindItem(Handle))
		{
			return CanEquipItem(EquipSlot, ItemInstance->GetItem());
		}
	}

	return false;
}

bool UEquipmentManagerComponent::IsReadyToManageEquipment() const
{
	if (!AbilitySystemComponent)
	{
		return false;
	}
	if (!JobManagerComponent)
	{
		return false;
	}
	if (!InventoryManagerComponent)
	{
		return false;
	}
	return true;
}

bool UEquipmentManagerComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UEquipmentManagerComponent::OnItemEquipped(const FEquippedItem& EquippedItem)
{
	OnItemEquippedDelegate.Broadcast(EquippedItem);
}

void UEquipmentManagerComponent::OnItemUnequipped(const FEquippedItem& EquippedItem)
{
	OnItemUnequippedDelegate.Broadcast(EquippedItem);
}

void UEquipmentManagerComponent::OnMainJobChanged()
{
	if (HasAuthority() && IsReadyToManageEquipment())
	{
		for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
		{
			FEquippedItem& EquippedItem = EquippedItemsContainer.Items[idx];
			if (FItemInstance* ItemInstance = FindItem(EquippedItem.ItemInstanceHandle))
			{
				if (!CanEquipItem(EquippedItem.EquipSlot, ItemInstance->GetItem()))
				{
					UnequipItemInternal(ItemInstance);
				}
			}
		}
	}
}

void UEquipmentManagerComponent::OnItemRemovedFromInventory(const FItemInstance& ItemInstance)
{
	if (HasAuthority() && IsReadyToManageEquipment())
	{
		if (const FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByItemGuid(ItemInstance.GetGuid()))
		{
			if (UItemContainer* MovedToItemContainer = ItemInstance.GetMovedToItemContainer())
			{
				if (FItemInstance* EquippedItemInstance = MovedToItemContainer->FindItemByGuid(ItemInstance.GetGuid()))
				{
					UnequipItemInternal(EquippedItemInstance);
				}
			}
			else
			{
				UnequipItemInternal(EquippedItem->EquipSlot);
			}
		}
	}
}

void UEquipmentManagerComponent::OnItemChangedInInventory(const FItemInstance& ItemInstance)
{
	if (HasAuthority())
	{
		if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByItemGuid(ItemInstance.GetGuid()))
		{
			int32 ItemLevel = ItemInstance.GetItem().Get().FindShardByType<FItemShard_Equipment>()->Level;
			AbilitySystemComponent->SetActiveGameplayEffectLevel(EquippedItem->GameplayEffectHandle, ItemLevel);
			
			if (EquippedItem->WeaponData.IsValid())
			{
				EquippedItem->WeaponData.Level = ItemLevel;
				EquippedItemsContainer.MarkItemDirty(*EquippedItem);
				ApplyBaseAttackDelay();
			}
		}
	}
}

void UEquipmentManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UEquipmentManagerComponent::OnLevelChanged(const FOnAttributeChangeData& Data)
{
	BareHandedWeaponData.Level = Data.NewValue;
	
	if (HasAuthority())
	{
		const FEquippedItem* MainHand = EquippedItemsContainer.FindItemByEquipSlot(Crys::NativeGameplayTag::EquipSlot_Hand_Main);
		if (!MainHand || !MainHand->WeaponData.IsValid())
		{
			ApplyBaseAttackDelay();
		}
	}
}

FItemInstance* UEquipmentManagerComponent::FindItem(const FItemInstanceHandle& Handle) const
{
	if (Handle.IsValid() && InventoryManagerComponent == Handle.GetItemContainer()->GetInventoryManagerComponent())
	{
		if (AllowedItemContainers.IsEmpty())
		{
			return InventoryManagerComponent->FindItem(Handle);
		}

		UItemContainer* ItemContainer = Handle.GetItemContainer();
		if (AllowedItemContainers.HasTag(ItemContainer->GetItemContainerTag()))
		{
			return InventoryManagerComponent->FindItem(Handle);
		}
	}
	
	return nullptr;
}

void UEquipmentManagerComponent::EquipItemInternal(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance)
{
	FEquippedItem& NewEquippedItem = EquippedItemsContainer.Items.AddDefaulted_GetRef();
	NewEquippedItem.ItemInstanceHandle = FItemInstanceHandle(*ItemInstance);
	NewEquippedItem.EquipSlot = EquipSlot;
	NewEquippedItem.GameplayEffectHandle = ApplyEquipmentGameplayEffect(ItemInstance->GetItem());
	NewEquippedItem.BlockedEquipSlots = UInventoryBlueprintFunctionLibrary::GetItemDefinition(ItemInstance->GetItem())->FindFragmentByType<FItemFragment_Equipment>()->BlockEquipSlots;
	
	TryInitWeapon(ItemInstance, NewEquippedItem);
	
	ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>()->EquipmentManagerComponent = this;
	ItemInstance->MarkItemDirty();
	
	OnItemEquipped(NewEquippedItem);
	EquippedItemsContainer.MarkItemDirty(NewEquippedItem);
}

void UEquipmentManagerComponent::UnequipItemInternal(FItemInstance* ItemInstance)
{
	for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
	{
		FEquippedItem TempItem = EquippedItemsContainer.Items[idx];
		if (TempItem.ItemInstanceHandle.GetGuid() == ItemInstance->GetGuid())
		{
			EquippedItemsContainer.Items.RemoveAt(idx);
			
			AbilitySystemComponent->RemoveActiveGameplayEffect(TempItem.GameplayEffectHandle);
			ClearEquipmentManagerFromItemInstance(ItemInstance);
			TryDeinitWeapon(TempItem);
			OnItemUnequipped(TempItem);
			EquippedItemsContainer.MarkArrayDirty();
			return;
		}
	}
}

void UEquipmentManagerComponent::UnequipItemInternal(const FGameplayTag& EquipSlot)
{
	for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
	{
		FEquippedItem TempItem = EquippedItemsContainer.Items[idx];
		if (TempItem.EquipSlot == EquipSlot)
		{
			EquippedItemsContainer.Items.RemoveAt(idx);
			
			AbilitySystemComponent->RemoveActiveGameplayEffect(TempItem.GameplayEffectHandle);
			TryDeinitWeapon(TempItem);
			OnItemUnequipped(TempItem);
			EquippedItemsContainer.MarkArrayDirty();
			return;
		}
	}
}

FActiveGameplayEffectHandle UEquipmentManagerComponent::ApplyEquipmentGameplayEffect(const TInstancedStruct<FItem>& Item)
{
	const FItemShard_Equipment* ItemShard_Equipment = Item.Get().FindShardByType<FItemShard_Equipment>();
	const UItemDefinition* ItemDefinition = UInventoryBlueprintFunctionLibrary::GetItemDefinition(Item);
	const FItemFragment_Equipment* ItemFragment_Equipment = ItemDefinition->FindFragmentByType<FItemFragment_Equipment>();

	FActiveGameplayEffectHandle Result;

	const TSubclassOf<UGameplayEffect> EquipmentGE = ItemFragment_Equipment->GameplayEffect;
	if (!ItemFragment_Equipment->GameplayEffect)
	{
		UE_LOG(LogCrys, Warning, TEXT("GameplayEffect is invalid in [%s]"), *GetNameSafe(ItemDefinition));
		return Result;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(EquipmentGE, ItemShard_Equipment->Level, ContextHandle);

	if (Spec.IsValid())
	{
		Result = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	return Result;
}

void UEquipmentManagerComponent::ClearEquipmentManagerFromItemInstance(FItemInstance* ItemInstance)
{
	FItemShard_Equipment* ItemShard = ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>();
	if (ItemShard->GetEquipmentManagerComponent() == this)
	{
		ItemShard->EquipmentManagerComponent = nullptr;
		ItemInstance->MarkItemDirty();
	}
}

void UEquipmentManagerComponent::TryInitWeapon(const FItemInstance* ItemInstance, FEquippedItem& EquippedItem)
{
	const FItemFragment_Weapon* WeaponFragment = UInventoryBlueprintFunctionLibrary::GetItemDefinition(ItemInstance->GetItem())->FindFragmentByType<FItemFragment_Weapon>();
	if ((EquippedItem.EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Main || EquippedItem.EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Sub) && WeaponFragment)
	{
		EquippedItem.WeaponData.Level = ItemInstance->GetItem().Get<FItem>().FindShardByType<FItemShard_Equipment>()->Level;
		EquippedItem.WeaponData.AutoAttackDelay = WeaponFragment->AutoAttackDelay;
		EquippedItem.WeaponData.Damage = WeaponFragment->Damage;
		EquippedItem.WeaponData.Range = WeaponFragment->Range;
		EquippedItem.WeaponData.WeaponSkill = WeaponFragment->WeaponSkill;
		if (!WeaponFragment->AutoAttackGameplayEffectClass.Get())
		{
			EquippedItem.WeaponData.AutoAttackGameplayEffectClass = UCrysAssetManager::Get().GetSubclass(WeaponFragment->AutoAttackGameplayEffectClass, false);
		}
		else
		{
			EquippedItem.WeaponData.AutoAttackGameplayEffectClass = WeaponFragment->AutoAttackGameplayEffectClass.Get();
		}
		
		if (EquippedItem.EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Sub)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWielding, 1, EGameplayTagReplicationState::TagOnly);
		}
		
		ApplyBaseAttackDelay();
	}
}

void UEquipmentManagerComponent::TryDeinitWeapon(const FEquippedItem& EquippedItem)
{
	if (EquippedItem.WeaponData.IsValid())
	{
		if (EquippedItem.EquipSlot == Crys::NativeGameplayTag::EquipSlot_Hand_Sub)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(Crys::NativeGameplayTag::Ability_State_DualWielding, 0, EGameplayTagReplicationState::TagOnly);
		}
		ApplyBaseAttackDelay();
	}
}

void UEquipmentManagerComponent::ApplyBaseAttackDelay()
{
	float AutoAttackDelay = BareHandedWeaponData.AutoAttackDelay.GetValueAtLevel(BareHandedWeaponData.Level);
	if (const FEquippedItem* MainHandWeapon = EquippedItemsContainer.FindItemByEquipSlot(Crys::NativeGameplayTag::EquipSlot_Hand_Main))
	{
		AutoAttackDelay = MainHandWeapon->WeaponData.AutoAttackDelay.GetValueAtLevel(MainHandWeapon->WeaponData.Level);
	}
	
	if (const FEquippedItem* SubHandWeapon = EquippedItemsContainer.FindItemByEquipSlot(Crys::NativeGameplayTag::EquipSlot_Hand_Sub))
	{
		AutoAttackDelay += SubHandWeapon->WeaponData.AutoAttackDelay.GetValueAtLevel(SubHandWeapon->WeaponData.Level);
	}

	UGameplayEffect* AttackDelayGE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseAutoAttackDelay")));
	AttackDelayGE->DurationPolicy = EGameplayEffectDurationType::Instant;

	int32 Idx = AttackDelayGE->Modifiers.Num();
	AttackDelayGE->Modifiers.SetNum(Idx + 1);

	FGameplayModifierInfo& InfoMaxHP = AttackDelayGE->Modifiers[Idx];
	InfoMaxHP.ModifierMagnitude = FScalableFloat(AutoAttackDelay);
	InfoMaxHP.ModifierOp = EGameplayModOp::Override;
	InfoMaxHP.Attribute = UAttackerAttributeSet::GetAutoAttackDelayAttribute();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(AttackDelayGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void UEquipmentManagerComponent::Server_TryEquipItem_Implementation(FGameplayTag EquipSlot, const FItemInstanceHandle& Handle)
{
	TryEquipItem(EquipSlot, Handle);
}

void UEquipmentManagerComponent::Server_TryUnequipItem_Implementation(FGameplayTag EquipSlot)
{
	TryUnequipItem(EquipSlot);
}
