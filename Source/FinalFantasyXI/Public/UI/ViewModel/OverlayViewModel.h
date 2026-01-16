// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysViewModel.h"
#include "OverlayViewModel.generated.h"

class UAttributeViewModel;
class UAttributeFractionViewModel;
class UCrimAbilitySystemComponent;
struct FGameplayTag;

/**
 * The overlay viewmodel for the Hero.
 */
UCLASS(Abstract)
class FINALFANTASYXI_API UOverlayViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	/**
	 * Creates an AttributeFraction ViewModel using the two attribute tags for Num/Den using the controlled pawn's ASC.
	 * @param NumeratorAttributeTag The attribute that denotes the current value.
	 * @param DenominatorAttributeTag The attribute that denotes the max value the Current value can achieve.
	 * @return The newly created ViewModel.
	 */
	UFUNCTION(BlueprintCallable, Category = "Overlay View Model")
	UAttributeFractionViewModel* CreateAttributeFractionViewModel(
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag NumeratorAttributeTag,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DenominatorAttributeTag);

	/**
	 * Creates an AttributeViewModel initialized with the controlled pawn's ASC.
	 * @param AttributeTag The attribute to monitor.
	 * @return The newly created ViewModel
	 */
	UFUNCTION(BlueprintCallable, Category = "Overlay View Model")
	UAttributeViewModel* CreateAttributeViewModel(UPARAM(meta = (Categories = "Attribute")) const FGameplayTag AttributeTag);

	/** The AbilitySystemComponent of the controlled pawn. */
	UFUNCTION(BlueprintPure, Category = "Overlay View Model")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const {return AbilitySystemComponent;}

	/** Called after the CrimAbilitySystemComponent has been set to a valid pointer. */
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnAbilitySystemComponentSet")
	void K2_OnAbilitySystemComponentSet(UCrimAbilitySystemComponent* NewAbilitySystemComponent, UCrimAbilitySystemComponent* OldAbilitySystemComponent);

private:

	/** The AbilitySystemComponent of the controlled pawn. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
};
