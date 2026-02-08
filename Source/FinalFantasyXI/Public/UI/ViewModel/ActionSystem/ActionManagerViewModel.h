// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "ActionManagerViewModel.generated.h"


class UActionItemViewModel;
class UCrysAction;
class UCrysActionManagerComponent;

/**
 * Information of Actions that are mapped to InputTags in the ActionManagerComponent.
 */
UCLASS()
class FINALFANTASYXI_API UActionManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:

	int32 GetCurrentActionSet() const { return CurrentActionSet; }
	void SetCurrentActionSet(int32 Index);
	
	/** Finds or creates an ActionBarItem VM from InputTag. */
	UFUNCTION(BlueprintCallable, Category = "ActionBarViewModel")
	UActionItemViewModel* FindOrCreateActionBarItemViewModel(const FGameplayTag& InputTag);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
	void InitActionManager(APlayerController* PlayerController);

private:
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;
	
	UPROPERTY()
	TArray<TObjectPtr<UActionItemViewModel>> ActionBarItemViewModels;
	
	/** The current set that is mapped to the InputActions. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Setter, Category = "ActionBarViewModel", meta = (AllowPrivateAccess = true))
	int32 CurrentActionSet = 0;
	
	UActionItemViewModel* InternalCreateActionBarItemViewModel(const FGameplayTag& InputTag);
	
	UFUNCTION()
	void OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index);
	UFUNCTION()
	void OnActionSetSelected(int32 Index);
};
