// Copyright Soccertitan 2025


#include "Character/CrysCharacterMovementComponent.h"

#include "CrimAbilitySystemComponent.h"
#include "CrysGameplayTags.h"
#include "AbilitySystem/AttributeSet/MovementAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"


UCrysCharacterMovementComponent::UCrysCharacterMovementComponent()
{
	bOrientRotationToMovement = true;
}

float UCrysCharacterMovementComponent::GetMaxSpeed() const
{
	if (bMovementRooted)
	{
		return 0.f;
	}
	
	return Super::GetMaxSpeed() * MovementSpeedMultiplier;
}

FRotator UCrysCharacterMovementComponent::GetDeltaRotation(float DeltaTime) const
{
	if (bMovementRooted)
	{
		return FRotator();
	}

	return Super::GetDeltaRotation(DeltaTime);
}

void UCrysCharacterMovementComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(
			FCrysGameplayTags::Get().Gameplay_State_MovementRooted,
			EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	}

	AbilitySystemComponent = NewAbilitySystemComponent;
	
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(
		   FCrysGameplayTags::Get().Gameplay_State_MovementRooted,
		   EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UCrysCharacterMovementComponent::OnGameplayTagMovementRootedUpdated);
		
		bMovementRooted = AbilitySystemComponent->HasMatchingGameplayTag(FCrysGameplayTags::Get().Gameplay_State_MovementRooted);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UMovementAttributeSet::GetMovementSpeedMultiplierAttribute())
		.AddUObject(this, &UCrysCharacterMovementComponent::OnMovementSpeedMultiplierUpdated);
		
		bool bFound = false;
		MovementSpeedMultiplier = AbilitySystemComponent->GetGameplayAttributeValue(
			UMovementAttributeSet::GetMovementSpeedMultiplierAttribute(), bFound);
		MovementSpeedMultiplier = bFound ? MovementSpeedMultiplier : 1.f;
	}
}

void UCrysCharacterMovementComponent::OnGameplayTagMovementRootedUpdated(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		bMovementRooted = true;
	}
	else
	{
		bMovementRooted = false;
	}
}

void UCrysCharacterMovementComponent::OnMovementSpeedMultiplierUpdated(const FOnAttributeChangeData& Data)
{
	MovementSpeedMultiplier = Data.NewValue;
}
