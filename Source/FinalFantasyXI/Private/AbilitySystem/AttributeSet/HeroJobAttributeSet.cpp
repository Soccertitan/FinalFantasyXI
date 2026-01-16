// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/HeroJobAttributeSet.h"

#include "Net/UnrealNetwork.h"

UHeroJobAttributeSet::UHeroJobAttributeSet()
{
	InitMainJobLevel(1.f);
	InitSubJobEffectiveness(0.25f);
}

void UHeroJobAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MainJobLevel, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SubJobLevel, COND_None, REPNOTIFY_Always);
}

void UHeroJobAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMainJobLevelAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UHeroJobAttributeSet::OnRep_MainJobLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MainJobLevel, OldValue);
}

void UHeroJobAttributeSet::OnRep_SubJobLevel(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, SubJobLevel, OldValue);
}
