// Copyright Soccertitan 2025


#include "ActionSystem/CrysAction.h"

#include "UI/ViewModel/ActionSystem/ActionViewModel.h"

UCrysAction::UCrysAction()
{
	ActionViewModel = UActionViewModel::StaticClass();
}

bool UCrysAction::TryActivateAction()
{
	if (CanActivateAction())
	{
		ActivateAction();
		return true;
	}
	
	return false;
}

bool UCrysAction::CanActivateAction() const
{
	if (PlayerController && K2_CanActivateAction())
	{
		return true;
	}

	return false;
}
