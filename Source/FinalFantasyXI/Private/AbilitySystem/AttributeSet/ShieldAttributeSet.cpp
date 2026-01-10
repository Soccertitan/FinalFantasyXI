// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/ShieldAttributeSet.h"

#include "Net/UnrealNetwork.h"

UShieldAttributeSet::UShieldAttributeSet()
{
}

void UShieldAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ShieldSkill, COND_None, REPNOTIFY_Always);
}

void UShieldAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	if (Attribute == GetBlockDamageReductionAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, 1.0f);
	}
	else
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void UShieldAttributeSet::OnRep_ShieldSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ShieldSkill, OldValue);
}
