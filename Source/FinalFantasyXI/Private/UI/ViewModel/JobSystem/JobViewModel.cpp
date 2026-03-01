// Copyright Soccertitan 2025


#include "UI/ViewModel/JobSystem/JobViewModel.h"

#include "JobSystem/JobDefinition.h"

FText UJobViewModel::GetJobName() const
{
	return JobDefinition ? JobDefinition->JobName : FText();
}

FText UJobViewModel::GetShortName() const
{
	return JobDefinition ? JobDefinition->ShortName : FText();
}

FText UJobViewModel::GetDescription() const
{
	return JobDefinition ? JobDefinition->Description : FText();
}

int32 UJobViewModel::GetExperienceRequiredUntilNextLevel() const
{
	if (JobDefinition)
	{
		const int32 RequiredExp = JobDefinition->ExperienceRequirement.GetValueAtLevel(JobProgressItem.Level);
		return FMath::Max(RequiredExp - JobProgressItem.Experience, 0);
	}
	return 0;
}

float UJobViewModel::GetPercentageTowardsNextLevel() const
{
	if (JobDefinition)
	{
		const float PreviousLevelRequirement = JobDefinition->ExperienceRequirement.GetValueAtLevel(JobProgressItem.Level - 1);
		const float NextLevelRequirement = JobDefinition->ExperienceRequirement.GetValueAtLevel(JobProgressItem.Level);
		
		const float ExperienceIntoNextLevel = JobProgressItem.Experience - PreviousLevelRequirement;
		const float TotalExperienceRequired = NextLevelRequirement - PreviousLevelRequirement;
		
		if (TotalExperienceRequired > 0)
		{
			return FMath::Clamp(ExperienceIntoNextLevel / TotalExperienceRequired, 0.f, 1.f);
		}
	}

	return 0.f;
}

void UJobViewModel::SetJobAndProgress(UJobDefinition* InHeroJob, const FJobProgressItem& InHeroJobProgressItem)
{
	SetJobDefinition(InHeroJob);
	SetJobProgressItem(InHeroJobProgressItem);
}

void UJobViewModel::SetIsJobAvailable(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bJobAvailable, InValue);
}

void UJobViewModel::SetIsMainJob(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bMainJob, InValue);
}

void UJobViewModel::SetIsSubJob(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSubJob, InValue);
}

void UJobViewModel::SetJobDefinition(UJobDefinition* InHeroJob)
{
	JobDefinition = InHeroJob;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetJobName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
}

void UJobViewModel::SetJobProgressItem(const FJobProgressItem& InHeroJobProgressItem)
{
	JobProgressItem = InHeroJobProgressItem;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperience);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperienceRequiredUntilNextLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentageTowardsNextLevel);
	SetIsJobAvailable(JobProgressItem.IsValid());
}
