// Copyright Soccertitan 2025


#include "EquipmentSystem/EquipmentManagerComponent.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysGameplayTags.h"
#include "CrysLogChannels.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryManagerComponent.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/HeroJobAttributeSet.h"
#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "System/CrysAssetManager.h"


UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
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
	HeroManagerComponent = UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(GetOwner());
	InventoryManagerComponent = UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(GetOwner());

	if (IsReadyToManageEquipment())
	{
		HeroManagerComponent->OnHeroMainJobChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnHeroJobChanged);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UHeroJobAttributeSet::GetMainJobLevelAttribute()).AddUObject(this, &UEquipmentManagerComponent::OnHeroMainJobLevelChanged);
		float MainJobLevel = AbilitySystemComponent->GetNumericAttribute(UHeroJobAttributeSet::GetMainJobLevelAttribute());
		FOnAttributeChangeData Data;
		Data.NewValue = MainJobLevel;
		OnHeroMainJobLevelChanged(Data);
		InventoryManagerComponent->OnItemRemovedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemRemovedFromInventory);
		InventoryManagerComponent->OnItemChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemChangedInInventory);
		OnEquipmentManagerInitializedDelegate.Broadcast();
	}
}

void UEquipmentManagerComponent::TryEquipItem(FGameplayTag EquipSlot, FGuid ItemGuid)
{
	if (!EquipSlot.IsValid() || !ItemGuid.IsValid() || !InventoryManagerComponent)
	{
		return;
	}

	FItemInstance* ItemInstance = FindItemByGuid(ItemGuid);
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
		Server_TryEquipItem(EquipSlot, ItemGuid);
		return;
	}

	TryUnequipItem(EquipSlot);

	// Unequips the item from an EquipmentManagerComponent if it's already equipped there.
	FItemShard_Equipment* ItemShard_Equipment = ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>();
	if (UEquipmentManagerComponent* EquippedTo = ItemShard_Equipment->GetEquipmentManagerComponent())
	{
		EquippedTo->Internal_UnequipItem(ItemInstance);
	}
	
	Internal_EquipItem(EquipSlot, ItemInstance);
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
		if (FItemInstance* ItemInstance = InventoryManagerComponent->FindItemByGuid(EquippedItem->ItemGuid))
		{
			Internal_UnequipItem(ItemInstance);
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
			return InventoryManagerComponent->FindItemByGuid(EquippedItem->ItemGuid);
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

	if (!ItemFragment_Equipment->EquipSlots.HasTagExact(EquipSlot))
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
	if (ItemFragment_Weapon && EquipSlot == FCrysGameplayTags::Get().EquipSlot_SubHand)
	{
		const bool bBlockDualWield = AbilitySystemComponent->GetGameplayTagCount(FCrysGameplayTags::Get().Gameplay_State_DualWieldAllowed) == 0;
		if (bBlockDualWield)
		{
			return false;
		}
	}

	if (ItemFragment_Equipment->LevelRequirement > 0)
	{
		bool bSuccess = false;
		const float AttributeValue = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
			AbilitySystemComponent, UHeroJobAttributeSet::GetMainJobLevelAttribute(), 
			EGameplayModEvaluationChannel::Channel0 /** Base value. */,
			FGameplayTagContainer(),FGameplayTagContainer(), bSuccess);
		if (bSuccess == false || AttributeValue < ItemFragment_Equipment->LevelRequirement)
		{
			return false;
		}
	}

	if (ItemFragment_Equipment->HeroJobs.IsValid())
	{
		if (UHeroJobDefinition* HeroClass = HeroManagerComponent->GetHeroMainJob())
		{
			if (!ItemFragment_Equipment->HeroJobs.HasTag(HeroClass->JobTag))
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

bool UEquipmentManagerComponent::CanEquipItemByItemGuid(FGameplayTag EquipSlot, FGuid ItemGuid) const
{
	if (IsReadyToManageEquipment())
	{
		if (FItemInstance* ItemInstance = FindItemByGuid(ItemGuid))
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
	if (!HeroManagerComponent)
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

void UEquipmentManagerComponent::OnHeroJobChanged()
{
	if (HasAuthority() && IsReadyToManageEquipment())
	{
		for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
		{
			FEquippedItem& EquippedItem = EquippedItemsContainer.Items[idx];
			if (FItemInstance* ItemInstance = FindItemByGuid(EquippedItem.ItemGuid))
			{
				if (!CanEquipItem(EquippedItem.EquipSlot, ItemInstance->GetItem()))
				{
					Internal_UnequipItem(ItemInstance);
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
					Internal_UnequipItem(EquippedItemInstance);
				}
			}
			else
			{
				Internal_UnequipItem(EquippedItem->EquipSlot);
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

void UEquipmentManagerComponent::OnHeroMainJobLevelChanged(const FOnAttributeChangeData& Data)
{
	BareHandedWeaponData.Level = Data.NewValue;
	
	if (HasAuthority())
	{
		const FEquippedItem* MainHand = EquippedItemsContainer.FindItemByEquipSlot(FCrysGameplayTags::Get().EquipSlot_MainHand);
		if (!MainHand || !MainHand->WeaponData.IsValid())
		{
			ApplyBaseAttackDelay();
		}
	}
}

FItemInstance* UEquipmentManagerComponent::FindItemByGuid(const FGuid& ItemGuid) const
{
	if (AllowedItemContainers.IsEmpty())
	{
		return InventoryManagerComponent->FindItemByGuid(ItemGuid);
	}
	
	for (const FGameplayTag& ItemContainerTag : AllowedItemContainers)
	{
		if (UItemContainer* ItemContainer = InventoryManagerComponent->FindItemContainerByTag(ItemContainerTag))
		{
			if (FItemInstance* ItemInstance = ItemContainer->FindItemByGuid(ItemGuid))
			{
				return ItemInstance;
			}
		}
	}
	
	return nullptr;
}

void UEquipmentManagerComponent::Internal_EquipItem(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance)
{
	FEquippedItem& NewEquippedItem = EquippedItemsContainer.Items.AddDefaulted_GetRef();
	NewEquippedItem.ItemGuid = ItemInstance->GetGuid();
	NewEquippedItem.EquipSlot = EquipSlot;
	NewEquippedItem.GameplayEffectHandle = ApplyEquipmentGameplayEffect(ItemInstance->GetItem());
	NewEquippedItem.BlockedEquipSlots = UInventoryBlueprintFunctionLibrary::GetItemDefinition(ItemInstance->GetItem())->FindFragmentByType<FItemFragment_Equipment>()->BlockEquipSlots;
	
	TryInitWeapon(ItemInstance, NewEquippedItem);
	
	ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>()->EquipmentManagerComponent = this;
	ItemInstance->MarkItemDirty();
	
	OnItemEquipped(NewEquippedItem);
	EquippedItemsContainer.MarkItemDirty(NewEquippedItem);
}

void UEquipmentManagerComponent::Internal_UnequipItem(FItemInstance* ItemInstance)
{
	for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
	{
		FEquippedItem TempItem = EquippedItemsContainer.Items[idx];
		if (TempItem.ItemGuid == ItemInstance->GetGuid())
		{
			EquippedItemsContainer.Items.RemoveAt(idx);
			
			AbilitySystemComponent->RemoveActiveGameplayEffect(TempItem.GameplayEffectHandle);
			ClearItemInstanceEquipmentManager(ItemInstance);
			TryDeinitWeapon(TempItem);
			OnItemUnequipped(TempItem);
			EquippedItemsContainer.MarkArrayDirty();
			return;
		}
	}
}

void UEquipmentManagerComponent::Internal_UnequipItem(const FGameplayTag& EquipSlot)
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

void UEquipmentManagerComponent::ClearItemInstanceEquipmentManager(FItemInstance* ItemInstance)
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
	if ((EquippedItem.EquipSlot == FCrysGameplayTags::Get().EquipSlot_MainHand || EquippedItem.EquipSlot == FCrysGameplayTags::Get().EquipSlot_SubHand) && WeaponFragment)
	{
		EquippedItem.WeaponData.Level = ItemInstance->GetItem().Get<FItem>().FindShardByType<FItemShard_Equipment>()->Level;
		EquippedItem.WeaponData.Delay = WeaponFragment->Delay;
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
		
		if (EquippedItem.EquipSlot == FCrysGameplayTags::Get().EquipSlot_SubHand)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(FCrysGameplayTags::Get().Gameplay_State_DualWielding, 1, EGameplayTagReplicationState::TagOnly);
		}
		
		ApplyBaseAttackDelay();
	}
}

void UEquipmentManagerComponent::TryDeinitWeapon(const FEquippedItem& EquippedItem)
{
	if (EquippedItem.WeaponData.IsValid())
	{
		if (EquippedItem.EquipSlot == FCrysGameplayTags::Get().EquipSlot_SubHand)
		{
			AbilitySystemComponent->SetLooseGameplayTagCount(FCrysGameplayTags::Get().Gameplay_State_DualWielding, 0, EGameplayTagReplicationState::TagOnly);
		}
		ApplyBaseAttackDelay();
	}
}

void UEquipmentManagerComponent::ApplyBaseAttackDelay()
{
	float AutoAttackDelay = BareHandedWeaponData.Delay.GetValueAtLevel(BareHandedWeaponData.Level);
	if (const FEquippedItem* MainHandWeapon = EquippedItemsContainer.FindItemByEquipSlot(FCrysGameplayTags::Get().EquipSlot_MainHand))
	{
		AutoAttackDelay = MainHandWeapon->WeaponData.Delay.GetValueAtLevel(MainHandWeapon->WeaponData.Level);
	}
	
	if (const FEquippedItem* SubHandWeapon = EquippedItemsContainer.FindItemByEquipSlot(FCrysGameplayTags::Get().EquipSlot_SubHand))
	{
		AutoAttackDelay += SubHandWeapon->WeaponData.Delay.GetValueAtLevel(SubHandWeapon->WeaponData.Level);
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

void UEquipmentManagerComponent::Server_TryEquipItem_Implementation(FGameplayTag EquipSlot, FGuid ItemGuid)
{
	TryEquipItem(EquipSlot, ItemGuid);
}

void UEquipmentManagerComponent::Server_TryUnequipItem_Implementation(FGameplayTag EquipSlot)
{
	TryUnequipItem(EquipSlot);
}
