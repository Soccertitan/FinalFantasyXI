// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysAction.h"
#include "CrysAction_ActivateGameplayAbility.generated.h"

class UCrimGameplayAbility;
class UAbilityInputManagerComponent;

/**
 * Tries to activate a gameplay ability from the PlayerControllers AbilityInputComponent.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UCrysAction_ActivateGameplayAbility : public UCrysAction
{
	GENERATED_BODY()
	
public:
	
	virtual bool CanActivateAction() const override;
	
protected:
	virtual void InitializeAction() override;
	virtual void ActivateAction_Implementation() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UCrimGameplayAbility> AbilityClass;
};
