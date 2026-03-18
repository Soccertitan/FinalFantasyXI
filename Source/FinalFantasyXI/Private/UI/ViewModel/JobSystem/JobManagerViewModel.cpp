// Copyright Soccertitan 2025


#include "UI/ViewModel/JobSystem/JobManagerViewModel.h"

#include "Engine/AssetManager.h"
#include "GameFramework/PlayerState.h"
#include "JobSystem/JobManagerComponent.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"
#include "UI/ViewModel/JobSystem/JobViewModel.h"

bool UJobManagerViewModel::IsSubJobEquipped() const
{
	return SubJobViewModel ? true : false;
}

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

void UJobManagerViewModel::TrySetJobs(UJobViewModel* InMainJobViewModel, UJobViewModel* InSubJobViewModel)
{
	if (!InMainJobViewModel || !JobManagerComponent || bSwitchingJobs)
	{
		return;
	}
	
	UJobDefinition* NewMainJob = InMainJobViewModel->GetJob();
	UJobDefinition* NewSubJob = InSubJobViewModel ? InSubJobViewModel->GetJob() : nullptr;

	if (NewMainJob)
	{
		UE_MVVM_SET_PROPERTY_VALUE(bSwitchingJobs, true);
		JobManagerComponent->TrySetJobs(NewMainJob, NewSubJob);
	}
}

void UJobManagerViewModel::TrySetMainJob(UJobViewModel* JobViewModel)
{
	if (!JobViewModel || !JobManagerComponent || bSwitchingJobs)
	{
		return;
	}
	
	if (JobManagerComponent->GetSubJob() == JobViewModel->GetJob())
	{
		JobManagerComponent->TrySetJobs(JobViewModel->GetJob(), JobManagerComponent->GetMainJob());
	}
	else
	{
		JobManagerComponent->TrySetJobs(JobViewModel->GetJob(), JobManagerComponent->GetSubJob());
	}
}

void UJobManagerViewModel::TrySetSubJob(UJobViewModel* JobViewModel)
{
	if (!JobViewModel || !JobManagerComponent || bSwitchingJobs)
	{
		return;
	}
	
	if (JobManagerComponent->GetMainJob() == JobViewModel->GetJob())
	{
		JobManagerComponent->TrySetJobs(JobManagerComponent->GetSubJob(), JobViewModel->GetJob());
	}
	else
	{
		JobManagerComponent->TrySetJobs( JobManagerComponent->GetMainJob(), JobViewModel->GetJob());
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
		JobManagerComponent->OnJobManagerDataUpdatedDelegate.AddUniqueDynamic(this, &UJobManagerViewModel::OnJobManagerDataUpdated);
		OnJobManagerDataUpdated();
		CreateJobViewModel(JobManagerComponent->GetMainJob());
		CreateJobViewModel(JobManagerComponent->GetSubJob());
		LoadJobs();
	}
}

void UJobManagerViewModel::SetMainJobViewModel(UJobViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsMainJob(true);
	}
	UE_MVVM_SET_PROPERTY_VALUE(MainJobViewModel, InValue);
}

void UJobManagerViewModel::SetSubJobViewModel(UJobViewModel* InValue)
{
	if (InValue)
	{
		InValue->SetIsSubJob(false);
	}
	UE_MVVM_SET_PROPERTY_VALUE(SubJobViewModel, InValue);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsSubJobEquipped);
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
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(JobsLoaded);
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
		if (SubJob)
		{
			CreateJobViewModel(SubJob);
		}
		else
		{
			SetSubJobViewModel(nullptr);
		}
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

void UJobManagerViewModel::OnJobManagerDataUpdated()
{
	JobManagerData = JobManagerComponent->GetJobManagerData();
	
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperience);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetMaxJobLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(IsSubJobUnlocked);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetSubJobEfficiency);
}
