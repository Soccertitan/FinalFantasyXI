// Copyright Soccertitan 2025


#include "UI/ViewModel/HeroManagerViewModel.h"

#include "Engine/AssetManager.h"
#include "GameFramework/PlayerState.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
#include "UI/ViewModel/Component/HeroClassViewModel.h"

UHeroClassViewModel* UHeroManagerViewModel::FindHeroClassViewModel(FGameplayTag HeroClassTag)
{
	if (HeroClassTag.IsValid())
	{
		for (const TObjectPtr<UHeroClassViewModel>& HeroClassViewModel : HeroClassViewModels)
		{
			if (HeroClassViewModel->GetHeroClass()->HeroClassTag.MatchesTagExact(HeroClassTag))
			{
				return HeroClassViewModel;
			}
		}
	}
	return nullptr;
}

void UHeroManagerViewModel::TrySetHeroClass(FGameplayTag HeroClassTag)
{
	if (!HeroClassTag.IsValid() || !HeroManagerComponent || bSwitchingHeroClass)
	{
		return;
	}
	
	for (const TObjectPtr<UHeroClassViewModel>& HeroClassViewModel : HeroClassViewModels)
	{
		if (HeroClassViewModel->GetHeroClass()->HeroClassTag.MatchesTagExact(HeroClassTag))
		{
			UE_MVVM_SET_PROPERTY_VALUE(bSwitchingHeroClass, true);
			HeroManagerComponent->TrySetHeroClass(HeroClassViewModel->GetHeroClass());
		}
	}
}

void UHeroManagerViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	HeroManagerComponent = UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(PlayerController->GetPlayerState<APlayerState>());

	if (HeroManagerComponent)
	{
		HeroManagerComponent->OnHeroClassChangedDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnHeroClassChanged);
		HeroManagerComponent->OnTrySetHeroClassFailedDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnTrySetHeroClassFailed);
		HeroManagerComponent->OnHeroClassProgressUpdatedDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnHeroClassProgressUpdated);
		CreateHeroClassViewModel(HeroManagerComponent->GetHeroClass());
		LoadHeroClasses();
	}
}

void UHeroManagerViewModel::SetIsLoadingHeroClasses(const bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bLoadingHeroClasses, InValue);
}

void UHeroManagerViewModel::SetCurrentHeroClassViewModel(UHeroClassViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsCurrentHeroClass(true);
	}
	CurrentHeroClassViewModel = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetCurrentHeroClassViewModel);
}

void UHeroManagerViewModel::LoadHeroClasses()
{
	FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UHeroManagerViewModel::OnHeroClassesLoaded);
	HeroClassStreamableHandle = UAssetManager::Get().PreloadPrimaryAssets(HeroClassesToLoad, {}, false, Delegate);
}

void UHeroManagerViewModel::OnHeroClassesLoaded()
{
	for (const FPrimaryAssetId& AssetId : HeroClassesToLoad)
	{
		if (UHeroClassDefinition* HeroClass = Cast<UHeroClassDefinition>(UAssetManager::Get().GetPrimaryAssetObject(AssetId)))
		{
			CreateHeroClassViewModel(HeroClass);
		}
	}
	UE_MVVM_SET_PROPERTY_VALUE(bLoadingHeroClasses, false);
	HeroClassStreamableHandle.Reset();
}

void UHeroManagerViewModel::CreateHeroClassViewModel(UHeroClassDefinition* HeroClass)
{
	if (HeroClass)
	{
		UHeroClassViewModel* HeroClassViewModel = FindHeroClassViewModel(HeroClass->HeroClassTag);
		if (!HeroClassViewModel)
		{
			HeroClassViewModel = NewObject<UHeroClassViewModel>(this, UHeroClassViewModel::StaticClass());
			const FHeroClassProgressItem ProgressItem = HeroManagerComponent->FindHeroClassProgressItem(HeroClass->HeroClassTag);
			HeroClassViewModel->SetHeroClassAndProgress(HeroClass, ProgressItem);
			if (HeroManagerComponent->GetHeroClass() == HeroClass)
			{
				SetCurrentHeroClassViewModel(HeroClassViewModel);
			}
			HeroClassViewModels.Add(HeroClassViewModel);
		}
	}
}

void UHeroManagerViewModel::OnHeroClassChanged(UHeroManagerComponent* InHeroManagerComponent)
{
	UHeroClassDefinition* CurrentHeroClass = InHeroManagerComponent->GetHeroClass();

	bool bFoundExistingHeroClass = false;
	for (const TObjectPtr<UHeroClassViewModel>& HeroClassViewModel : HeroClassViewModels)
	{
		if (HeroClassViewModel->GetHeroClass() == CurrentHeroClass)
		{
			SetCurrentHeroClassViewModel(HeroClassViewModel);
			bFoundExistingHeroClass = true;
		}
		else
		{
			HeroClassViewModel->SetIsCurrentHeroClass(false);
		}
	}

	if (!bFoundExistingHeroClass)
	{
		CreateHeroClassViewModel(CurrentHeroClass);
	}

	UE_MVVM_SET_PROPERTY_VALUE(bSwitchingHeroClass, false);
}

void UHeroManagerViewModel::OnTrySetHeroClassFailed(UHeroManagerComponent* InHeroManagerComponent)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSwitchingHeroClass, false);
}

void UHeroManagerViewModel::OnHeroClassProgressUpdated(UHeroManagerComponent* InHeroManagerComponent,
	const FHeroClassProgressItem& HeroClassProgressItem)
{
	for (const TObjectPtr<UHeroClassViewModel>& HeroClassViewModel : HeroClassViewModels)
	{
		if (HeroClassViewModel->GetHeroClass()->HeroClassTag.MatchesTagExact(HeroClassProgressItem.HeroClassTag))
		{
			HeroClassViewModel->SetHeroClassProgressItem(HeroClassProgressItem);
			return;
		}
	}
}
