// Copyright Soccertitan 2025


#include "EquipmentSystem/EquipmentManagerComponent.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysGameplayTags.h"
#include "CrysLogChannels.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryManagerComponent.h"
#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Settings/FinalFantasyXIGameData.h"
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
		// HeroManagerComponent->OnHeroClassChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnHeroClassChanged);
		InventoryManagerComponent->OnItemRemovedDelegate.AddUniqueDynamic(this, &UEquipmentManagerComponent::OnItemRemoved);
		OnEquipmentManagerInitializedDelegate.Broadcast();
	}
}

void UEquipmentManagerComponent::TryEquipItem(FGameplayTag EquipSlot, FGuid ItemGuid)
{
	if (!IsReadyToManageEquipment() || !EquipSlot.IsValid() || !ItemGuid.IsValid())
	{
		return;
	}

	FItemInstance* ItemInstance = InventoryManagerComponent->FindItemByGuid(ItemGuid);
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

	// Unequips the item from the EquipmentManagerComponent if it's already equipped.
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
		else
		{
			Internal_UnequipItem(EquipSlot);
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

	if (!Item.IsValid() || !EquipSlot.MatchesTag(FCrysGameplayTags::Get().EquipSlot))
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

	if (!ItemFragment_Equipment->AllowedEquipSlots.HasTag(EquipSlot))
	{
		return false;
	}

	if (ItemFragment_Equipment->EquipRequirement.AttributeTag.IsValid())
	{
		FAttributeRelationshipItem AttributeItem = UCrysBlueprintFunctionLibrary::FindAttributeRelationshipItem(ItemFragment_Equipment->EquipRequirement.AttributeTag, true);
		if (AttributeItem.IsValid())
		{
			if (!AbilitySystemComponent->HasAttributeSetForAttribute(AttributeItem.GameplayAttribute))
			{
				return false;
			}
			bool bSuccess = false;
			const float AttributeValue = UCrimAbilitySystemBlueprintFunctionLibrary::EvaluateAttributeValueWithTagsUpToChannel(
				AbilitySystemComponent, AttributeItem.GameplayAttribute, EGameplayModEvaluationChannel::Channel0,
				FGameplayTagContainer(),FGameplayTagContainer(), bSuccess);
			if (bSuccess == false || AttributeValue < ItemFragment_Equipment->EquipRequirement.BaseValue)
			{
				return false;
			}
		}
	}

	if (ItemFragment_Equipment->EquipRequirement.HeroClassRestrictions.IsValid())
	{
		if (UHeroJobDefinition* HeroClass = HeroManagerComponent->GetHeroMainJob())
		{
			if (!ItemFragment_Equipment->EquipRequirement.HeroClassRestrictions.HasTag(HeroClass->JobTag))
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

bool UEquipmentManagerComponent::CanEquipItemByItemGuid(FGameplayTag EquipSlot, FGuid ItemGuid) const
{
	if (IsReadyToManageEquipment())
	{
		if (FItemInstance* ItemInstance = InventoryManagerComponent->FindItemByGuid(ItemGuid))
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
	OnItemEquippedDelegate.Broadcast(this, EquippedItem);
}

void UEquipmentManagerComponent::OnItemUnequipped(const FEquippedItem& EquippedItem)
{
	OnItemUnequippedDelegate.Broadcast(this, EquippedItem);
}

void UEquipmentManagerComponent::OnHeroClassChanged(UHeroManagerComponent* InHeroManagerComponent)
{
	if (HasAuthority() && IsReadyToManageEquipment())
	{
		for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
		{
			FEquippedItem& EquippedItem = EquippedItemsContainer.Items[idx];
			if (FItemInstance* ItemInstance = InventoryManagerComponent->FindItemByGuid(EquippedItem.ItemGuid))
			{
				if (!CanEquipItem(EquippedItem.EquipSlot, ItemInstance->GetItem()))
				{
					Internal_UnequipItem(ItemInstance);
				}
			}
			else
			{
				Internal_UnequipItem(EquippedItem.EquipSlot);
			}
		}
	}
}

void UEquipmentManagerComponent::OnItemRemoved(UInventoryManagerComponent* InInventoryManagerComponent, const FItemInstance& ItemInstance)
{
	if (HasAuthority() && IsReadyToManageEquipment())
	{
		if (FEquippedItem* EquippedItem = EquippedItemsContainer.FindItemByItemGuid(ItemInstance.GetGuid()))
		{
			if (UItemContainer* MovedToItemContainer = ItemInstance.GetMovedToItemContainer())
			{
				if (FItemInstance* EquippedItemInstance = MovedToItemContainer->FindItemByGuid(ItemInstance.GetGuid()))
				{
					Internal_UnequipItem(EquippedItemInstance);
					return;
				}
			}
			
			Internal_UnequipItem(EquippedItem->EquipSlot);
		}
	}
}

void UEquipmentManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UEquipmentManagerComponent::Internal_EquipItem(const FGameplayTag& EquipSlot, FItemInstance* ItemInstance)
{
	FEquippedItem& NewEquippedItem = EquippedItemsContainer.Items.AddDefaulted_GetRef();
	NewEquippedItem.ItemGuid = ItemInstance->GetGuid();
	NewEquippedItem.EquipSlot = EquipSlot;
	NewEquippedItem.EquipGrantedHandle = ApplyItemStats(ItemInstance->GetItem());
	ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>()->EquipmentManagerComponent = this;
	ItemInstance->GetItemContainer()->MarkItemDirty(*ItemInstance);
	OnItemEquipped(NewEquippedItem);
	EquippedItemsContainer.MarkItemDirty(NewEquippedItem);
}

void UEquipmentManagerComponent::Internal_UnequipItem(const FGameplayTag& EquipSlot)
{
	for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
	{
		FEquippedItem TempItem = EquippedItemsContainer.Items[idx];
		if (TempItem.EquipSlot == EquipSlot)
		{
			ClearItemStats(TempItem.EquipGrantedHandle);
			
			EquippedItemsContainer.Items.RemoveAt(idx);
			OnItemUnequipped(TempItem);
			EquippedItemsContainer.MarkArrayDirty();
		}
	}
}

void UEquipmentManagerComponent::Internal_UnequipItem(FItemInstance* ItemInstance)
{
	for (int32 idx = EquippedItemsContainer.Items.Num() - 1; idx >= 0; idx--)
	{
		FEquippedItem TempItem = EquippedItemsContainer.Items[idx];
		if (TempItem.ItemGuid == ItemInstance->GetGuid())
		{
			ClearItemStats(TempItem.EquipGrantedHandle);
			ClearItemInstanceEquipmentManager(ItemInstance);
			
			EquippedItemsContainer.Items.RemoveAt(idx);
			OnItemUnequipped(TempItem);
			EquippedItemsContainer.MarkArrayDirty();
		}
	}
}

FEquipGrantedHandle UEquipmentManagerComponent::ApplyItemStats(const TInstancedStruct<FItem>& Item)
{
	const FItemShard_Equipment* ItemShard_Equipment = Item.Get().FindShardByType<FItemShard_Equipment>();
	const FItemFragment_Equipment* ItemFragment_Equipment = UInventoryBlueprintFunctionLibrary::GetItemDefinition(Item)->
		FindFragmentByType<FItemFragment_Equipment>();

	FEquipGrantedHandle Result;

	const TSubclassOf<UGameplayEffect> EquipmentGE;// = UCrysAssetManager::GetSubclass(GetDefault<UFinalFantasyXIGameData>()->EquipmentGameplayEffect);
	if (!EquipmentGE)
	{
		UE_LOG(LogCrys, Error, TEXT("EquipmentGameplayEffect is invalid in [%s]"), *GetDefault<UFinalFantasyXIGameData>()->GetName());
		return Result;
	}

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(EquipmentGE, 0.f, ContextHandle);

	if (Spec.IsValid())
	{
		for (const FEquipmentAttribute& Map : ItemFragment_Equipment->AttributeValues)
		{
			Spec.Data.Get()->SetSetByCallerMagnitude(Map.AttributeTag,
				FMath::Floor(Map.ScalableFloat.GetValueAtLevel(ItemShard_Equipment->GrindLevel)));
		}

		Spec.Data.Get()->AppendDynamicAssetTags(ItemFragment_Equipment->DynamicTags);

		Result.GameplayEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	if (!ItemFragment_Equipment->AbilitySet.IsNull())
	{
		if (!ItemFragment_Equipment->AbilitySet.Get())
		{
			UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
				ItemFragment_Equipment->AbilitySet.ToSoftObjectPath())->WaitUntilComplete();
		}
		if (const UAbilitySet* AbilitySet = ItemFragment_Equipment->AbilitySet.Get())
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &Result.AbilitySet_GrantedHandles);
		}
	}
	return Result;
}

void UEquipmentManagerComponent::ClearItemStats(FEquipGrantedHandle& EquipGrantedHandle)
{
	AbilitySystemComponent->RemoveActiveGameplayEffect(EquipGrantedHandle.GameplayEffectHandle);
	EquipGrantedHandle.AbilitySet_GrantedHandles.TakeFromAbilitySystem(AbilitySystemComponent);
}

void UEquipmentManagerComponent::ClearItemInstanceEquipmentManager(FItemInstance* ItemInstance)
{
	FItemShard_Equipment* ItemShard = ItemInstance->GetItemPtr()->GetMutablePtr<FItem>()->FindMutableShardByType<FItemShard_Equipment>();
	if (ItemShard->GetEquipmentManagerComponent() == this)
	{
		ItemShard->EquipmentManagerComponent = nullptr;
		ItemInstance->GetItemContainer()->MarkItemDirty(*ItemInstance);
	}
}

void UEquipmentManagerComponent::Server_TryEquipItem_Implementation(FGameplayTag EquipSlot, FGuid ItemGuid)
{
	TryEquipItem(EquipSlot, ItemGuid);
}

void UEquipmentManagerComponent::Server_TryUnequipItem_Implementation(FGameplayTag EquipSlot)
{
	TryUnequipItem(EquipSlot);
}
