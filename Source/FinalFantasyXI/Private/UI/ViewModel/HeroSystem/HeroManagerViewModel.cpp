// Copyright Soccertitan 2025


#include "UI/ViewModel/HeroSystem/HeroManagerViewModel.h"

#include "Engine/AssetManager.h"
#include "GameFramework/PlayerState.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
#include "UI/ViewModel/HeroSystem/HeroJobViewModel.h"

UHeroJobViewModel* UHeroManagerViewModel::FindHeroJobViewModel(FGameplayTag HeroJobTag)
{
	if (HeroJobTag.IsValid())
	{
		for (const TObjectPtr<UHeroJobViewModel>& HeroJobViewModel : HeroJobViewModels)
		{
			if (HeroJobViewModel->GetHeroJob()->JobTag.MatchesTagExact(HeroJobTag))
			{
				return HeroJobViewModel;
			}
		}
	}
	return nullptr;
}

void UHeroManagerViewModel::TrySetHeroJobs(FGameplayTag MainJobTag, FGameplayTag SubJobTag)
{
	if (!MainJobTag.IsValid() || !HeroManagerComponent || bSwitchingHeroJobs || MainJobTag == SubJobTag)
	{
		return;
	}
	
	UHeroJobDefinition* NewMainJob = nullptr;
	UHeroJobDefinition* NewSubJob = nullptr;

	for (const TObjectPtr<UHeroJobViewModel>& HeroJobViewModel : HeroJobViewModels)
	{
		if (HeroJobViewModel->GetHeroJob()->JobTag.MatchesTagExact(MainJobTag))
		{
			NewMainJob = HeroJobViewModel->GetHeroJob();
			break;
		}
	}
	
	if (SubJobTag.IsValid())
	{
		for (const TObjectPtr<UHeroJobViewModel>& HeroJobViewModel : HeroJobViewModels)
		{
			if (HeroJobViewModel->GetHeroJob()->JobTag.MatchesTagExact(SubJobTag))
			{
				NewSubJob = HeroJobViewModel->GetHeroJob();
				break;
			}
		}
	}

	if (NewMainJob)
	{
		UE_MVVM_SET_PROPERTY_VALUE(bSwitchingHeroJobs, true);
		HeroManagerComponent->TrySetHeroJobs(NewMainJob, NewSubJob);
	}
}

void UHeroManagerViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	HeroManagerComponent = UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(PlayerController->GetPlayerState<APlayerState>());

	if (HeroManagerComponent)
	{
		HeroManagerComponent->OnHeroMainJobChangedDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnHeroMainJobChanged);
		HeroManagerComponent->OnHeroSubJobChangedDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnHeroSubJobChanged);
		HeroManagerComponent->OnTrySetHeroJobDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnTrySetHeroJob);
		HeroManagerComponent->OnHeroJobProgressUpdatedDelegate.AddUniqueDynamic(this, &UHeroManagerViewModel::OnHeroJobProgressUpdated);
		CreateHeroJobViewModel(HeroManagerComponent->GetHeroMainJob());
		CreateHeroJobViewModel(HeroManagerComponent->GetHeroSubJob());
		LoadHeroJobs();
	}
}

void UHeroManagerViewModel::SetIsLoadingHeroJobs(const bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bLoadingHeroJobs, InValue);
}

void UHeroManagerViewModel::SetHeroMainJobViewModel(UHeroJobViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsMainJob(true);
	}
	HeroMainJobViewModel = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHeroMainJobViewModel);
}

void UHeroManagerViewModel::SetHeroSubJobViewModel(UHeroJobViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsSubJob(false);
	}
	HeroSubJobViewModel = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHeroSubJobViewModel);
}

void UHeroManagerViewModel::LoadHeroJobs()
{
	FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UHeroManagerViewModel::OnHeroJobsLoaded);
	HeroJobStreamableHandle = UAssetManager::Get().PreloadPrimaryAssets(HeroJobsToLoad, {}, false, Delegate);
}

void UHeroManagerViewModel::OnHeroJobsLoaded()
{
	for (const FPrimaryAssetId& AssetId : HeroJobsToLoad)
	{
		if (UHeroJobDefinition* HeroJob = Cast<UHeroJobDefinition>(UAssetManager::Get().GetPrimaryAssetObject(AssetId)))
		{
			CreateHeroJobViewModel(HeroJob);
		}
	}
	UE_MVVM_SET_PROPERTY_VALUE(bLoadingHeroJobs, false);
	HeroJobStreamableHandle.Reset();
}

void UHeroManagerViewModel::CreateHeroJobViewModel(UHeroJobDefinition* HeroJob)
{
	if (HeroJob)
	{
		UHeroJobViewModel* HeroJobViewModel = FindHeroJobViewModel(HeroJob->JobTag);
		if (!HeroJobViewModel)
		{
			HeroJobViewModel = NewObject<UHeroJobViewModel>(this, UHeroJobViewModel::StaticClass());
			const FHeroJobProgressItem ProgressItem = HeroManagerComponent->FindHeroJobProgressItem(HeroJob->JobTag);
			HeroJobViewModel->SetHeroJobAndProgress(HeroJob, ProgressItem);
			if (HeroManagerComponent->GetHeroMainJob() == HeroJob)
			{
				SetHeroMainJobViewModel(HeroJobViewModel);
			}
			if (HeroManagerComponent->GetHeroSubJob() == HeroJob)
			{
				SetHeroSubJobViewModel(HeroJobViewModel);
			}
			HeroJobViewModels.Add(HeroJobViewModel);
		}
	}
}

void UHeroManagerViewModel::OnHeroMainJobChanged(UHeroManagerComponent* InHeroManagerComponent)
{
	UHeroJobDefinition* MainJob = InHeroManagerComponent->GetHeroMainJob();

	bool bFoundExistingHeroJob = false;
	for (const TObjectPtr<UHeroJobViewModel>& HeroJobViewModel : HeroJobViewModels)
	{
		if (HeroJobViewModel->GetHeroJob() == MainJob)
		{
			SetHeroMainJobViewModel(HeroJobViewModel);
			bFoundExistingHeroJob = true;
		}
		else
		{
			HeroJobViewModel->SetIsMainJob(false);
		}
	}

	if (!bFoundExistingHeroJob)
	{
		CreateHeroJobViewModel(MainJob);
	}
}

void UHeroManagerViewModel::OnHeroSubJobChanged(UHeroManagerComponent* InHeroManagerComponent)
{
	UHeroJobDefinition* SubJob = InHeroManagerComponent->GetHeroSubJob();

	bool bFoundExistingHeroJob = false;
	for (const TObjectPtr<UHeroJobViewModel>& HeroJobViewModel : HeroJobViewModels)
	{
		if (HeroJobViewModel->GetHeroJob() == SubJob)
		{
			SetHeroSubJobViewModel(HeroJobViewModel);
			bFoundExistingHeroJob = true;
		}
		else
		{
			HeroJobViewModel->SetIsSubJob(false);
		}
	}

	if (!bFoundExistingHeroJob)
	{
		CreateHeroJobViewModel(SubJob);
	}
}

void UHeroManagerViewModel::OnTrySetHeroJob(UHeroManagerComponent* InHeroManagerComponent, bool bSuccess)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSwitchingHeroJobs, false);
}

void UHeroManagerViewModel::OnHeroJobProgressUpdated(UHeroManagerComponent* InHeroManagerComponent,
	const FHeroJobProgressItem& HeroJobProgressItem)
{
	for (const TObjectPtr<UHeroJobViewModel>& HeroJobViewModel : HeroJobViewModels)
	{
		if (HeroJobViewModel->GetHeroJob()->JobTag.MatchesTagExact(HeroJobProgressItem.JobTag))
		{
			HeroJobViewModel->SetHeroJobProgressItem(HeroJobProgressItem);
			return;
		}
	}
}
