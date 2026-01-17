// Copyright Soccertitan 2025


#include "UI/ViewModel/WeaponSetManagerViewModel.h"

#include "CrysGameplayTags.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryGameplayTags.h"
#include "InventoryManagerComponent.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Player/Hero/WeaponSetManagerComponent.h"
#include "UI/InventoryUISubsystem.h"
#include "UI/ViewModel/ItemContainerViewModel.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"
#include "UI/ViewModel/Component/WeaponSetViewModel.h"
#include "UI/ViewModel/InventorySystem/Filter/ItemViewModelFilter_EquipableItems.h"

UWeaponSetManagerViewModel::UWeaponSetManagerViewModel()
{
	ItemFilter_EquipableItems = CreateDefaultSubobject<UItemViewModelFilter_EquipableItems>("ItemFilter_EquipableItems");
	ItemFilter_EquipableItems->EquipSlot = FCrysGameplayTags::Get().EquipSlot_Weapon;

	ItemContainerTag_Equipment = FInventoryGameplayTags::Get().ItemContainer_Default;
	ItemContainerTag_Ability = FCrysGameplayTags::Get().ItemContainer_Abilities;
}

UWeaponSetViewModel* UWeaponSetManagerViewModel::FindOrCreateWeaponSetViewModel(const int32 Index)
{
	if (Index >= 0 && WeaponSetManagerComponent && InventoryManagerComponent)
	{
		for (TObjectPtr<UWeaponSetViewModel>& VM : WeaponSetViewModels)
		{
			if (VM->GetWeaponSetIndex() == Index)
			{
				return VM;
			}
		}

		UWeaponSetViewModel* NewVM = NewObject<UWeaponSetViewModel>(this, UWeaponSetViewModel::StaticClass());
		NewVM->SetWeaponSetIndex(Index);
		FWeaponSet WeaponSet = WeaponSetManagerComponent->GetWeaponSetAt(Index);
		NewVM->SetWeaponSet(WeaponSet);
		NewVM->SetWeaponViewModel(CreateWeaponViewModel(WeaponSet.ItemGuid));
		WeaponSetViewModels.Add(NewVM);
		return NewVM;
	}
	return nullptr;
}

void UWeaponSetManagerViewModel::SetActiveWeaponSet(const int32 Index)
{
	if (WeaponSetManagerComponent)
	{
		WeaponSetManagerComponent->SetActiveWeaponSet(Index);
	}
}

void UWeaponSetManagerViewModel::SetWeaponItem(const FGuid& ItemGuid, const int32 Index)
{
	if (WeaponSetManagerComponent)
	{
		FWeaponSet WeaponSet = WeaponSetManagerComponent->GetWeaponSetAt(Index);
		WeaponSet.ItemGuid = ItemGuid;
		WeaponSetManagerComponent->AddWeaponSet(WeaponSet, Index);
	}
}

void UWeaponSetManagerViewModel::ClearWeaponItem(const int32 Index)
{
	SetWeaponItem(FGuid(), Index);
}

// void UWeaponSetManagerViewModel::SetAbilityInput(const FGameplayTag& InputTag, UGameplayAbilityDefinition* AbilityDefinition, const int32 Index)
// {
// 	if (WeaponSetManagerComponent)
// 	{
// 		FWeaponSet WeaponSet = WeaponSetManagerComponent->GetWeaponSetAt(Index);
// 		if (FAbilityInputItem* InputItem = WeaponSet.InputItems.FindByKey(InputTag))
// 		{
// 			InputItem->AbilityDefinition = AbilityDefinition;
// 		}
// 		else
// 		{
// 			WeaponSet.InputItems.Add(FAbilityInputItem(InputTag, AbilityDefinition));
// 		}
// 		WeaponSetManagerComponent->AddWeaponSet(WeaponSet, Index);
// 	}
// }

void UWeaponSetManagerViewModel::ClearAbilityInput(const FGameplayTag& InputTag, const int32 Index)
{
	// SetAbilityInput(InputTag, nullptr, Index);
}

void UWeaponSetManagerViewModel::ResetWeaponSet(const int32 Index)
{
	if (WeaponSetManagerComponent)
	{
		WeaponSetManagerComponent->ClearWeaponSet(Index);
	}
}

TArray<UItemInstanceViewModel*> UWeaponSetManagerViewModel::GetEquipableWeapons() const
{
	TArray<UItemInstanceViewModel*> Result;
	if (ItemContainerViewModel_Equipment)
	{
		Result = ItemContainerViewModel_Equipment->GetItemInstanceViewModels();
		ItemFilter_EquipableItems->FilterItemInstanceViewModels(EquipmentManagerComponent, Result);
	}
	return Result;
}

TArray<UItemInstanceViewModel*> UWeaponSetManagerViewModel::GetEquipableAbilities() const
{
	TArray<UItemInstanceViewModel*> Result;
	if (ItemContainerViewModel_Abilities)
	{
		Result = ItemContainerViewModel_Abilities->GetItemInstanceViewModels();
	}
	return Result;
}


void UWeaponSetManagerViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	WeaponSetManagerComponent = PlayerController->FindComponentByClass<UWeaponSetManagerComponent>();
	InventoryManagerComponent = UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(PlayerController->GetPlayerState<APlayerState>());
	EquipmentManagerComponent = UEquipmentSystemBlueprintFunctionLibrary::GetEquipmentManagerComponent(PlayerController->GetPlayerState<APlayerState>());

	if (WeaponSetManagerComponent && InventoryManagerComponent)
	{
		SetActiveWeaponSetIndex(WeaponSetManagerComponent->GetActiveWeaponSetIndex());
		WeaponSetManagerComponent->OnActiveWeaponSetChangedDelegate.AddUObject(this, &UWeaponSetManagerViewModel::OnActiveWeaponSetChanged);
		WeaponSetManagerComponent->OnWeaponSetChangedDelegate.AddUObject(this, &UWeaponSetManagerViewModel::OnWeaponSetChanged);

		ItemContainerViewModel_Equipment = GetWorld()->GetSubsystem<UInventoryUISubsystem>()->
				CreateItemContainerViewModelForActor(InventoryManagerComponent->GetOwner(), ItemContainerTag_Equipment);
		ItemContainerViewModel_Abilities = GetWorld()->GetSubsystem<UInventoryUISubsystem>()->
				CreateItemContainerViewModelForActor(InventoryManagerComponent->GetOwner(), ItemContainerTag_Ability);
	}
}

void UWeaponSetManagerViewModel::SetActiveWeaponSetIndex(const int32 InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(ActiveWeaponSetIndex, InValue);
}

UItemInstanceViewModel* UWeaponSetManagerViewModel::CreateWeaponViewModel(const FGuid& ItemGuid)
{
	UItemInstanceViewModel* ItemViewModel = NewObject<UItemInstanceViewModel>(this, UItemInstanceViewModel::StaticClass());
	if (ItemGuid.IsValid())
	{
		if (const FItemInstance* ItemInstance = InventoryManagerComponent->FindItemByGuid(ItemGuid))
		{
			ItemViewModel->SetItemInstance(*ItemInstance);
		}
	}
	return ItemViewModel;
}

void UWeaponSetManagerViewModel::OnActiveWeaponSetChanged(int32 NewIndex, int32 OldIndex)
{
	SetActiveWeaponSetIndex(NewIndex);
}

void UWeaponSetManagerViewModel::OnWeaponSetChanged(const FWeaponSet& WeaponSet, int32 Index)
{
	for (const TObjectPtr<UWeaponSetViewModel>& VM : WeaponSetViewModels)
	{
		if (VM->GetWeaponSetIndex() == Index)
		{
			FWeaponSet OldWeaponSet = VM->GetWeaponSet();
			VM->SetWeaponSet(WeaponSet);
			if (OldWeaponSet.ItemGuid != WeaponSet.ItemGuid)
			{
				VM->SetWeaponViewModel(CreateWeaponViewModel(WeaponSet.ItemGuid));
			}
			break;
		}
	}
}
