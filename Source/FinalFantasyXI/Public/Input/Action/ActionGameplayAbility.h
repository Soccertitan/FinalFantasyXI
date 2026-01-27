// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysAction.h"
#include "ActionGameplayAbility.generated.h"

class UGameplayAbility;
class UAbilityInputManagerComponent;

/**
 * Activates a gameplay ability by InputTag. If the AbilityClass is invalid, removes the ability input.
 */
UCLASS()
class FINALFANTASYXI_API UActionGameplayAbility : public UCrysAction
{
	GENERATED_BODY()
	
public:
	
protected:
	virtual void OnInitializeAction() override;
	virtual void OnInputActionTriggered(const FInputActionValue& Value) override;
	virtual void OnInputActionCanceled(const FInputActionValue& Value) override;
	virtual void OnInputActionCompleted(const FInputActionValue& Value) override;
	
	virtual void OnInputTagAdded(const FGameplayTag& InputTag) override;
	virtual void OnInputTagRemoved(const FGameplayTag& InputTag) override;
	
private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;
	
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UGameplayAbility> AbilityClass;
	
	UPROPERTY()
	bool bValidAbility = false;
};
