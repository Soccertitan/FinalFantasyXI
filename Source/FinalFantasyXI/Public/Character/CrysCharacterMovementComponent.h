// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrysCharacterMovementComponent.generated.h"


struct FOnAttributeChangeData;
struct FGameplayTag;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UCrysCharacterMovementComponent : public UCharacterMovementComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()

public:
	UCrysCharacterMovementComponent();

	virtual float GetMaxSpeed() const override;
	virtual FRotator GetDeltaRotation(float DeltaTime) const override;

	// ~Implements ICrimAbilitySystemInterface
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;
	// ~end Implements ICrimAbilitySystemInterface
	
private:
	// Cached ASC from the owner.
	UPROPERTY()
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	
	// Cached value from the ASC.
	UPROPERTY()
	float MovementSpeedMultiplier = 1.f;
	
	UPROPERTY()
	bool bMovementRooted = false;
	
	void OnGameplayTagMovementRootedUpdated(const FGameplayTag Tag, int32 NewCount);
	void OnMovementSpeedMultiplierUpdated(const FOnAttributeChangeData& Data);
};
