// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Attribute/CrimAttributeSet.h"
#include "MovementAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UMovementAttributeSet : public UCrimAttributeSet
{
	GENERATED_BODY()
	
public:
	UMovementAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MovementSpeedMultiplier);
	
protected:
	
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void ClampAttributes(const FGameplayAttribute& Attribute, float& NewValue) const override;
		
	UFUNCTION()
	virtual void OnRep_MovementSpeedMultiplier(const FGameplayAttributeData& OldValue);

private:
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement Attribute Set", ReplicatedUsing=OnRep_MovementSpeedMultiplier, meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeedMultiplier;
};
