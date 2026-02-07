// Copyright Soccertitan 2025


#include "ActionSystem/CrysActionInputActionListener.h"

#include "ActionSystem/CrysActionManagerComponent.h"

void UCrysActionInputActionListener::OnInitializeListener()
{
	Super::OnInitializeListener();
	
	ActionManager = GetPlayerController()->FindComponentByClass<UCrysActionManagerComponent>();
}

void UCrysActionInputActionListener::OnInputActionTriggered(const FInputActionValue& Value)
{
	Super::OnInputActionTriggered(Value);
	
	if (IsPressed() && ActionManager)
	{
		ActionManager->TryActivateAction(InputTag);
	}
}
