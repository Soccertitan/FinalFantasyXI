// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrysViewModel.h"
#include "OverlayViewModel.generated.h"

class UAutoAttackManagerComponent;
class UTargetPointViewModel;
class UTargetPointComponent;
class UTargetingSystemComponent;
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
	UFUNCTION(BlueprintPure, FieldNotify)
	FMVVMEventField OnTargetCleared() const { return FMVVMEventField {};}
	
	bool IsRootWidgetAdded() const { return bRootWidgetAdded; }
	bool IsAutoAttacking() const { return bAutoAttacking; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTargetPointViewModel> TargetPointViewModelClass;
	
	virtual void OnInitializeViewModel(APlayerController* PlayerController) override;

	/** The AbilitySystemComponent of the controlled pawn. */
	UFUNCTION(BlueprintPure, Category = "Viewmodel|Overlay")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const {return AbilitySystemComponent;}
	
	UFUNCTION()
	void OnTargetPointUpdated(UTargetPointComponent* NewTarget);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnTargetPointSet")
	void K2_OnTargetPointSet(UTargetPointComponent* NewTarget);
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "OnTargetPointRemoved")
	void K2_OnTargetPointRemoved();
	UFUNCTION()
	void SetAutoAttacking(bool Value);
	
	void OnRootWidgetAdded();
	void OnRootWidgetRemoved();
	
	void SetRootWidgetAdded(bool Value);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsRootWidgetAdded, meta = (AllowPrivateAccess = true))
	bool bRootWidgetAdded = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsAutoAttacking, meta = (AllowPrivateAccess = true))
	bool bAutoAttacking = false;

	/** The AbilitySystemComponent of the controlled pawn. */
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UTargetingSystemComponent> TargetingSystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManagerComponent;
};
