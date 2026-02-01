// Copyright Soccertitan 2025


#include "Input/Action/CrysInputActionListener.h"

#include "InputActionValue.h"


void UCrysInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	bPressed = Value.Get<bool>();
	if (bPressed)
	{
		OnInputPressed.Broadcast(this);
	}
	else
	{
		OnInputReleased.Broadcast(this);
	}
}

void UCrysInputActionListener::OnInputActionCanceled(const FInputActionValue& Value)
{
	Super::OnInputActionCanceled(Value);
	if (bPressed)
	{
		bPressed = false;
		OnInputReleased.Broadcast(this);
	}
}

void UCrysInputActionListener::OnInputActionCompleted(const FInputActionValue& Value)
{
	Super::OnInputActionCompleted(Value);
	if (bPressed)
	{
		bPressed = false;
		OnInputReleased.Broadcast(this);
	}
}
