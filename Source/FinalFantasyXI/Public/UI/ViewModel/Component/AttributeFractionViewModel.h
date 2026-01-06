// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AttributeFractionViewModel.generated.h"

struct FGameplayTag;
class UAbilitySystemComponent;
class UAttributeViewModel;
/**
 * Takes two attributes and treats them as a Numerator/Denominator value relationship. For ease in getting percentages
 * between the current values of both. Most common example is HitPoints.
 */
UCLASS()
class FINALFANTASYXI_API UAttributeFractionViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	UAttributeFractionViewModel();

	UFUNCTION(BlueprintCallable, Category = "Attribute Fraction View Model")
	void SetAttributesWithASC(UAbilitySystemComponent* InAbilitySystemComponent,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag NumeratorAttributeTag,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DenominatorAttributeTag);

	UFUNCTION(BlueprintCallable, Category = "Attribute Fraction View Model")
	void SetAttributes(UPARAM(meta = (Categories = "Attribute"))const FGameplayTag NumeratorAttributeTag, float NumeratorCurrentValue, float NumeratorBaseValue,
		UPARAM(meta = (Categories = "Attribute")) const FGameplayTag DenominatorAttributeTag, float DenominatorCurrentValue, float DenominatorBaseValue);

	UAttributeViewModel* GetNumeratorAttribute() const {return NumeratorAttribute;}
	UAttributeViewModel* GetDenominatorAttribute() const {return DenominatorAttribute;}
	
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Attribute Fraction View Model")
	float GetPercentCurrentValue() const;

private:
	/** The numerator value of the attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeViewModel> NumeratorAttribute;

	/** The denominator value for the current attribute. */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Getter, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAttributeViewModel> DenominatorAttribute;

	void CreateViewModelsAndBindToDelegates();
	void BroadcastValueChanged();
};
