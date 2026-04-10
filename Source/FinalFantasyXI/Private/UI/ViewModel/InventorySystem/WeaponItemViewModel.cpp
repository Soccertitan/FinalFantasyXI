// Copyright Soccertitan 2025


#include "UI/ViewModel/InventorySystem/WeaponItemViewModel.h"

#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "UI/ViewModel/CrysGameplayTagViewModel.h"


void UWeaponItemViewModel::SetDamage(FScalableFloat Value)
{
	Damage = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
}

void UWeaponItemViewModel::SetAutoAttackDelay(FScalableFloat Value)
{
	AutoAttackDelay = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemViewModel::SetWeaponSkillViewModel(UCrysGameplayTagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponSkillViewModel, Value);
}

void UWeaponItemViewModel::SetDamageTypeViewModel(UCrysGameplayTagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(DamageTypeViewModel, Value);
}

void UWeaponItemViewModel::OnItemSet(const TInstancedStruct<FItem>& Item)
{
	Super::OnItemSet(Item);
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemViewModel::OnItemDefinitionSet(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionSet(ItemDefinition);
	
	if (const FItemFragment_Weapon* Fragment = ItemDefinition->FindFragmentByType<FItemFragment_Weapon>())
	{
		SetDamage(Fragment->Damage);
		SetAutoAttackDelay(Fragment->AutoAttackDelay);
		
		UCrysGameplayTagViewModel* WeaponSkill = NewObject<UCrysGameplayTagViewModel>(this);
		WeaponSkill->SetGameplayTag(Fragment->WeaponSkill);
		SetWeaponSkillViewModel(WeaponSkill);
		
		UCrysGameplayTagViewModel* DamageType = NewObject<UCrysGameplayTagViewModel>(this);
		DamageType->SetGameplayTag(Fragment->DamageType);
		SetDamageTypeViewModel(DamageType);
	}
}
