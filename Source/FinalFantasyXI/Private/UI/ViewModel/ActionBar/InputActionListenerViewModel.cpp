// Copyright Soccertitan 2025


#include "UI/ViewModel/ActionBar/InputActionListenerViewModel.h"

#include "Input/InputAction/CrysInputActionListener.h"


void UInputActionListenerViewModel::InitializeInputActionListener(UInputAction* InInputAction, UCrysInputActionListener* InInputActionListener)
{
	InputAction = InInputAction;
	
	if (!InputActionListener && InInputActionListener)
	{
		InputActionListener = InInputActionListener;
		
		SetIsPressed(InputActionListener->IsPressed());
		InputActionListener->OnInputPressed.AddUniqueDynamic(this, &UInputActionListenerViewModel::OnInputPressed);
		InputActionListener->OnInputReleased.AddUniqueDynamic(this, &UInputActionListenerViewModel::OnInputReleased);
	}
}

void UInputActionListenerViewModel::SetIsPressed(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bPressed, InValue);
}

void UInputActionListenerViewModel::OnInputPressed(UCrysInputActionListener* Action)
{
	SetIsPressed(true);
}

void UInputActionListenerViewModel::OnInputReleased(UCrysInputActionListener* Action)
{
	SetIsPressed(false);
}
