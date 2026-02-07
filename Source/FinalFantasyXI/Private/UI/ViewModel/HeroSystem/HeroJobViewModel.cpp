// Copyright Soccertitan 2025


#include "UI/ViewModel/HeroSystem/HeroJobViewModel.h"

#include "HeroSystem/HeroJobDefinition.h"

FText UHeroJobViewModel::GetHeroJobName() const
{
	return HeroJobDefinition ? HeroJobDefinition->JobName : FText();
}

FText UHeroJobViewModel::GetShortName() const
{
	return HeroJobDefinition ? HeroJobDefinition->ShortName : FText();
}

FText UHeroJobViewModel::GetDescription() const
{
	return HeroJobDefinition ? HeroJobDefinition->Description : FText();
}

int32 UHeroJobViewModel::GetExperienceRequiredUntilNextLevel() const
{
	if (HeroJobDefinition)
	{
		const int32 RequiredExp = HeroJobDefinition->ExperienceRequirement.GetValueAtLevel(HeroJobProgressItem.Level);
		return FMath::Max(RequiredExp - HeroJobProgressItem.Experience, 0);
	}
	return 0;
}

float UHeroJobViewModel::GetPercentageTowardsNextLevel() const
{
	if (HeroJobDefinition)
	{
		const float PreviousLevelRequirement = HeroJobDefinition->ExperienceRequirement.GetValueAtLevel(HeroJobProgressItem.Level - 1);
		const float NextLevelRequirement = HeroJobDefinition->ExperienceRequirement.GetValueAtLevel(HeroJobProgressItem.Level);
		
		const float ExperienceIntoNextLevel = HeroJobProgressItem.Experience - PreviousLevelRequirement;
		const float TotalExperienceRequired = NextLevelRequirement - PreviousLevelRequirement;
		
		if (TotalExperienceRequired > 0)
		{
			return FMath::Clamp(ExperienceIntoNextLevel / TotalExperienceRequired, 0.f, 1.f);
		}
	}

	return 0.f;
}

void UHeroJobViewModel::SetHeroJobAndProgress(UHeroJobDefinition* InHeroJob, const FHeroJobProgressItem& InHeroJobProgressItem)
{
	SetHeroJobDefinition(InHeroJob);
	SetHeroJobProgressItem(InHeroJobProgressItem);
}

void UHeroJobViewModel::SetIsHeroJobAvailable(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bHeroJobAvailable, InValue);
}

void UHeroJobViewModel::SetIsMainJob(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bMainJob, InValue);
}

void UHeroJobViewModel::SetIsSubJob(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bSubJob, InValue);
}

void UHeroJobViewModel::SetHeroJobDefinition(UHeroJobDefinition* InHeroJob)
{
	HeroJobDefinition = InHeroJob;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHeroJobName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
}

void UHeroJobViewModel::SetHeroJobProgressItem(const FHeroJobProgressItem& InHeroJobProgressItem)
{
	HeroJobProgressItem = InHeroJobProgressItem;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperience);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperienceRequiredUntilNextLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentageTowardsNextLevel);
	SetIsHeroJobAvailable(HeroJobProgressItem.IsValid());
}
