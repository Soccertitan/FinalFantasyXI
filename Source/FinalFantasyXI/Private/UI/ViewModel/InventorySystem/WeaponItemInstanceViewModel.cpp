// Copyright Soccertitan 2025


#include "UI/ViewModel/InventorySystem/WeaponItemInstanceViewModel.h"

#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "UI/ViewModel/CrysGameplayTagViewModel.h"


void UWeaponItemInstanceViewModel::SetDamage(FScalableFloat Value)
{
	Damage = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
}

void UWeaponItemInstanceViewModel::SetAutoAttackDelay(FScalableFloat Value)
{
	AutoAttackDelay = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemInstanceViewModel::SetWeaponSkillViewModel(UCrysGameplayTagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponSkillViewModel, Value);
}

void UWeaponItemInstanceViewModel::SetDamageTypeViewModel(UCrysGameplayTagViewModel* Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(DamageTypeViewModel, Value);
}

void UWeaponItemInstanceViewModel::OnItemInstanceSet()
{
	Super::OnItemInstanceSet();
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDamage);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAutoAttackDelay);
}

void UWeaponItemInstanceViewModel::OnItemDefinitionLoaded(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionLoaded(ItemDefinition);
	
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
