// Copyright Soccertitan 2025


#include "JobSystem/JobTypes.h"

#include "CrysGameplayTags.h"
#include "JobSystem/JobManagerComponent.h"
#include "JobSystem/RaceDefinition.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"


void FJobProgressItem::PostReplicatedAdd(const FJobProgressContainer& InSerializer)
{
	InSerializer.Owner->OnJobProgressUpdatedDelegate.Broadcast(*this);
}

void FJobProgressItem::PostReplicatedChange(const FJobProgressContainer& InSerializer)
{
	InSerializer.Owner->OnJobProgressUpdatedDelegate.Broadcast(*this);
}

bool FJobProgressItem::IsValid() const
{
	if (!JobTag.IsValid())
	{
		return false;
	}
	
	return true;
}

void FJobProgressContainer::AddJobProgressItem(const FJobProgressItem& NewItem)
{
	FJobProgressItem& Item = Items.AddDefaulted_GetRef();
	Item.JobTag = NewItem.JobTag;
	Item.Level = NewItem.Level;
	Item.Experience = NewItem.Experience;

	MarkItemDirty(Item);
	Owner->OnJobProgressUpdatedDelegate.Broadcast(NewItem);
}

void FJobProgressContainer::AddExperience(const FGameplayTag& JobTag,
	const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	for (FJobProgressItem& Item : Items)
	{
		if (Item.JobTag.MatchesTagExact(JobTag))
		{
			Internal_AddExperience(Item, ExperienceRequirement, Experience);
			return;
		}
	}

	FJobProgressItem& NewItem = Items.AddDefaulted_GetRef();
	NewItem.JobTag = JobTag;
	NewItem.Level = 1;
	Internal_AddExperience(NewItem, ExperienceRequirement, Experience);
}

FJobProgressItem FJobProgressContainer::GetJobProgressItem(const FGameplayTag& JobTag) const
{
	for (const FJobProgressItem& Item : Items)
	{
		if (Item.JobTag.MatchesTagExact(JobTag))
		{
			return Item;
		}
	}
	return FJobProgressItem();
}

void FJobProgressContainer::Reset()
{
	Items.Empty();
	MarkArrayDirty();
}

void FJobProgressContainer::Internal_AddExperience(FJobProgressItem& Item, const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	const int32 OldLevel = Item.Level;
	const bool bLeveledUp = UJobSystemBlueprintFunctionLibrary::AddExperience(
		Item.Level, Item.Experience, Experience, ExperienceRequirement, Owner->GetJobManagerData().MaxJobLevel);
	MarkItemDirty(Item);
	Owner->OnJobProgressUpdatedDelegate.Broadcast(Item);
	if (bLeveledUp)
	{
		Owner->Internal_OnJobLevelUp(Item, OldLevel);
	}
}

FPrimaryAttributesCalculated::FPrimaryAttributesCalculated(const URaceDefinition* RaceDefinition,
    int32 Level, const UJobDefinition* MainJob, int32 MainJobRank, const UJobDefinition* SubJob,
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

int32 FPrimaryAttributesCalculated::CalculateValue(int32 RaceValue, float MainJobMultiplier, float SubJobMultiplier, float SubJobEfficiency)
{
	return FMath::Floor(RaceValue * (MainJobMultiplier + (SubJobMultiplier * SubJobEfficiency)));
}
