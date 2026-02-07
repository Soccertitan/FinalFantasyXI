// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "ActionBarViewModel.generated.h"


class UActionBarItemViewModel;
class UInputActionListenerViewModel;
class UInputAction;
class UCrysAction;
class UCrimEnhancedInputComponent;
class UCrysActionManagerComponent;

/**
 * Information of Actions that are bound to InputActions.
 */
UCLASS()
class FINALFANTASYXI_API UActionBarViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:

	int32 GetCurrentActionSet() const { return CurrentActionSet; }
	void SetCurrentActionSet(int32 Index);
	
	/** Finds or creates an ActionBarItem VM from InputTag. */
	UFUNCTION(BlueprintCallable, Category = "ActionBarViewModel")
	UActionBarItemViewModel* FindOrCreateActionBarItemViewModel(const FGameplayTag& InputTag);
	
	/** Finds or creates an InputActionListener VM from an InputAction. */
	UFUNCTION(BlueprintCallable, Category = "ActionBarViewModel")
	UInputActionListenerViewModel* FindOrCreateInputActionListenerViewModel(UInputAction* InputAction);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
	void InitActionManager(APlayerController* PlayerController);
	void InitEnhancedInput(APlayerController* PlayerController);

private:
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;
	
	UPROPERTY()
	TObjectPtr<UCrimEnhancedInputComponent> EnhancedInputComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UInputActionListenerViewModel>> InputActionListenerViewModels;
	
	UPROPERTY()
	TArray<TObjectPtr<UActionBarItemViewModel>> ActionBarItemViewModels;
	
	/** The current set that is mapped to the InputActions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, Category = "ActionBarViewModel", meta = (AllowPrivateAccess = true))
	int32 CurrentActionSet = 0;
	
	UActionBarItemViewModel* InternalCreateActionBarItemViewModel(const FGameplayTag& InputTag);
	UInputActionListenerViewModel* InternalCreateInputActionListenerViewModel(UInputAction* InputAction);
	
	UFUNCTION()
	void OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index);
	UFUNCTION()
	void OnActionSetSelected(int32 Index);
};
