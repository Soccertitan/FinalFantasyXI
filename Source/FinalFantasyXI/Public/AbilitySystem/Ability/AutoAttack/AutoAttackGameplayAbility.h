// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/CrysGameplayAbility.h"
#include "AutoAttackGameplayAbility.generated.h"

class UAutoAttackManagerComponent;

/**
 * Listens for an event from the AutoAttackManagerComponent to trigger auto attack. When the ability ends, automatically 
 * restarts the auto attack timer.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UAutoAttackGameplayAbility : public UCrysGameplayAbility
{
	GENERATED_BODY()
	
public:
	UAutoAttackGameplayAbility();
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
private:
	UPROPERTY()
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManager;
};
