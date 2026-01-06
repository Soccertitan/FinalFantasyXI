// Copyright Soccertitan 2025


#include "UI/ViewModel/ViewModelBlueprintFunctionLibrary.h"

#include "UI/HUD/CrysHUD.h"
#include "UI/ViewModel/CrysViewModel.h"

UCrysViewModel* UViewModelBlueprintFunctionLibrary::FindOrCreateViewModel(
	const TSubclassOf<UCrysViewModel> ViewModelClass, APlayerController* PlayerController)
{
	if (IsValid(PlayerController) && ViewModelClass)
	{
		if (ACrysHUD* HUD = Cast<ACrysHUD>(PlayerController->GetHUD()))
		{
			return HUD->FindOrCreateViewModel(ViewModelClass);
		}
	}
	return nullptr;
}
