// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/AutoAttack/AutoAttackGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "CrysGameplayTags.h"
#include "NativeGameplayTags.h"
#include "AbilitySystem/Ability/AutoAttack/AutoAttackManagerComponent.h"
#include "AbilitySystem/Ability/AutoAttack/AutoAttackAnimationData.h"
#include "System/CrysAssetManager.h"

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
	
	if (PrimaryAttacks)
	{
		LoadAnimationData(PrimaryAttacks, PrimaryAttacksStreamableHandle);
	}
	if (SecondaryAttacks)
	{
		LoadAnimationData(SecondaryAttacks, SecondaryAttacksStreamableHandle);
	}
	
	InitAutoAttackManager(ActorInfo);
	
	GetAbilitySystemComponentFromActorInfo()->RegisterGameplayTagEvent(FCrysGameplayTags::Get().Gameplay_State_DualWielding).AddUObject(this, &UAutoAttackGameplayAbility::OnDualWieldingTagChanged);
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

void UAutoAttackGameplayAbility::LoadAnimationData(UAutoAttackAnimationData* AnimationData, TSharedPtr<FStreamableHandle>& Handle)
{
	const TArray<FName>& LoadBundles = {"Animation"};
	Handle = UCrysAssetManager::Get().PreloadPrimaryAssets({AnimationData->GetPrimaryAssetId()}, LoadBundles, false);
}

void UAutoAttackGameplayAbility::InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo)
{
	AutoAttackManager = ActorInfo->OwnerActor->FindComponentByClass<UAutoAttackManagerComponent>();
	ensure(AutoAttackManager);
	
	if (AutoAttackManager)
	{
		if (UAutoAttackAnimationData* AnimationData = AutoAttackManager->GetPrimaryAutoAttackAnimationData())
		{
			PrimaryAttacks = AnimationData;
			LoadAnimationData(AnimationData, PrimaryAttacksStreamableHandle);
		}
		
		if (UAutoAttackAnimationData* AnimationData = AutoAttackManager->GetSecondaryAutoAttackAnimationData())
		{
			SecondaryAttacks = AnimationData;
			LoadAnimationData(AnimationData, SecondaryAttacksStreamableHandle);
		}
	}
}

void UAutoAttackGameplayAbility::OnDualWieldingTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	bDualWielding = NewCount > 0;
}
