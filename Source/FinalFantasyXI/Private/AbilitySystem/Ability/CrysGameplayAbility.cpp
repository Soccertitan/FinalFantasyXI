// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/CrysGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimMathStatics.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"

UCrysGameplayAbility::UCrysGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	bServerRespectsRemoteAbilityCancellation = false;
	
	TargetType = EAbilityTargetType::Any;
	TargetRange = 300.f;
	TargetHalfAngle = 45.f;
	CastTime = 0.f;
}

float UCrysGameplayAbility::GetTargetHalfAngle_Implementation() const
{
	return TargetHalfAngle.GetValueAtLevel(GetAbilityLevel());
}

float UCrysGameplayAbility::GetTargetRange_Implementation() const
{
	return TargetRange.GetValueAtLevel(GetAbilityLevel());
}

float UCrysGameplayAbility::GetCastTime_Implementation() const
{
	float BaseCastTime = CastTime.GetValueAtLevel(GetAbilityLevel());
	FGameplayTagRequirements SourceTagRequirements;
	SourceTagRequirements.RequireTags.AppendTags(GetAbilitySystemComponentFromActorInfo()->GetOwnedGameplayTags());
	SourceTagRequirements.RequireTags.AppendTags(GetAssetTags());
	float CastTimeMultiplier = GetAbilitySystemComponentFromActorInfo()->GetFilteredAttributeValue(
		UAbilityAttributeSet::GetCastSpeedMultiplierAttribute(), 
		SourceTagRequirements,
		FGameplayTagContainer());
	
	return FMath::Max(BaseCastTime * CastTimeMultiplier, 0.f);
}

void UCrysGameplayAbility::ApplyGameplayEffectToTarget(AActor* TargetActor, AActor* EffectCauser)
{
}

bool UCrysGameplayAbility::IsTargetValid(AActor* TargetActor) const
{
	if (!TargetActor)
	{
		return false;
	}
	
	UCrimAbilitySystemComponent* TargetASC = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(TargetActor);
	if (!TargetASC)
	{
		return false;
	}
	
	if (TargetTagRequirements.RequirementsMet(TargetASC->GetOwnedGameplayTags()) == false)
	{
		return false;
	}
	
	if (GetTargetType() != EAbilityTargetType::Any)
	{
		if (GetTargetType() == EAbilityTargetType::SelfOnly &&
			GetCrimAbilitySystemComponentFromActorInfo() != TargetASC)
		{
			return false;
		}
		
		ETeamAttitude::Type AttitudeTowardsTarget = UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(GetAvatarActorFromActorInfo(), TargetActor);
		if (GetTargetType() == EAbilityTargetType::Friendly &&
			AttitudeTowardsTarget != ETeamAttitude::Friendly)
		{
			return false;
		}
	
		if (GetTargetType() == EAbilityTargetType::FriendlyExcludeSelf && (
			AttitudeTowardsTarget != ETeamAttitude::Friendly ||
			GetCrimAbilitySystemComponentFromActorInfo() == TargetASC))
		{
			return false;
		}
	
		if (GetTargetType() == EAbilityTargetType::Hostile &&
			AttitudeTowardsTarget != ETeamAttitude::Hostile)
		{
			return false;
		}
	}
	
	if (UCrimMathStatics::DistanceBetweenActors(GetAvatarActorFromActorInfo(), TargetActor) > GetTargetRange())
	{
		return false;
	}
	
	if (!UCrimMathStatics::IsInCone(
		GetAvatarActorFromActorInfo()->GetActorLocation(), 
		GetAvatarActorFromActorInfo()->GetActorForwardVector(),
		GetTargetHalfAngle(),
		TargetActor->GetActorLocation()))
	{
		return false;
	}
	
	return true;
}
