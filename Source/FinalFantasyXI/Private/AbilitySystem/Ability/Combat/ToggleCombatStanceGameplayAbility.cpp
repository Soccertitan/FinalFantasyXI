// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/ToggleCombatStanceGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CrysGameplayTags.h"
#include "AbilitySystem/Ability/Combat/CombatAnimationData.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"

UAnimMontage* UToggleCombatStanceGameplayAbility::GetPrimaryEnterCombatAnim() const
{
	if (PrimaryAttacks)
	{
		return PrimaryAttacks->EnterCombat.Get();
	}
	return nullptr;
}

UAnimMontage* UToggleCombatStanceGameplayAbility::GetPrimaryExitCombatAnim() const
{
	if (PrimaryAttacks)
	{
		return PrimaryAttacks->ExitCombat.Get();
	}
	return nullptr;
}

UAnimMontage* UToggleCombatStanceGameplayAbility::GetSecondaryEnterCombatAnim() const
{
	if (SecondaryAttacks)
	{
		return SecondaryAttacks->EnterCombat.Get();
	}
	return nullptr;
}

UAnimMontage* UToggleCombatStanceGameplayAbility::GetSecondaryExitCombatAnim() const
{
	if (SecondaryAttacks)
	{
		return SecondaryAttacks->ExitCombat.Get();
	}
	return nullptr;
}

void UToggleCombatStanceGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	InitAutoAttackManager(ActorInfo);
	
	const FGameplayTag& DualWieldTag = FCrysGameplayTags::Get().Gameplay_State_DualWielding;
	OnDualWieldingTagChanged(DualWieldTag, GetAbilitySystemComponentFromActorInfo()->GetTagCount(DualWieldTag));
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(DualWieldTag).AddUObject(this, &UToggleCombatStanceGameplayAbility::OnDualWieldingTagChanged);
}

void UToggleCombatStanceGameplayAbility::InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo)
{
	AutoAttackManager = ActorInfo->OwnerActor->FindComponentByClass<UAutoAttackManagerComponent>();
	ensure(AutoAttackManager);
	
	if (AutoAttackManager)
	{
		if (UCombatAnimationData* AnimationData = AutoAttackManager->GetPrimaryCombatAnimationData())
		{
			OnPrimaryAttacksChanged(AnimationData);
		}
		AutoAttackManager->OnPrimaryCombatAnimationDataUpdatedDelegate.AddUniqueDynamic(this, &UToggleCombatStanceGameplayAbility::OnPrimaryAttacksChanged);
		
		if (UCombatAnimationData* AnimationData = AutoAttackManager->GetSecondaryCombatAnimationData())
		{
			OnSecondaryAttacksChanged(AnimationData);
		}
		AutoAttackManager->OnSecondaryCombatAnimationDataUpdatedDelegate.AddUniqueDynamic(this, &UToggleCombatStanceGameplayAbility::OnSecondaryAttacksChanged);
	}
}

void UToggleCombatStanceGameplayAbility::OnPrimaryAttacksChanged(UCombatAnimationData* AnimationData)
{
	PrimaryAttacks = AnimationData;
}

void UToggleCombatStanceGameplayAbility::OnSecondaryAttacksChanged(UCombatAnimationData* AnimationData)
{
	SecondaryAttacks = AnimationData;
}

void UToggleCombatStanceGameplayAbility::OnDualWieldingTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bDualWielding = NewCount > 0;
}
