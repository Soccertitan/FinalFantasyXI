// Copyright Soccertitan 2025


#include "HeroSystem/HeroTypes.h"

#include "CrysGameplayTags.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroRaceDefinition.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"


void FHeroJobProgressItem::PostReplicatedAdd(const FHeroJobProgressContainer& InSerializer)
{
	InSerializer.Owner->OnHeroJobProgressUpdatedDelegate.Broadcast(*this);
}

void FHeroJobProgressItem::PostReplicatedChange(const FHeroJobProgressContainer& InSerializer)
{
	InSerializer.Owner->OnHeroJobProgressUpdatedDelegate.Broadcast(*this);
}

bool FHeroJobProgressItem::IsValid() const
{
	if (!JobTag.IsValid())
	{
		return false;
	}
	
	return true;
}

void FHeroJobProgressContainer::AddHeroJobProgressItem(const FHeroJobProgressItem& NewItem)
{
	FHeroJobProgressItem& Item = Items.AddDefaulted_GetRef();
	Item.JobTag = NewItem.JobTag;
	Item.Level = NewItem.Level;
	Item.Experience = NewItem.Experience;

	MarkItemDirty(Item);
	Owner->OnHeroJobProgressUpdatedDelegate.Broadcast(NewItem);
}

void FHeroJobProgressContainer::AddExperience(const FGameplayTag& JobTag,
	const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	for (FHeroJobProgressItem& Item : Items)
	{
		if (Item.JobTag.MatchesTagExact(JobTag))
		{
			Internal_AddExperience(Item, ExperienceRequirement, Experience);
			return;
		}
	}

	FHeroJobProgressItem& NewItem = Items.AddDefaulted_GetRef();
	NewItem.JobTag = JobTag;
	NewItem.Level = 1;
	Internal_AddExperience(NewItem, ExperienceRequirement, Experience);
}

FHeroJobProgressItem FHeroJobProgressContainer::GetHeroJobProgressItem(const FGameplayTag& JobTag) const
{
	for (const FHeroJobProgressItem& Item : Items)
	{
		if (Item.JobTag.MatchesTagExact(JobTag))
		{
			return Item;
		}
	}
	return FHeroJobProgressItem();
}

void FHeroJobProgressContainer::Reset()
{
	Items.Empty();
	MarkArrayDirty();
}

void FHeroJobProgressContainer::Internal_AddExperience(FHeroJobProgressItem& Item, const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	const int32 OldLevel = Item.Level;
	const bool bLeveledUp = UHeroSystemBlueprintFunctionLibrary::AddExperience(
		Item.Level, Item.Experience, Experience, ExperienceRequirement, Owner->GetHeroProgress().MaxJobLevel);
	MarkItemDirty(Item);
	Owner->OnHeroJobProgressUpdatedDelegate.Broadcast(Item);
	if (bLeveledUp)
	{
		Owner->Internal_OnJobLevelUp(Item, OldLevel);
	}
}

FHeroPrimaryAttributesCalculated::FHeroPrimaryAttributesCalculated(const UHeroRaceDefinition* RaceDefinition,
    int32 Level, const UHeroJobDefinition* MainJob, int32 MainJobRank, const UHeroJobDefinition* SubJob,
	int32 SubJobRank, float SubJobEfficiency)
{
	const bool bRaceValid = RaceDefinition ? true : false;
	const bool bMainJobValid = MainJob ? true : false;
	const bool bSubJobValid = SubJob ? true : false;
	
	MaxHitPoints = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.MaxHitPoints.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.MaxHitPoints.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.MaxHitPoints.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	MaxMagicPoints = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.MaxMagicPoints.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.MaxMagicPoints.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.MaxMagicPoints.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Strength = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Strength.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Strength.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Strength.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Vitality = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Vitality.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Vitality.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Vitality.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Dexterity = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Dexterity.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Dexterity.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Dexterity.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Agility = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Agility.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Agility.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Agility.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Intelligence = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Intelligence.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Intelligence.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Intelligence.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Mind = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Mind.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Mind.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Mind.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
	
	Charisma = CalculateValue(bRaceValid ? RaceDefinition->BaseAttributes.Charisma.GetValueAtLevel(Level) : 1,
		bMainJobValid ? MainJob->BaseAttributesMultiplier.Charisma.GetValueAtLevel(MainJobRank) : 1,
		bSubJobValid ? SubJob->BaseAttributesMultiplier.Charisma.GetValueAtLevel(SubJobRank) : 0,
		SubJobEfficiency);
}

int32 FHeroPrimaryAttributesCalculated::CalculateValue(int32 RaceValue, float MainJobMultiplier, float SubJobMultiplier, float SubJobEfficiency)
{
	return FMath::Floor(RaceValue * (MainJobMultiplier + (SubJobMultiplier * SubJobEfficiency)));
}
