// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CombatGameplayAbility.h"
#include "AutoAttackGameplayAbility.generated.h"

struct FWeaponData;
class UEquipmentManagerComponent;
class UCombatAnimationData;
class UAutoAttackManagerComponent;

/**
 * Listens for an event from the AutoAttackManagerComponent to trigger auto attack. When the ability ends, automatically 
 * restarts the auto attack timer.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UAutoAttackGameplayAbility : public UCombatGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAutoAttackGameplayAbility();
	
	/** Use this function for random number generation to allow for client prediction. */
	UFUNCTION(BlueprintPure, Category = "Ability|AutoAttack")
	const FRandomStream& GetAutoAttackRandomStream() const;
	
	/** 
	 * Indicates how many attack cycles there should be. Ie. Double/Triple attack effects.
	 */
	UFUNCTION(BlueprintCallable, BlueprintPure = false, BlueprintNativeEvent, Category = "Ability|AutoAttack")
	int32 CalculateNumberOfAttacks() const;
	
	/** Uses the RandomStream to get a random Primary Attack. */
	UFUNCTION(BlueprintPure, Category = "Ability|AutoAttack")
	UAnimMontage* GetRandomPrimaryAttackMontage() const;
	/** Uses the RandomStream to get a random Secondary Attack. */
	UFUNCTION(BlueprintPure, Category = "Ability|AutoAttack")
	UAnimMontage* GetRandomSecondaryAttackMontage() const;
	
	UFUNCTION(BlueprintPure, Category = "Ability|AutoAttack")
	FWeaponData GetWeaponData(UPARAM(meta = (Categories = "EquipSlot.Hand")) const FGameplayTag EquipSlot) const;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	/** 
	 * Uses the weapon data to create a gameplay spec to apply to the target actor. Using the SetByCaller.WeaponDamage. 
	 * @WeaponData The weapon to use.
	 * @TargetActor The actor to attack.
	 * @EffectCauser The actor that enmity should be applied to.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Ability|AutoAttack")
	void AttackTarget(const FWeaponData& WeaponData, AActor* TargetActor, AActor* EffectCauser = nullptr);
	
private:
	UPROPERTY()
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManager;

	/** Tells us what weapons are equipped. */
	UPROPERTY()
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

	FRandomStream AutoAttackRandomStream;

	void InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo);
};
