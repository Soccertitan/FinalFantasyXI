// Copyright Soccertitan 2025


#include "UI/HUD/CrysHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/CrysViewModel.h"

UCrysViewModel* ACrysHUD::FindOrCreateViewModel(const TSubclassOf<UCrysViewModel> ViewModelClass)
{
	ViewModels.Remove(nullptr);
	
	if (!ViewModelClass)
	{
		return nullptr;
	}

	for (const TWeakObjectPtr<UCrysViewModel>& VM : ViewModels)
	{
		if (IsValid(VM.Get()) && VM->IsA(ViewModelClass))
		{
			return VM.Get();
		}
	}

	UCrysViewModel* ViewModel = NewObject<UCrysViewModel>(this, ViewModelClass);
	ViewModel->InitializeViewModel(GetOwningPlayerController());
	ViewModels.Add(ViewModel);
	return ViewModel;
}

void ACrysHUD::InitializeHUD()
{
	if (HUDWidgetClass)
	{
		HUDWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), HUDWidgetClass);
		HUDWidget->AddToPlayerScreen();
	}
}
