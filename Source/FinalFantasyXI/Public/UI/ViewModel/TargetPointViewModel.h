// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "TargetPointViewModel.generated.h"

class UCrimAbilitySystemComponent;
class UAttributeFractionViewModel;
class UAttributeViewModel;
class UTargetPointComponent;

/**
 * Displays information about the current target.
 */
UCLASS()
class FINALFANTASYXI_API UTargetPointViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Viewmodel")
	void SetTargetPoint(UTargetPointComponent* TargetPoint);
	
	FText GetTargetName() const { return TargetName; }
	
protected:
	void SetTargetName(FText Value);
	
	UFUNCTION(BlueprintImplementableEvent, DisplayName = "SetTargetPoint")
	void K2_SetTargetPoint(UTargetPointComponent* TargetPoint);
	
	UFUNCTION(BlueprintPure, Category = "Viewmodel")
	UCrimAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; } 
	
private:
	UPROPERTY(BlueprintReadOnly, FieldNotify, meta = (AllowPrivateAccess = true))
	FText TargetName;
	
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
};
