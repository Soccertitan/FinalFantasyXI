// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/CombatGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CrysGameplayTags.h"
#include "Character/CrysCharacter.h"

UCombatAnimationData* UCombatGameplayAbility::GetPrimaryCombatAnimationData() const
{
	return PrimaryCombatAnimationData;
}

UCombatAnimationData* UCombatGameplayAbility::GetSecondaryCombatAnimationData() const
{
	return SecondaryCombatAnimationData;
}

void UCombatGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	const FGameplayTag& DualWieldTag = FCrysGameplayTags::Get().Gameplay_State_DualWielding;
	FOnGameplayEffectTagCountChanged::FDelegate Delegate = FOnGameplayEffectTagCountChanged::FDelegate::CreateWeakLambda(this,
		[this](const FGameplayTag Tag, int32 NewCount)
		{
			bDualWielding = NewCount > 0;
		});
	GetAbilitySystemComponentFromActorInfo()->RegisterAndCallGameplayTagEvent(DualWieldTag, Delegate);
}

void UCombatGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	
	if (ACrysCharacter* Character = Cast<ACrysCharacter>(ActorInfo->AvatarActor.Get()))
	{
		OnPrimaryAnimationDataChanged(Character->GetPrimaryCombatAnimationData());
		Character->OnPrimaryCombatAnimationDataUpdatedDelegate.AddUniqueDynamic(this, &UCombatGameplayAbility::OnPrimaryAnimationDataChanged);
		OnSecondaryAnimationDataChanged(Character->GetSecondaryCombatAnimationData());
		Character->OnSecondaryCombatAnimationDataUpdatedDelegate.AddUniqueDynamic(this, &UCombatGameplayAbility::OnSecondaryAnimationDataChanged);
	}
}

void UCombatGameplayAbility::OnPrimaryAnimationDataChanged(UCombatAnimationData* AnimationData)
{
	PrimaryCombatAnimationData = AnimationData;
}

void UCombatGameplayAbility::OnSecondaryAnimationDataChanged(UCombatAnimationData* AnimationData)
{
	SecondaryCombatAnimationData = AnimationData;
}
