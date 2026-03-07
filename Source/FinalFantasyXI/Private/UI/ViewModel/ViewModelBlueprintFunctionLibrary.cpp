// Copyright Soccertitan 2025


#include "UI/ViewModel/ViewModelBlueprintFunctionLibrary.h"

#include "UI/HUD/CrysHUD.h"
#include "UI/ViewModel/CrysGameplayTagViewModel.h"
#include "UI/ViewModel/CrysViewModel.h"

#define LOCTEXT_NAMESPACE "GameplayTagViewModel"

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

FText UViewModelBlueprintFunctionLibrary::Combine_GameplayTagViewModelsText(const TArray<UCrysGameplayTagViewModel*> ViewModels, const FText Divider, const FText EmptyText)
{
	if (ViewModels.IsEmpty())
	{
		return EmptyText;
	}
	FText Result;
	
	for (const UCrysGameplayTagViewModel* ViewModel : ViewModels)
	{
		if (ViewModel)
		{
			if (Result.IsEmptyOrWhitespace())
			{
				Result = ViewModel->GetTagName();
			}
			else
			{
				//TODO: Move this argument to a static.
				FFormatNamedArguments Args;
				Args.Add(TEXT("Base"), Result);
				Args.Add(TEXT("Divider"), Divider);
				Args.Add(TEXT("Add"), ViewModel->GetTagName());
				
				Result = FText::Format(NSLOCTEXT("GameplayTagViewModel","TagName", "{Base}{Divider}{Add}"), Args);
			}
		}
	}
	return Result;
}

FText UViewModelBlueprintFunctionLibrary::Combine_GameplayTagViewModelsShortText(const TArray<UCrysGameplayTagViewModel*> ViewModels, const FText Divider, const FText EmptyText)
{
	if (ViewModels.IsEmpty())
	{
		return EmptyText;
	}
	FText Result;
	
	for (const UCrysGameplayTagViewModel* ViewModel : ViewModels)
	{
		if (ViewModel)
		{
			if (Result.IsEmptyOrWhitespace())
			{
				Result = ViewModel->GetShortName();
			}
			else
			{
				//TODO: Move this argument to a static.
				FFormatNamedArguments Args;
				Args.Add(TEXT("Base"), Result);
				Args.Add(TEXT("Divider"), Divider);
				Args.Add(TEXT("Add"), ViewModel->GetShortName());
				
				Result = FText::Format(NSLOCTEXT("GameplayTagViewModel","ShortName", "{Base}{Divider}{Add}"), Args);
			}
		}
	}
	return Result;
}

#undef LOCTEXT_NAMESPACE
