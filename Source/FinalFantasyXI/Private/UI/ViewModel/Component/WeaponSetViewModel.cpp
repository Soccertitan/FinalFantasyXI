// Copyright Soccertitan 2025


#include "UI/ViewModel/Component/WeaponSetViewModel.h"

#include "InventoryManagerComponent.h"
#include "Player/Hero/WeaponSetManagerComponent.h"
#include "UI/ViewModel/ItemViewModel.h"
#include "UI/ViewModel/Component/GameplayAbilityDefinitionViewModel.h"


void UWeaponSetViewModel::SetWeaponSet(const FWeaponSet& InWeaponSet)
{
	WeaponSet = InWeaponSet;
	for (const TPair<FGameplayTag, TObjectPtr<UGameplayAbilityDefinitionViewModel>>& Pair : GameplayAbilityDefinitionInputMapViewModels)
	{
		if (const FAbilityInputItem* FoundInputItem = InWeaponSet.InputItems.FindByKey(Pair.Key))
		{
			// Pair.Value->SetAbilityDefinition(FoundInputItem->AbilityDefinition);
		}
		else
		{
			Pair.Value->ResetData();
		}
	}
	SetCanEquipWeapon(WeaponSet.CanEquipWeapon());
}

UGameplayAbilityDefinitionViewModel* UWeaponSetViewModel::FindOrCreateGameplayAbilityDefinitionViewModel(FGameplayTag InputTag)
{
	if (InputTag.IsValid())
	{
		TObjectPtr<UGameplayAbilityDefinitionViewModel>* FoundVM = GameplayAbilityDefinitionInputMapViewModels.Find(InputTag);
		if (FoundVM)
		{
			return *FoundVM;
		}

		UGameplayAbilityDefinitionViewModel* NewVM = NewObject<UGameplayAbilityDefinitionViewModel>(this, UGameplayAbilityDefinitionViewModel::StaticClass());
		if (const FAbilityInputItem* FoundInputItem = WeaponSet.InputItems.FindByKey(InputTag))
		{
			// NewVM->SetAbilityDefinition(FoundInputItem->AbilityDefinition);
		}
		GameplayAbilityDefinitionInputMapViewModels.Add(InputTag, NewVM);
		return NewVM;
	}
	return nullptr;
}

void UWeaponSetViewModel::SetWeaponSetIndex(const int32 InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponSetIndex, InValue);
}

void UWeaponSetViewModel::SetCanEquipWeapon(const bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bCanEquipWeapon, InValue);
}

void UWeaponSetViewModel::SetWeaponViewModel(UItemInstanceViewModel* InViewModel)
{
	UE_MVVM_SET_PROPERTY_VALUE(WeaponViewModel, InViewModel);
}
