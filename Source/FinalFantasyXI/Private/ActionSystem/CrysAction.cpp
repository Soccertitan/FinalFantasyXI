// Copyright Soccertitan 2025


#include "ActionSystem/CrysAction.h"

UCrysAction::UCrysAction()
{
}

bool UCrysAction::TryActivateAction()
{
	if (CanActivateAction())
	{
		ActivateAction();
		K2_ActivateAction();
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

bool UCrysAction::K2_CanActivateAction_Implementation() const
{
	return true;
}
