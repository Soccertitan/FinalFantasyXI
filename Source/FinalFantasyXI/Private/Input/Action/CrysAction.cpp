// Copyright Soccertitan 2025


#include "Input/Action/CrysAction.h"

#include "InputActionValue.h"

void UCrysAction::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (Value.Get<bool>())
	{
		OnInputPressed.Broadcast(this);
	}
	else
	{
		OnInputReleased.Broadcast(this);
	}
}

void UCrysAction::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	OnInputReleased.Broadcast(this);
}

void UCrysAction::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	OnInputReleased.Broadcast(this);
}
