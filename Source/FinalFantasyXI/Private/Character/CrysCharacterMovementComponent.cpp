// Copyright Soccertitan 2025


#include "Character/CrysCharacterMovementComponent.h"

#include "CrimAbilitySystemComponent.h"
#include "CrysGameplayTags.h"


UCrysCharacterMovementComponent::UCrysCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
}

float UCrysCharacterMovementComponent::GetMaxSpeed() const
{
	if (AbilitySystemComponent)
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FCrysGameplayTags::Get().Gameplay_State_MovementRooted))
		{
			return 0.f;	
		}
	}
	
	return Super::GetMaxSpeed();
}

FRotator UCrysCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (AbilitySystemComponent)
	{
		if (AbilitySystemComponent->HasMatchingGameplayTag(FCrysGameplayTags::Get().Gameplay_State_MovementRooted))
		{
			return FRotator();
		}
	}

	return Super::GetDeltaRotation(DeltaTime);
}

void UCrysCharacterMovementComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	AbilitySystemComponent = NewAbilitySystemComponent;
}



