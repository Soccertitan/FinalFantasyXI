// Copyright Soccertitan 2025


#include "UI/ViewModel/AbilitySystem/AttributeViewModel.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "Settings/FinalFantasyXIGameData.h"

void UAttributeViewModel::SetAttribute(const FGameplayTag& AttributeTag, UAbilitySystemComponent* InAbilitySystemComponent)
{
	if (!InAbilitySystemComponent)
	{
		UE_LOG(LogCrys, Error, TEXT("Invalid AbilitySystemComponent passed into [%s]"), *GetNameSafe(this));
		return;
	}

	FindAndSetAttributeTagRelationshipItem(AttributeTag);

	if (!AttributeTagRelationshipItem.IsValid())
	{
		if (!AttributeTagRelationshipItem.GameplayAttribute.IsValid())
		{
			UE_LOG(LogCrys, Error, TEXT("Invalid GameplayAttribute found with AttributeTag [%s] in [%s]"), *AttributeTag.ToString(), *GetDefault<UFinalFantasyXIGameData>()->GetName());
			return;
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeTagRelationshipItem.GameplayAttribute).RemoveAll(this);
	}
	AbilitySystemComponent = InAbilitySystemComponent;
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeTagRelationshipItem.GameplayAttribute).AddUObject(this, &UAttributeViewModel::OnAttributeValueChanged);

	if (AbilitySystemComponent->HasAttributeSetForAttribute(AttributeTagRelationshipItem.GameplayAttribute))
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
				   AttributeTagRelationshipItem.GameplayAttribute, AttributeTagRelationshipItem.SourceTags, FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
		SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(AttributeTagRelationshipItem.GameplayAttribute));
	}
}

void UAttributeViewModel::SetAttribute(const FGameplayTag& AttributeTag, float InCurrentValue, float InBaseValue)
{
	FindAndSetAttributeTagRelationshipItem(AttributeTag);

	SetCurrentValue(InCurrentValue);
	SetBaseValue(InBaseValue);
}

void UAttributeViewModel::SetCurrentValue(float InValue)
{
	if (UE_MVVM_SET_PROPERTY_VALUE(CurrentValue, InValue))
	{
		OnCurrentValueChangedDelegate.Broadcast();
	}
}

void UAttributeViewModel::SetBaseValue(float InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(BaseValue, InValue);
}

void UAttributeViewModel::FindAndSetAttributeTagRelationshipItem(const FGameplayTag& AttributeTag)
{
	if (AttributeTagRelationshipItem.AttributeTag == AttributeTag)
	{
		// Do nothing as we already have the AttributeInfo.
		return;
	}

	AttributeTagRelationshipItem = UCrysBlueprintFunctionLibrary::FindAttributeRelationshipItem(AttributeTag, true);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetIcon);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetAttributeName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
}

void UAttributeViewModel::OnAttributeValueChanged(const FOnAttributeChangeData& Data)
{
	if (AttributeTagRelationshipItem.SourceTags.IsEmpty())
	{
		SetCurrentValue(Data.NewValue);
	}
	else
	{
		float NewCurrentValue = AbilitySystemComponent->GetFilteredAttributeValue(
			AttributeTagRelationshipItem.GameplayAttribute, AttributeTagRelationshipItem.SourceTags, FGameplayTagContainer());
		SetCurrentValue(NewCurrentValue);
	}

	SetBaseValue(AbilitySystemComponent->GetNumericAttributeBase(AttributeTagRelationshipItem.GameplayAttribute));
}
