// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/AutoAttackGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CrysGameplayTags.h"
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
	if (PrimaryAttacks && PrimaryAttacks->AutoAttacks.Num() > 0)
	{
		return PrimaryAttacks->AutoAttacks[AutoAttackRandomStream.RandRange(0, PrimaryAttacks->AutoAttacks.Num() - 1)].Get();
	}
	return nullptr;
}

UAnimMontage* UAutoAttackGameplayAbility::GetRandomSecondaryAttackMontage() const
{
	if (SecondaryAttacks && SecondaryAttacks->AutoAttacks.Num() > 0)
	{
		return SecondaryAttacks->AutoAttacks[AutoAttackRandomStream.RandRange(0, SecondaryAttacks->AutoAttacks.Num() - 1)].Get();
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
	
	const FGameplayTag& DualWieldTag = FCrysGameplayTags::Get().Gameplay_State_DualWielding;
	OnDualWieldingTagChanged(DualWieldTag, GetAbilitySystemComponentFromActorInfo()->GetTagCount(DualWieldTag));
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(DualWieldTag).AddUObject(this, &UAutoAttackGameplayAbility::OnDualWieldingTagChanged);
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
	
	if (AutoAttackManager)
	{
		if (UCombatAnimationData* AnimationData = AutoAttackManager->GetPrimaryCombatAnimationData())
		{
			OnPrimaryAttacksChanged(AnimationData);
		}
		AutoAttackManager->OnPrimaryCombatAnimationDataUpdatedDelegate.AddUniqueDynamic(this, &UAutoAttackGameplayAbility::OnPrimaryAttacksChanged);
		
		if (UCombatAnimationData* AnimationData = AutoAttackManager->GetSecondaryCombatAnimationData())
		{
			OnSecondaryAttacksChanged(AnimationData);
		}
		AutoAttackManager->OnSecondaryCombatAnimationDataUpdatedDelegate.AddUniqueDynamic(this, &UAutoAttackGameplayAbility::OnSecondaryAttacksChanged);
	}
}

void UAutoAttackGameplayAbility::OnPrimaryAttacksChanged(UCombatAnimationData* AnimationData)
{
	PrimaryAttacks = AnimationData;
}

void UAutoAttackGameplayAbility::OnSecondaryAttacksChanged(UCombatAnimationData* AnimationData)
{
	SecondaryAttacks = AnimationData;
}

void UAutoAttackGameplayAbility::OnDualWieldingTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bDualWielding = NewCount > 0;
}
