// Copyright Soccertitan 2025


#include "StopAttackClearTargetInputActionListener.h"

#include "InputActionValue.h"
#include "TargetingSystemBlueprintFunctionLibrary.h"
#include "TargetingSystemComponent.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"
#include "GameFramework/PlayerState.h"


void UStopAttackClearTargetInputActionListener::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	Super::OnPossessedPawnChanged(OldPawn, NewPawn);
	
	TargetingSystemComponent = UTargetingSystemBlueprintFunctionLibrary::GetTargetingSystemComponent(NewPawn);
	
	if (NewPawn)
	{
		APlayerState* PlayerState = NewPawn->GetPlayerState();
		AutoAttackManagerComponent = PlayerState ? PlayerState->GetComponentByClass<UAutoAttackManagerComponent>() : nullptr;
	}
	else
	{
		AutoAttackManagerComponent = nullptr;
	}
}

void UStopAttackClearTargetInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (Value.Get<bool>() == false)
	{
		return;
	}
	
	if (AutoAttackManagerComponent && AutoAttackManagerComponent->IsAutoAttacking())
	{
		AutoAttackManagerComponent->StopAutoAttack();
		return;
	}
	
	if (TargetingSystemComponent)
	{
		if (TargetingSystemComponent->IsCameraLocked())
		{
			TargetingSystemComponent->SetCameraLock(false);
			return;
		}
		if (TargetingSystemComponent->GetTargetedPoint())
		{
			TargetingSystemComponent->ClearTarget();
			return;
		}
	}
}
