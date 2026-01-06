// Copyright Soccertitan 2025


#include "UI/ViewModel/Component/HeroClassViewModel.h"

#include "HeroSystem/HeroClassDefinition.h"

FText UHeroClassViewModel::GetHeroClassName() const
{
	return HeroClassDefinition ? HeroClassDefinition->HeroClassName : FText();
}

FText UHeroClassViewModel::GetShortName() const
{
	return HeroClassDefinition ? HeroClassDefinition->ShortName : FText();
}

FText UHeroClassViewModel::GetDescription() const
{
	return HeroClassDefinition ? HeroClassDefinition->Description : FText();
}

int32 UHeroClassViewModel::GetExperienceRequiredUntilNextLevel() const
{
	if (HeroClassDefinition)
	{
		const int32 RequiredExp = HeroClassDefinition->ExperienceRequirement.GetValueAtLevel(HeroClassProgressItem.Level);
		return FMath::Max(RequiredExp - HeroClassProgressItem.Experience, 0);
	}
	return 0;
}

float UHeroClassViewModel::GetPercentageTowardsNextLevel() const
{
	if (HeroClassDefinition)
	{
		const float PreviousLevelRequirement = HeroClassDefinition->ExperienceRequirement.GetValueAtLevel(HeroClassProgressItem.Level - 1);
		const float NextLevelRequirement = HeroClassDefinition->ExperienceRequirement.GetValueAtLevel(HeroClassProgressItem.Level);
		
		const float ExperienceIntoNextLevel = HeroClassProgressItem.Experience - PreviousLevelRequirement;
		const float TotalExperienceRequired = NextLevelRequirement - PreviousLevelRequirement;
		
		if (TotalExperienceRequired > 0)
		{
			return FMath::Clamp(ExperienceIntoNextLevel / TotalExperienceRequired, 0.f, 1.f);
		}
	}

	return 0.f;
}

void UHeroClassViewModel::SetHeroClassAndProgress(UHeroClassDefinition* InHeroClass, const FHeroClassProgressItem& InHeroClassProgressItem)
{
	SetHeroClassDefinition(InHeroClass);
	SetHeroClassProgressItem(InHeroClassProgressItem);
}

void UHeroClassViewModel::SetIsHeroClassAvailable(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bHeroClassAvailable, InValue);
}

void UHeroClassViewModel::SetIsCurrentHeroClass(bool InValue)
{
	UE_MVVM_SET_PROPERTY_VALUE(bCurrentHeroClass, InValue);
}

void UHeroClassViewModel::SetHeroClassDefinition(UHeroClassDefinition* InHeroClass)
{
	HeroClassDefinition = InHeroClass;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHeroClassName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShortName);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetDescription);
}

void UHeroClassViewModel::SetHeroClassProgressItem(const FHeroClassProgressItem& InHeroClassProgressItem)
{
	HeroClassProgressItem = InHeroClassProgressItem;
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperience);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetExperienceRequiredUntilNextLevel);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetPercentageTowardsNextLevel);
	SetIsHeroClassAvailable(HeroClassProgressItem.IsValid());
}
