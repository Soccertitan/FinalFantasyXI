// Copyright Soccertitan 2025


#include "Player/Hero/WeaponSetManagerComponent.h"

#include "CrysGameplayTags.h"
#include "Input/AbilityInputManagerComponent.h"


UWeaponSetManagerComponent::UWeaponSetManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UWeaponSetManagerComponent::InitializeWeaponSetManager(UAbilityInputManagerComponent* InAbilityInputManagerComponent, UEquipmentManagerComponent* InEquipmentManagerComponent)
{
	if (!InAbilityInputManagerComponent || !InEquipmentManagerComponent)
	{
		return;
	}

	if (AbilityInputManagerComponent)
	{
		//TODO: Unbind Delegates
	}

	if (EquipmentManagerComponent)
	{
		//TODO: Unbind Delegates
	}

	AbilityInputManagerComponent = InAbilityInputManagerComponent;
	EquipmentManagerComponent = InEquipmentManagerComponent;
}

bool UWeaponSetManagerComponent::IsWeaponSetManagerInitialized() const
{
	if (!AbilityInputManagerComponent)
	{
		return false;
	}

	if (!EquipmentManagerComponent)
	{
		return false;
	}

	return true;
}

void UWeaponSetManagerComponent::SetActiveWeaponSet(int32 Index)
{
	if (!IsWeaponSetManagerInitialized() || Index < 0)
	{
		return;
	}

	if (Index == ActiveWeaponSetIndex)
	{
		return;
	}

	if (!WeaponSets.IsValidIndex(Index))
	{
		return;
	}

	const int32 OldActiveWeaponSetIndex = ActiveWeaponSetIndex;
	ActiveWeaponSetIndex = Index;
	const FWeaponSet& ActiveWeaponSet = WeaponSets[ActiveWeaponSetIndex];

	// Updating Ability Inputs
	if (WeaponSets.IsValidIndex(OldActiveWeaponSetIndex))
	{
		AbilityInputManagerComponent->RemoveAbilityInputItemsByAbilityInputItem(WeaponSets[OldActiveWeaponSetIndex].InputItems);
	}
	AbilityInputManagerComponent->AddAbilityInputItems(ActiveWeaponSet.InputItems);

	// Equip the weapon if possible.
	const FGameplayTag& EquipSlot_Weapon = FCrysGameplayTags::Get().EquipSlot_Weapon;
	if (EquipmentManagerComponent->GetEquippedItem(EquipSlot_Weapon).ItemGuid != ActiveWeaponSet.ItemGuid)
	{
		EquipmentManagerComponent->TryUnequipItem(EquipSlot_Weapon);
		if (ActiveWeaponSet.CanEquipWeapon())
		{
			EquipmentManagerComponent->TryEquipItem(EquipSlot_Weapon, ActiveWeaponSet.ItemGuid);
		}
	}

	OnActiveWeaponSetChangedDelegate.Broadcast(ActiveWeaponSetIndex, OldActiveWeaponSetIndex);
}

FWeaponSet UWeaponSetManagerComponent::GetWeaponSetAt(const int32 Index) const
{
	if (WeaponSets.IsValidIndex(Index))
	{
		return WeaponSets[Index];
	}
	return FWeaponSet();
}

void UWeaponSetManagerComponent::AddWeaponSet(FWeaponSet& InWeaponSet, const int32 Index)
{
	if (!IsWeaponSetManagerInitialized())
	{
		return;
	}

	Internal_UpdateWeaponSet(InWeaponSet, Index);
}

void UWeaponSetManagerComponent::ClearWeaponSet(const int32 Index)
{
	if (!IsWeaponSetManagerInitialized() && Index < 0)
	{
		return;
	}

	FWeaponSet Empty;
	Internal_UpdateWeaponSet(Empty, Index);
}

void UWeaponSetManagerComponent::AddWeaponSets(TArray<FWeaponSet>& InWeaponSets)
{
	if (!IsWeaponSetManagerInitialized() || InWeaponSets.IsEmpty())
	{
		return;
	}

	WeaponSets.Reserve(WeaponSets.Num() + InWeaponSets.Num());
	for (int32 Idx = 0; Idx < InWeaponSets.Num(); Idx++)
	{
		Internal_UpdateWeaponSet(InWeaponSets[Idx], -1);
	}
}

void UWeaponSetManagerComponent::RemoveAllWeaponSets()
{
	if (IsWeaponSetManagerInitialized())
	{
		Internal_ResetWeaponSets();
	}
}

void UWeaponSetManagerComponent::UpdateEquipStatus()
{
	if (IsWeaponSetManagerInitialized())
	{
		const FGameplayTag& EquipSlot_Weapon = FCrysGameplayTags::Get().EquipSlot_Weapon;
		for (int32 Idx = 0; Idx < WeaponSets.Num(); Idx++)
		{
			FWeaponSet& WeaponSet = WeaponSets[Idx];
			bool OldStatus = WeaponSet.CanEquipWeapon();
			WeaponSet.bCanEquipWeapon = EquipmentManagerComponent->CanEquipItemByItemGuid(EquipSlot_Weapon, WeaponSet.ItemGuid);
			if (OldStatus != WeaponSet.CanEquipWeapon())
			{
				OnWeaponSetChangedDelegate.Broadcast(WeaponSet, Idx);
			}
		}
	}
}

const TArray<FWeaponSet>& UWeaponSetManagerComponent::GetWeaponSets() const
{
	return WeaponSets;
}

void UWeaponSetManagerComponent::Internal_UpdateWeaponSet(FWeaponSet& WeaponSet, int32 Index)
{
	const FGameplayTag& EquipSlot_Weapon = FCrysGameplayTags::Get().EquipSlot_Weapon;
	WeaponSet.bCanEquipWeapon = EquipmentManagerComponent->CanEquipItemByItemGuid(EquipSlot_Weapon, WeaponSet.ItemGuid);
	if (Index >= 0)
	{
		if (WeaponSets.IsValidIndex(Index))
		{
			FWeaponSet OldWeaponSet = WeaponSets[Index];
			WeaponSets[Index] = WeaponSet;

			if (ActiveWeaponSetIndex == Index)
			{
				AbilityInputManagerComponent->RemoveAbilityInputItemsByAbilityInputItem(OldWeaponSet.InputItems);	
				AbilityInputManagerComponent->AddAbilityInputItems(WeaponSet.InputItems);

				if (OldWeaponSet.ItemGuid != WeaponSet.ItemGuid)
				{
					EquipmentManagerComponent->TryUnequipItem(EquipSlot_Weapon);
				}
				if (WeaponSet.CanEquipWeapon())
				{
					EquipmentManagerComponent->TryEquipItem(EquipSlot_Weapon, WeaponSet.ItemGuid);
				}
			}
		}
		else
		{
			WeaponSets.AddDefaulted(Index);
			WeaponSets.Insert(WeaponSet, Index);
		}
	}
	else
	{
		WeaponSets.Add(WeaponSet);
		Index = WeaponSets.Num() - 1;
	}

	OnWeaponSetChangedDelegate.Broadcast(WeaponSets[Index], Index);
}

void UWeaponSetManagerComponent::Internal_ResetWeaponSets()
{
	if (WeaponSets.IsValidIndex(ActiveWeaponSetIndex))
	{
		AbilityInputManagerComponent->RemoveAbilityInputItemsByAbilityInputItem(WeaponSets[ActiveWeaponSetIndex].InputItems);	
	}
	EquipmentManagerComponent->TryUnequipItem(FCrysGameplayTags::Get().EquipSlot_Weapon);

	const int32 Num = WeaponSets.Num();
	WeaponSets.Empty();
	for (int32 Idx = 0; Idx < Num; Idx++)
	{
		OnWeaponSetChangedDelegate.Broadcast(FWeaponSet(), Idx);
	}

	const int32 OldActiveWeaponSetIndex = ActiveWeaponSetIndex;
	ActiveWeaponSetIndex = INDEX_NONE;
	OnActiveWeaponSetChangedDelegate.Broadcast(ActiveWeaponSetIndex, OldActiveWeaponSetIndex);
}

