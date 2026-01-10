// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/ParryAttributeSet.h"

#include "Net/UnrealNetwork.h"

UParryAttributeSet::UParryAttributeSet()
{
}

void UParryAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ParrySkill, COND_None, REPNOTIFY_Always);
}

void UParryAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	NewValue = FMath::Max(NewValue, 0.0f);
}

void UParryAttributeSet::OnRep_ParrySkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ParrySkill, OldValue);
}
