// Copyright Soccertitan 2025


#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"

#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroSystemInterface.h"

UHeroManagerComponent* UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(const AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return nullptr;
	}

	if (Actor->Implements<UHeroSystemInterface>())
	{
		return IHeroSystemInterface::Execute_GetHeroManagerComponent(Actor);
	}

	return Actor->FindComponentByClass<UHeroManagerComponent>();
}

bool UHeroSystemBlueprintFunctionLibrary::AddExperience(int32& InLevel, int32& InExperience, const int32 ExperienceToGrant, const FScalableFloat& ExperienceRequirement, int32 MaxLevel)
{
	if (ExperienceToGrant <= 0 || !ExperienceRequirement.IsValid())
	{
		return false;
	}
	
	if (MaxLevel <= -1)
	{
		MaxLevel = MAX_int32;
	}
	
	bool bAddingExp = true;
	const int32 NewExp = InExperience + ExperienceToGrant;
	bool bLevelUpdated = false;
	
	while (bAddingExp)
	{
		const int32 ExpRequired = ExperienceRequirement.AsInteger(InLevel);
		
		if (NewExp >= ExpRequired && InLevel < MaxLevel)
		{
			InLevel++;
			bLevelUpdated = true;
		}
		else
		{
			bAddingExp = false;
		}
	}
	
	InExperience = FMath::Min(NewExp, ExperienceRequirement.AsInteger(InLevel));
	return bLevelUpdated;
}

bool UHeroSystemBlueprintFunctionLibrary::SubtractExperience(int32& InLevel, int32& InExperience, const int32 ExperienceToRemove, const FScalableFloat& ExperienceRequirement)
{
	if (ExperienceToRemove <= 0 || !ExperienceRequirement.IsValid())
	{
		return false;
	}
	
	bool bSubtractingExp = true;
	const int32 NewExp = FMath::Max(0, InExperience - ExperienceToRemove);
	bool bLevelUpdated = false;
	
	while (bSubtractingExp)
	{
		const int32 ExpRequired = ExperienceRequirement.AsInteger(InLevel - 1);
		
		if (NewExp <= ExpRequired && InLevel > 0)
		{
			InLevel--;
			bLevelUpdated = true;
		}
		else
		{
			bSubtractingExp = false;
		}
	}
	
	InExperience = FMath::Clamp(NewExp, 0, ExperienceRequirement.AsInteger(InLevel));
	return bLevelUpdated;
}

void UHeroSystemBlueprintFunctionLibrary::SetLevel(int32& InLevel, int32& InExperience, const int32& NewLevel, const FScalableFloat& ExperienceRequirement)
{
	if (NewLevel < 0 || !ExperienceRequirement.IsValid())
	{
		return;
	}
	
	InLevel = NewLevel;
	InExperience = FMath::Max(0, ExperienceRequirement.AsInteger(NewLevel - 1));
}
