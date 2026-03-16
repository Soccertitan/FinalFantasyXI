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
	
protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
	/** The AbilitySystemComponent of the controlled pawn. */
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Attribute")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const {return AbilitySystemComponent;}
	
private:
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
};
