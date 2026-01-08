// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"

#include "Net/UnrealNetwork.h"

UCrysHitPointsAttributeSet::UCrysHitPointsAttributeSet()
{
}

void UCrysHitPointsAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, BarrierPoints, COND_None, REPNOTIFY_Always);
}

void UCrysHitPointsAttributeSet::OnRep_BarrierPoints(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, BarrierPoints, OldValue);
}

void UCrysHitPointsAttributeSet::ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetBarrierPointsAttribute())
	{
		// Barrier can't go below a value of 0.
		NewValue = FMath::Max(NewValue, 0.f);
	}
	else
	{
		Super::ClampAttributes(Attribute, NewValue);
	}
}

void UCrysHitPointsAttributeSet::HandleDamage(const FGameplayEffectModCallbackData& Data, float Magnitude)
{
	const float LocalDamage = FMath::Abs(Magnitude);
	const float RemainingDamage = FMath::Max(LocalDamage - GetBarrierPoints(), 0.f);

	SetBarrierPoints(FMath::Max(GetBarrierPoints() - LocalDamage, 0.f));
	
	Super::HandleDamage(Data, RemainingDamage);
}
