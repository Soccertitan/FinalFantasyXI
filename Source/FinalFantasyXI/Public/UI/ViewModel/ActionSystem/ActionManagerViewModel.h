// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "ActionViewModelTypes.h"
#include "GameplayTagContainer.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "ActionManagerViewModel.generated.h"


class UActionViewModel;
class UActionSlotViewModel;
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
	
	int32 GetActiveActionSetIndex() const { return ActiveActionSetIndex; }
	
	/** Finds or creates an ActionVM from InputTag and Index. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionSlotViewModel* FindOrCreateActionSlotViewModel(const FGameplayTag& InputTag, int32 Index = 0);
	
	/** Finds or creates an ActionVM from InputTag from the ActiveActionSetIndex. */
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	UActionSlotViewModel* FindOrCreateActiveActionSlotViewModel(const FGameplayTag& InputTag);
	
	UFUNCTION(BlueprintCallable, Category = "Viewmodel|ActionBar")
	void SetAction(const FGameplayTag& InputTag, const int32 Index, const TSoftClassPtr<UCrysAction> ActionClass);
	
	UFUNCTION(BlueprintCallable, Category = "CrysActionManager")
	void ClearAction(const FGameplayTag& InputTag, const int32 Index);

protected:
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;
	
	void InitActionManager(APlayerController* PlayerController);
	
	UFUNCTION()
	void SetActiveActionSetIndex(int32 Index);

private:
	UPROPERTY()
	TObjectPtr<UCrysActionManagerComponent> ActionManagerComponent;
	
	UPROPERTY()
	TArray<FActionViewModelContainer> ActionSlotViewModelContainers;
	
	UPROPERTY()
	TArray<TObjectPtr<UActionSlotViewModel>> ActiveActionSlotViewModels;
	
	/** The current set that is mapped to the InputActions. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, Category = "Viewmodel|ActionBar", meta = (AllowPrivateAccess = true))
	int32 ActiveActionSetIndex = 0;
	
	UActionViewModel* InternalCreateActionViewModel(const FGameplayTag& InputTag, int32 Index);
	
	UFUNCTION()
	void OnActionMapUpdated(UCrysAction* Action, const FGameplayTag& InputTag, int32 Index);
};
