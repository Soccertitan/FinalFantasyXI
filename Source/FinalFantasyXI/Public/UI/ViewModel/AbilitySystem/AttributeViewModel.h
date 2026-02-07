// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "AbilitySystem/AttributeTagRelationship.h"
#include "AttributeViewModel.generated.h"

class UAttributeTagRelationship;
class UAbilitySystemComponent;
struct FGameplayTag;

DECLARE_MULTICAST_DELEGATE(FCrysAttributeViewModelValueChangedSignature);

/**
 * Takes an AttributeTag and displays information about that attribute.
 */
UCLASS()
class FINALFANTASYXI_API UAttributeViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Will bind to the ASC and listen for attribute changes on the character. */
	void SetAttribute(const FGameplayTag& AttributeTag, UAbilitySystemComponent* InAbilitySystemComponent);

	/** Manually initializes the ViewModel with static data. */
	void SetAttribute(const FGameplayTag& AttributeTag, float InCurrentValue, float InBaseValue);

	float GetCurrentValue() const {return CurrentValue;}
	void SetCurrentValue(float InValue);
	float GetBaseValue() const {return BaseValue;}
	void SetBaseValue(float InValue);

	UFUNCTION(BlueprintPure, FieldNotify, Category = "Attribute ViewModel")
	FText GetAttributeName() const {return AttributeTagRelationshipItem.Name;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Attribute ViewModel")
	FText GetShortName() const {return AttributeTagRelationshipItem.ShortName;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Attribute ViewModel")
	FText GetDescription() const {return AttributeTagRelationshipItem.Description;}
	UFUNCTION(BlueprintPure, FieldNotify, Category = "Attribute ViewModel")
	UTexture2D* GetIcon() const {return AttributeTagRelationshipItem.Icon;}
	
	FCrysAttributeViewModelValueChangedSignature OnCurrentValueChangedDelegate;

private:
	/** The static details of the attribute. */
	UPROPERTY()
	FAttributeRelationshipItem AttributeTagRelationshipItem;

	/** Cached ASC */
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** The current value of the attribute. With all modifiers */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Category = "Attribute ViewModel", meta = (AllowPrivateAccess = true))
	float CurrentValue = 0.f;
	/** The base value of the attribute. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, FieldNotify, Getter, Category = "Attribute ViewModel", meta = (AllowPrivateAccess = true))
	float BaseValue = 0.f;

	void FindAndSetAttributeTagRelationshipItem(const FGameplayTag& AttributeTag);
	void OnAttributeValueChanged(const FOnAttributeChangeData& Data);
};
