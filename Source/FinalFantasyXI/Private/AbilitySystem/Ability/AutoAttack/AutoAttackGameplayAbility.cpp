// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/AutoAttack/AutoAttackGameplayAbility.h"

#include "NativeGameplayTags.h"
#include "AbilitySystem/Ability/AutoAttack/AutoAttackManagerComponent.h"

namespace AutoAttackTag
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_GameplayEvent_AutoAttack, "Ability.GameplayEvent.AutoAttack")
}

UAutoAttackGameplayAbility::UAutoAttackGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ActivationPolicy = EAbilityActivationPolicy::OnEvent;
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = AutoAttackTag::Ability_GameplayEvent_AutoAttack;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UAutoAttackGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	AutoAttackManager = ActorInfo->OwnerActor->FindComponentByClass<UAutoAttackManagerComponent>();
	ensure(AutoAttackManager);
}

void UAutoAttackGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AutoAttackManager)
	{
		AutoAttackManager->RestartAutoAttackTimer();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
