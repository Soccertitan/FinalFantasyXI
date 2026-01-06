// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrysCharacterMovementComponent.generated.h"


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
};
