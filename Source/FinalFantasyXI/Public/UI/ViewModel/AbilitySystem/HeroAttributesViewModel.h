// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "HeroAttributesViewModel.generated.h"

class UCrimAbilitySystemComponent;

/**
 * Display information about the hero's attributes from the Ability System Component.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UHeroAttributesViewModel : public UCrysViewModel
{
	GENERATED_BODY()
	
public:
	virtual void InitializeViewModel(APlayerController* PlayerController) override;
	
protected:
	/** The AbilitySystemComponent from the player state. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilitySystemComponentRetrieved(UCrimAbilitySystemComponent* AbilitySystemComponent);
};
