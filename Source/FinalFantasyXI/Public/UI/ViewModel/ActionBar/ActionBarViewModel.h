// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "../CrysViewModel.h"
#include "GameplayTagContainer.h"
#include "ActionBarViewModel.generated.h"

class UActionBarItemViewModel;
struct FAbilityInputItem;
class UAbilityInputManagerComponent;
struct FWeaponSet;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UActionBarViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:
	bool IsAbilityTogglePressed() const { return bAbilityTogglePressed; }

	UFUNCTION(BlueprintCallable)
	UActionBarItemViewModel* FindOrCreateActionBarItemViewModel(UPARAM(ref) FGameplayTag& InputTag);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

private:
	UPROPERTY()
	TObjectPtr<UAbilityInputManagerComponent> AbilityInputManagerComponent;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsAbilityTogglePressed, meta = (AllowPrivateAccess = "true"))
	bool bAbilityTogglePressed = false;

	void OnAbilityToggleStateChanged(bool bPressed);

	void OnAbilityInputAdded(UAbilityInputManagerComponent* InputManager, const FAbilityInputItem& AbilityInputItem);
	void OnAbilityInputChanged(UAbilityInputManagerComponent* InputManager, const FAbilityInputItem& AbilityInputItem);
	void OnAbilityInputRemoved(UAbilityInputManagerComponent* InputManager, const FAbilityInputItem& AbilityInputItem);

	void OnInputPressed(const FGameplayTag& InputTag);
	void OnInputReleased(const FGameplayTag& InputTag);

	/** Cached InputTag whenever an Input is pressed or released. */
	UPROPERTY()
	FGameplayTag InputTagEvent;

	UPROPERTY()
	TArray<TObjectPtr<UActionBarItemViewModel>> ActionBarItemViewModels;
};
