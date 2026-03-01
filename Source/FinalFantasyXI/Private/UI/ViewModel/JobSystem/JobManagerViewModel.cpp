// Copyright Soccertitan 2025


#include "UI/ViewModel/JobSystem/JobManagerViewModel.h"

#include "Engine/AssetManager.h"
#include "GameFramework/PlayerState.h"
#include "JobSystem/JobManagerComponent.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"
#include "UI/ViewModel/JobSystem/JobViewModel.h"

UJobViewModel* UJobManagerViewModel::FindJobViewModel(FGameplayTag JobTag)
{
	if (JobTag.IsValid())
	{
		for (const TObjectPtr<UJobViewModel>& JobViewModel : JobViewModels)
		{
			if (JobViewModel->GetJob()->JobTag.MatchesTagExact(JobTag))
			{
				return JobViewModel;
			}
		}
	}
	return nullptr;
}

void UJobManagerViewModel::TrySetJobs(FGameplayTag MainJobTag, FGameplayTag SubJobTag)
{
	if (!MainJobTag.IsValid() || !JobManagerComponent || bSwitchingJobs || MainJobTag == SubJobTag)
	{
		return;
	}
	
	UJobDefinition* NewMainJob = nullptr;
	UJobDefinition* NewSubJob = nullptr;

	for (const TObjectPtr<UJobViewModel>& JobViewModel : JobViewModels)
	{
		if (JobViewModel->GetJob()->JobTag.MatchesTagExact(MainJobTag))
		{
			NewMainJob = JobViewModel->GetJob();
			break;
		}
	}
	
	if (SubJobTag.IsValid())
	{
		for (const TObjectPtr<UJobViewModel>& JobViewModel : JobViewModels)
		{
			if (JobViewModel->GetJob()->JobTag.MatchesTagExact(SubJobTag))
			{
				NewSubJob = JobViewModel->GetJob();
				break;
			}
		}
	}

	if (NewMainJob)
	{
		UE_MVVM_SET_PROPERTY_VALUE(bSwitchingJobs, true);
		JobManagerComponent->TrySetJobs(NewMainJob, NewSubJob);
	}
}

void UJobManagerViewModel::OnInitializeViewModel(APlayerController* PlayerController)
{
	JobManagerComponent = UJobSystemBlueprintFunctionLibrary::GetJobManagerComponent(PlayerController->GetPlayerState<APlayerState>());

	if (JobManagerComponent)
	{
		JobManagerComponent->OnMainJobChangedDelegate.AddUniqueDynamic(this, &UJobManagerViewModel::OnMainJobChanged);
		JobManagerComponent->OnSubJobChangedDelegate.AddUniqueDynamic(this, &UJobManagerViewModel::OnSubJobChanged);
		JobManagerComponent->OnTrySetJobDelegate.AddUniqueDynamic(this, &UJobManagerViewModel::OnTrySetJob);
		JobManagerComponent->OnJobProgressUpdatedDelegate.AddUniqueDynamic(this, &UJobManagerViewModel::OnJobProgressUpdated);
		CreateJobViewModel(JobManagerComponent->GetMainJob());
		CreateJobViewModel(JobManagerComponent->GetSubJob());
		LoadJobs();
	}
}

void UJobManagerViewModel::SetIsLoadingJobs(const bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bLoadingJobs, InValue);
}

void UJobManagerViewModel::SetMainJobViewModel(UJobViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsMainJob(true);
	}
	MainJobViewModel = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMainJobViewModel);
}

void UJobManagerViewModel::SetSubJobViewModel(UJobViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsSubJob(false);
	}
	SubJobViewModel = InValue;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetSubJobViewModel);
}

void UJobManagerViewModel::LoadJobs()
{
	FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UJobManagerViewModel::OnJobsLoaded);
	JobStreamableHandle = UAssetManager::Get().PreloadPrimaryAssets(JobsToLoad, {}, false, Delegate);
}

void UJobManagerViewModel::OnJobsLoaded()
{
	for (const FPrimaryAssetId& AssetId : JobsToLoad)
	{
		if (UJobDefinition* JobDefinition = Cast<UJobDefinition>(UAssetManager::Get().GetPrimaryAssetObject(AssetId)))
		{
			CreateJobViewModel(JobDefinition);
		}
	}
	UE_MVVM_SET_PROPERTY_VALUE(bLoadingJobs, false);
	JobStreamableHandle.Reset();
}

void UJobManagerViewModel::CreateJobViewModel(UJobDefinition* JobDefinition)
{
	if (JobDefinition)
	{
		UJobViewModel* JobViewModel = FindJobViewModel(JobDefinition->JobTag);
		if (!JobViewModel)
		{
			JobViewModel = NewObject<UJobViewModel>(this, UJobViewModel::StaticClass());
			const FJobProgressItem ProgressItem = JobManagerComponent->FindJobProgressItem(JobDefinition->JobTag);
			JobViewModel->SetJobAndProgress(JobDefinition, ProgressItem);
			if (JobManagerComponent->GetMainJob() == JobDefinition)
			{
				SetMainJobViewModel(JobViewModel);
			}
			if (JobManagerComponent->GetSubJob() == JobDefinition)
			{
				SetSubJobViewModel(JobViewModel);
			}
			JobViewModels.Add(JobViewModel);
		}
	}
}

void UJobManagerViewModel::OnMainJobChanged()
{
	UJobDefinition* MainJob = JobManagerComponent->GetMainJob();

	bool bFoundExistingJob = false;
	for (const TObjectPtr<UJobViewModel>& JobViewModel : JobViewModels)
	{
		if (JobViewModel->GetJob() == MainJob)
		{
			SetMainJobViewModel(JobViewModel);
			bFoundExistingJob = true;
		}
		else
		{
			JobViewModel->SetIsMainJob(false);
		}
	}

	if (!bFoundExistingJob)
	{
		CreateJobViewModel(MainJob);
	}
}

void UJobManagerViewModel::OnSubJobChanged()
{
	UJobDefinition* SubJob = JobManagerComponent->GetSubJob();

	bool bFoundExistingJob = false;
	for (const TObjectPtr<UJobViewModel>& JobViewModel : JobViewModels)
	{
		if (JobViewModel->GetJob() == SubJob)
		{
			SetSubJobViewModel(JobViewModel);
			bFoundExistingJob = true;
		}
		else
		{
			JobViewModel->SetIsSubJob(false);
		}
	}

	if (!bFoundExistingJob)
	{
		CreateJobViewModel(SubJob);
	}
}

void UJobManagerViewModel::OnTrySetJob(bool bSuccess)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSwitchingJobs, false);
}

void UJobManagerViewModel::OnJobProgressUpdated(const FJobProgressItem& JobProgressItem)
{
	for (const TObjectPtr<UJobViewModel>& JobViewModel : JobViewModels)
	{
		if (JobViewModel->GetJob()->JobTag.MatchesTagExact(JobProgressItem.JobTag))
		{
			JobViewModel->SetJobProgressItem(JobProgressItem);
			return;
		}
	}
}
