// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Combat/AutoAttackGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"
#include "AbilitySystem/Ability/Combat/CombatAnimationData.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"


UAutoAttackGameplayAbility::UAutoAttackGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ActivationPolicy = EAbilityActivationPolicy::OnEvent;
	
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = Crys::NativeGameplayTag::Ability_GameplayEvent_AutoAttack;
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

FWeaponData UAutoAttackGameplayAbility::GetWeaponData(const FGameplayTag EquipSlot) const
{
	if (EquipmentManagerComponent)
	{
		FEquippedItem EquippedItem = EquipmentManagerComponent->GetEquippedItem(EquipSlot);
		if (EquippedItem.WeaponData.IsValid())
		{
			return EquippedItem.WeaponData;
		}
		return EquipmentManagerComponent->GetBareHandedWeaponData();
	}
	return FWeaponData();
}

int32 UAutoAttackGameplayAbility::CalculateNumberOfAttacks_Implementation() const
{
	return 1;
}

void UAutoAttackGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	
	InitAutoAttackManager(ActorInfo);
	
	EquipmentManagerComponent = ActorInfo->OwnerActor->FindComponentByClass<UEquipmentManagerComponent>();
	ensure(EquipmentManagerComponent);
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

void UAutoAttackGameplayAbility::AttackTarget(const FWeaponData& WeaponData, AActor* TargetActor, AActor* EffectCauser)
{
	if (!K2_HasAuthority() && !WeaponData.AutoAttackGameplayEffectClass)
	{
		return;
	}
	
	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(WeaponData.AutoAttackGameplayEffectClass, WeaponData.Level);
		if (EffectCauser)
		{
			SpecHandle.Data.Get()->GetContext().Get()->SetEffectCauser(EffectCauser);
		}
		SpecHandle.Data.Get()->AddDynamicAssetTag(WeaponData.WeaponSkill);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(Crys::NativeGameplayTag::SetByCaller_WeaponDamage, 
			WeaponData.Damage.GetValueAtLevel(WeaponData.Level));
		
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void UAutoAttackGameplayAbility::InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo)
{
	AutoAttackManager = ActorInfo->OwnerActor->FindComponentByClass<UAutoAttackManagerComponent>();
	ensure(AutoAttackManager);
}
