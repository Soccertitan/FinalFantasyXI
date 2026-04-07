// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "ActionViewModelTypes.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "ActionManagerViewModel.generated.h"


class UActionViewModel;
class UCrysAction;
class UCrysActionManagerComponent;

/**
 * Information of Actions that are mapped to the active ActionBar InputTags in the ActionManagerComponent.
 */
UCLASS()
class FINALFANTASYXI_API UActionManagerViewModel : public UCrysViewModel
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, FieldNotify)
	FActionViewModelUpdated OnActionViewModelUpdated() const { return ActionViewModelUpdated; }
	
	int32 GetActiveActionSetIndex() const { return ActiveActionSetIndex; }
	
	/** Finds or creates an ActionVM from InputTag and Index. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionViewModel* FindOrCreateActionViewModel(const FGameplayTag& InputTag, int32 Index = 0);
	
	/** Finds or creates an ActionVM from InputTag from the ActiveActionSetIndex. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionViewModel* FindOrCreateActiveActionViewModel(const FGameplayTag& InputTag);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
	void InitActionManager(APlayerController* PlayerController);
	
	UFUNCTION()
	void SetActiveActionSetIndex(int32 Index);

private:
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;
	
	UPROPERTY()
	TArray<FActionViewModelContainer> ActionViewModelContainers;
	
	UPROPERTY()
	TArray<FActionViewModelItem> ActiveActionViewModels;
	
	/** Information on the most recent change. */
	UPROPERTY()
	FActionViewModelUpdated ActionViewModelUpdated;
	
	/** The current set that is mapped to the InputActions. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	int32 ActiveActionSetIndex = 0;
	
	UActionViewModel* InternalCreateActionViewModel(const FGameplayTag& InputTag, int32 Index);
	
	UFUNCTION()
	void OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index);
};
