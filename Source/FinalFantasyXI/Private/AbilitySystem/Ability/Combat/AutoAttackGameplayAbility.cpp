// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/AutoAttackGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"
#include "AbilitySystem/Ability/Combat/CombatAnimationData.h"

namespace AutoAttackTag
{
	UE_DEFINE_GAMEPLAY_TAG_STATIC(Ability_GameplayEvent_AutoAttack, "Ability.GameplayEvent.AutoAttack")
}

UAutoAttackGameplayAbility::UAutoAttackGameplayAbility()
{
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

const FRandomStream& UAutoAttackGameplayAbility::GetAutoAttackRandomStream() const
{
	return AutoAttackRandomStream;
}

UAnimMontage* UAutoAttackGameplayAbility::GetRandomPrimaryAttackMontage() const
{
	if (GetPrimaryCombatAnimationData() && GetPrimaryCombatAnimationData()->AutoAttacks.Num() > 0)
	{
		return GetPrimaryCombatAnimationData()->AutoAttacks[AutoAttackRandomStream.RandRange(0, GetPrimaryCombatAnimationData()->AutoAttacks.Num() - 1)].Get();
	}
	return nullptr;
}

UAnimMontage* UAutoAttackGameplayAbility::GetRandomSecondaryAttackMontage() const
{
	if (GetSecondaryCombatAnimationData() && GetSecondaryCombatAnimationData()->AutoAttacks.Num() > 0)
	{
		return GetSecondaryCombatAnimationData()->AutoAttacks[AutoAttackRandomStream.RandRange(0, GetSecondaryCombatAnimationData()->AutoAttacks.Num() - 1)].Get();
	}
	return nullptr;
}

int32 UAutoAttackGameplayAbility::CalculateNumberOfAttacks_Implementation() const
{
	return 1;
}

void UAutoAttackGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	InitAutoAttackManager(ActorInfo);
}

void UAutoAttackGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (TriggerEventData)
	{
		AutoAttackRandomStream.Initialize(TriggerEventData->EventMagnitude);
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAutoAttackGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (AutoAttackManager)
	{
		AutoAttackManager->RestartAutoAttackTimer();
	}
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UAutoAttackGameplayAbility::InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo)
{
	AutoAttackManager = ActorInfo->OwnerActor->FindComponentByClass<UAutoAttackManagerComponent>();
	ensure(AutoAttackManager);
}
