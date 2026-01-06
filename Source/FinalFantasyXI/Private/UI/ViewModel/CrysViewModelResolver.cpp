// Copyright Soccertitan 2025


#include "UI/ViewModel/CrysViewModelResolver.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/CrysViewModel.h"
#include "UI/ViewModel/ViewModelBlueprintFunctionLibrary.h"

UObject* UCrysViewModelResolver::CreateInstance(const UClass* ExpectedType, const UUserWidget* UserWidget, const UMVVMView* View) const
{
	return GetViewModel(ExpectedType, UserWidget);
}

bool UCrysViewModelResolver::DoesSupportViewModelClass(const UClass* Class) const
{
	if (Super::DoesSupportViewModelClass(Class))
	{
		if (!Class->IsChildOf(UCrysViewModel::StaticClass()))
		{
			return false;
		}
	}

	return true;
}

UCrysViewModel* UCrysViewModelResolver::GetViewModel(const UClass* ExpectedType, const UUserWidget* UserWidget) const
{
	TSubclassOf<UCrysViewModel> Class(const_cast<UClass*>(ExpectedType));
	return UViewModelBlueprintFunctionLibrary::FindOrCreateViewModel(Class, UserWidget->GetOwningPlayer());
}
