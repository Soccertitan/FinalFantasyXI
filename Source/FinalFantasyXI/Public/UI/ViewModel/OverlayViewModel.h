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
	
	bool IsRootWidgetAdded() const { return bRootWidgetAdded; }
	bool IsAutoAttacking() const { return bAutoAttacking; }
	
	virtual void InitializeViewModel(APlayerController* PlayerController) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTargetPointViewModel> TargetPointViewModelClass;

	/** The AbilitySystemComponent from the player state. */
	UFUNCTION(BlueprintImplementableEvent)
	void OnAbilitySystemComponentRetrieved(UCrimAbilitySystemComponent* AbilitySystemComponent);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnTargetPointUpdated(UTargetPointComponent* NewTarget);
	
	UFUNCTION()
	void SetIsAutoAttacking(bool Value);
	
	void OnRootWidgetAdded();
	void OnRootWidgetRemoved();
	
	void SetIsRootWidgetAdded(bool Value);

	virtual void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn) override;
	
	void InitTargetingSystemComponent(APawn* NewPawn);

private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsRootWidgetAdded, meta = (AllowPrivateAccess = true))
	bool bRootWidgetAdded = false;
	
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter=IsAutoAttacking, meta = (AllowPrivateAccess = true))
	bool bAutoAttacking = false;
	
	UPROPERTY()
	TObjectPtr<UTargetingSystemComponent> TargetingSystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAutoAttackManagerComponent> AutoAttackManagerComponent;
};
