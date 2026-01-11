// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeSet/JobAttributeSet.h"

UJobAttributeSet::UJobAttributeSet()
{
	InitMainJobRank(1.f);
	InitSubJobEffectiveness(0.5f);
}

void UJobAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UJobAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetMainJobRankAttribute())
	{
		NewValue = FMath::Max(NewValue, 1.f);
	}
	else
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UJobAttributeSet::OnRep_MainJobRank(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MainJobRank, OldValue);
}

void UJobAttributeSet::OnRep_SubJobRank(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, SubJobRank, OldValue);
}
