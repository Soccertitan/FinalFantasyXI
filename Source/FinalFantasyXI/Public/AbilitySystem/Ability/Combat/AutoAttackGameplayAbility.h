// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CombatGameplayAbility.h"
#include "AutoAttackGameplayAbility.generated.h"

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
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	UPROPERTY()
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManager;
	
	FRandomStream AutoAttackRandomStream;

	void InitAutoAttackManager(const FGameplayAbilityActorInfo* ActorInfo);
};
