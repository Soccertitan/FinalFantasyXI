// Copyright Soccertitan 2025


#include "UI/ViewModel/EquipmentSystem/EquipmentManagerViewModel.h"

#include "GameplayTagContainer.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "InventoryFastTypes.h"
#include "InventoryManagerComponent.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "UI/InventoryUISubsystem.h"
#include "UI/InventoryViewModelBlueprintFunctionLibrary.h"
#include "UI/ViewModel/ItemContainerViewModel.h"
#include "UI/ViewModel/ItemInstanceViewModel.h"
#include "UI/ViewModel/EquipmentSystem/EquippedItemViewModel.h"
#include "UI/ViewModel/InventorySystem/Filter/ItemInstanceViewModelFilter_EquipableItems.h"

UEquipmentManagerViewModel::UEquipmentManagerViewModel()
{
	EquippableItemsFilter = CreateDefaultSubobject<UItemInstanceViewModelFilter_EquipableItems>("Filter");
}

UEquippedItemViewModel* UEquipmentManagerViewModel::FindOrCreateEquippedItemViewModel(const FGameplayTag EquipSlot)
{
	if (EquipSlot.IsValid())
	{
		for (TObjectPtr<UEquippedItemViewModel>& ViewModel : EquippedItemViewModels)
		{
			if (ViewModel->EquipSlot == EquipSlot)
			{
				return ViewModel;
			}
		}
		if (EquipmentManagerComponent)
		{
			return InternalCreateEquippedItemViewModel(EquipSlot);
		}
	}
	return nullptr;
}

TArray<UItemContainerViewModel*> UEquipmentManagerViewModel::GetAllowedItemContainers() const
{
	return AllowedItemContainerViewModels;
}

TArray<UItemInstanceViewModel*> UEquipmentManagerViewModel::GetEquippableItems(const FGameplayTag EquipSlot, UItemContainerViewModel* ItemContainerViewModel) const
{
	TArray<UItemInstanceViewModel*> Result;
	
	if (EquipSlot.IsValid() && ItemContainerViewModel)
	{
		Result = ItemContainerViewModel->GetItemInstanceViewModels();
		EquippableItemsFilter->EquipSlot = EquipSlot;
		EquippableItemsFilter->FilterItemInstanceViewModels(EquipmentManagerComponent, Result);
	}
	
	return Result;
}

void UEquipmentManagerViewModel::TryEquipItem(FGameplayTag EquipSlot, UItemInstanceViewModel* ItemInstanceViewModel)
{
	if (EquipmentManagerComponent && ItemInstanceViewModel)
	{
		EquipmentManagerComponent->TryEquipItem(EquipSlot, ItemInstanceViewModel->GetItemInstance().GetGuid());
	}
}

void UEquipmentManagerViewModel::TryUnequipItem(FGameplayTag EquipSlot)
{
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->TryUnequipItem(EquipSlot);
	}
}

void UEquipmentManagerViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	Super::OnInitializeViewModel(PlayerController);
	
	EquipmentManagerComponent = UEquipmentSystemBlueprintFunctionLibrary::GetEquipmentManagerComponent(PlayerController->GetPlayerState<APlayerState>());
	
	if (EquipmentManagerComponent)
	{
		EquipmentManagerComponent->OnItemEquippedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemEquipped);
		EquipmentManagerComponent->OnItemUnequippedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemUnequipped);
	}
	
	InventoryManagerComponent = UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(PlayerController->GetPlayerState<APlayerState>());
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->OnItemAddedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemChanged);
		InventoryManagerComponent->OnItemChangedDelegate.AddUniqueDynamic(this, &UEquipmentManagerViewModel::OnItemChanged);
	}
	
	if (EquipmentManagerComponent && InventoryManagerComponent)
	{
		TArray<FGameplayTag> AllowedItemContainers = EquipmentManagerComponent->GetAllowedItemContainers();
		UInventoryUISubsystem* UISubsystem = GetWorld()->GetSubsystem<UInventoryUISubsystem>();
		for (const FItemContainerInstance& ItemContainerInstance : InventoryManagerComponent->GetItemContainers())
		{
			if (UItemContainer* ItemContainer = ItemContainerInstance.GetItemContainer())
			{
				bool bAllowed = AllowedItemContainers.Num() == 0;
				for (const FGameplayTag& AllowedTag : AllowedItemContainers)
				{
					if (ItemContainer->GetItemContainerTag() == AllowedTag)
					{
						bAllowed = true;
					}
				}
				
				if (bAllowed)
				{
					if (UItemContainerViewModel* ItemContainerViewModel = UISubsystem->CreateItemContainerViewModel(ItemContainer))
					{
						AllowedItemContainerViewModels.Add(ItemContainerViewModel);
					}
				}
			}
		}
	}
}

UEquippedItemViewModel* UEquipmentManagerViewModel::InternalCreateEquippedItemViewModel(const FGameplayTag& EquipSlot)
{
	UEquippedItemViewModel* NewVM = NewObject<UEquippedItemViewModel>(this);
	NewVM->SetEquipSlot(EquipSlot);
	NewVM->SetItemInstanceViewModel(
		UInventoryViewModelBlueprintFunctionLibrary::CreateItemInstanceViewModel(this, 
			EquipmentManagerComponent->K2_GetEquippedItemInstance(EquipSlot)));
	EquippedItemViewModels.Add(NewVM);
	return NewVM;
}

void UEquipmentManagerViewModel::OnItemEquipped(const FEquippedItem& EquippedItem)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetEquipSlot() == EquippedItem.EquipSlot)
		{
			UItemInstanceViewModel* ItemInstanceViewModel = UInventoryViewModelBlueprintFunctionLibrary::CreateItemInstanceViewModel(
				this, EquipmentManagerComponent->K2_GetEquippedItemInstance(EquippedItem.EquipSlot));
			ViewModel->SetItemInstanceViewModel(ItemInstanceViewModel);
			break;
		}
	}
}

void UEquipmentManagerViewModel::OnItemUnequipped(const FEquippedItem& EquippedItem)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetEquipSlot() == EquippedItem.EquipSlot)
		{
			UItemInstanceViewModel* ItemInstanceViewModel = NewObject<UItemInstanceViewModel>(this);
			ViewModel->SetItemInstanceViewModel(ItemInstanceViewModel);
			break;
		}
	}
}

void UEquipmentManagerViewModel::OnItemChanged(const FItemInstance& ItemInstance)
{
	for (UEquippedItemViewModel* ViewModel : EquippedItemViewModels)
	{
		if (ViewModel->GetItemInstanceViewModel()->GetItemInstance() == ItemInstance)
		{
			ViewModel->GetItemInstanceViewModel()->SetItemInstance(ItemInstance);
		}
	}
}
