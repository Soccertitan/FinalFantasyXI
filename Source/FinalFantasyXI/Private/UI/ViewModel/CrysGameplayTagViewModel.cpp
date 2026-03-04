// Copyright Soccertitan 2025


#include "UI/ViewModel/CrysGameplayTagViewModel.h"

#include "CrysBlueprintFunctionLibrary.h"

void UCrysGameplayTagViewModel::SetGameplayTag(const FGameplayTag& InValue)
{
	if (RelationshipItem.Tag != InValue)
	{
		RelationshipItem = UCrysGameplayTagRelationship::FindCrysGameplayTagRelationshipItem(InValue, true);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetTagName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	}
}
