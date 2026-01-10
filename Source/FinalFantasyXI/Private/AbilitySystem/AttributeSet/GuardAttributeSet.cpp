// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/GuardAttributeSet.h"

#include "Net/UnrealNetwork.h"

UGuardAttributeSet::UGuardAttributeSet()
{
}

void UGuardAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, GuardSkill, COND_None, REPNOTIFY_Always);
}

void UGuardAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::ClampAttributes(Attribute, NewValue);
	
	NewValue = FMath::Max(NewValue, 0.0f);
}

void UGuardAttributeSet::OnRep_GuardSkill(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, GuardSkill, OldValue);
}
