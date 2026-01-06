// Copyright Soccertitan 2025


#include "UI/ViewModel/CrysViewModel.h"

void UCrysViewModel::InitializeViewModel(APlayerController* PlayerController)
{
	if (IsValid(PlayerController))
	{
		OnInitializeViewModel(PlayerController);
		K2_OnInitializeViewModel(PlayerController);
		UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(OnViewModelInitialized);
	}
}
