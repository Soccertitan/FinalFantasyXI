// Copyright Soccertitan 2025


#include "UI/ViewModel/InventorySystem/EquipmentItemInstanceViewModel.h"

#include "InventoryBlueprintFunctionLibrary.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "EquipmentSystem/ItemFragment_Equipment.h"
#include "UI/ViewModel/CrysGameplayTagViewModel.h"

void UEquipmentItemInstanceViewModel::SetLevelRequirement(int32 Value)
{
	UE_MVVM_SET_PROPERTY_VALUE(LevelRequirement, Value);
}

void UEquipmentItemInstanceViewModel::SetAllowedJobViewModels(TArray<UCrysGameplayTagViewModel*> Value)
{
	AllowedJobViewModels = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAllowedJobViewModels);
}

void UEquipmentItemInstanceViewModel::SetEquipSlotViewModels(UCrysGameplayTagViewModel* Value)
{
	EquipSlotViewModel = Value;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetEquipSlotViewModels);
}

void UEquipmentItemInstanceViewModel::OnItemInstanceSet()
{
	Super::OnItemInstanceSet();
	
	const TInstancedStruct<FItem>& Item = GetItemInstance().GetItem();
	if (const FItemShard_Equipment* ItemShard = Item.Get<FItem>().FindShardByType<FItemShard_Equipment>())
	{
		SetUpgradeLevel(ItemShard->Level);
		SetIsEquipped(IsValid(ItemShard->GetEquipmentManagerComponent()));
	}
	else
	{
		SetIsEquipped(false);
	}
}

void UEquipmentItemInstanceViewModel::OnItemDefinitionLoaded(const UItemDefinition* ItemDefinition)
{
	Super::OnItemDefinitionLoaded(ItemDefinition);
	
	if (const FItemFragment_Equipment* ItemFragment = ItemDefinition->FindFragmentByType<FItemFragment_Equipment>())
	{
		SetLevelRequirement(ItemFragment->LevelRequirement);

		UCrysGameplayTagViewModel* NewVM = NewObject<UCrysGameplayTagViewModel>(this);
		NewVM->SetGameplayTag(ItemFragment->EquipSlot);
		SetEquipSlotViewModels(NewVM);
		
		TArray<UCrysGameplayTagViewModel*> AllowedJobs;
		for (const FGameplayTag& Job : ItemFragment->Jobs.GetGameplayTagArray())
		{
			UCrysGameplayTagViewModel* NewJobVM = NewObject<UCrysGameplayTagViewModel>(this);
			NewJobVM->SetGameplayTag(Job);
			AllowedJobs.Add(NewJobVM);
		}
		SetAllowedJobViewModels(AllowedJobs);
	}
}
