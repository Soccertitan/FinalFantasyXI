// Copyright Soccertitan 2025


#include "HeroSystem/HeroTypes.h"

#include "CrysGameplayTags.h"
#include "HeroSystem/HeroManagerComponent.h"
#include "HeroSystem/HeroRaceDefinition.h"


void FHeroClassProgressItem::PostReplicatedAdd(const FHeroClassProgressContainer& InSerializer)
{
	InSerializer.Owner->OnHeroClassProgressUpdatedDelegate.Broadcast(InSerializer.Owner, *this);
}

void FHeroClassProgressItem::PostReplicatedChange(const FHeroClassProgressContainer& InSerializer)
{
	InSerializer.Owner->OnHeroClassProgressUpdatedDelegate.Broadcast(InSerializer.Owner, *this);
}

bool FHeroClassProgressItem::IsValid() const
{
	if (!HeroClassTag.IsValid())
	{
		return false;
	}

	if (Level < 1)
	{
		return false;
	}

	if (Experience < 0)
	{
		return false;
	}
	
	return true;
}

void FHeroClassProgressContainer::AddHeroClassProgressItem(const FHeroClassProgressItem& NewItem)
{
	FHeroClassProgressItem& Item = Items.AddDefaulted_GetRef();
	Item.HeroClassTag = NewItem.HeroClassTag;
	Item.Level = NewItem.Level;
	Item.Experience = NewItem.Experience;

	MarkItemDirty(Item);
	Owner->OnHeroClassProgressUpdatedDelegate.Broadcast(Owner, NewItem);
}

void FHeroClassProgressContainer::AddExperience(const FGameplayTag& HeroClassTag,
	const FScalableFloat& ExperienceRequirement, int32 Experience)
{
	for (FHeroClassProgressItem& Item : Items)
	{
		if (Item.HeroClassTag.MatchesTagExact(HeroClassTag))
		{
			bool bAddingExp = true;
			const int32 NewExp = Item.Experience + Experience;
			int32 NewLevel = Item.Level;
			const int32 OldLevel = Item.Level;
			bool bLeveledUp = false;

			while (bAddingExp)
			{
				const int32 ExpRequired = ExperienceRequirement.GetValueAtLevel(NewLevel);

				if (NewExp >= ExpRequired && NewLevel < Owner->GetMaxLevel())
				{
					NewLevel++;
					bLeveledUp = true;
				}
				else
				{
					bAddingExp = false;
				}
			}

			Item.Experience = FMath::Min(NewExp, ExperienceRequirement.GetValueAtLevel(NewLevel));
			Item.Level = NewLevel;
			MarkItemDirty(Item);
			Owner->OnHeroClassProgressUpdatedDelegate.Broadcast(Owner, Item);
			if (bLeveledUp)
			{
				Owner->Internal_OnLevelUp(Item, OldLevel);
			}
			return;
		}
	}
}

FHeroClassProgressItem FHeroClassProgressContainer::GetHeroClassProgressItem(const FGameplayTag& HeroClassTag) const
{
	for (const FHeroClassProgressItem& Item : Items)
	{
		if (Item.HeroClassTag.MatchesTagExact(HeroClassTag))
		{
			return Item;
		}
	}
	return FHeroClassProgressItem();
}

void FHeroClassProgressContainer::Reset()
{
	Items.Empty();
	MarkArrayDirty();
}

FHeroBaseAttributesCalculated::FHeroBaseAttributesCalculated(const UHeroClassDefinition* HeroClass,
	const UHeroRaceDefinition* HeroRace, int32 Level)
{
	const bool bHeroClassValid = HeroClass ? true : false;
	const bool bHeroRaceValid = HeroRace ? true : false;

	MaxHitPoints = (bHeroClassValid ? HeroClass->BaseAttributes.MaxHitPoints.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.MaxHitPoints.GetValueAtLevel(Level) : 0);

	MaxTacticalPoints = (bHeroClassValid ? HeroClass->BaseAttributes.MaxTacticalPoints.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.MaxTacticalPoints.GetValueAtLevel(Level) : 0);

	MeleeAttack = (bHeroClassValid ? HeroClass->BaseAttributes.MeleeAttack.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.MeleeAttack.GetValueAtLevel(Level) : 0);

	MeleeDefense = (bHeroClassValid ? HeroClass->BaseAttributes.MeleeDefense.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.MeleeDefense.GetValueAtLevel(Level) : 0);

	RangeAttack = (bHeroClassValid ? HeroClass->BaseAttributes.RangeAttack.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.RangeAttack.GetValueAtLevel(Level) : 0);

	RangeDefense = (bHeroClassValid ? HeroClass->BaseAttributes.RangeDefense.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.RangeDefense.GetValueAtLevel(Level) : 0);

	TechniqueAttack = (bHeroClassValid ? HeroClass->BaseAttributes.TechniqueAttack.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.TechniqueAttack.GetValueAtLevel(Level) : 0);

	TechniqueDefense = (bHeroClassValid ? HeroClass->BaseAttributes.TechniqueDefense.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.TechniqueDefense.GetValueAtLevel(Level) : 0);

	Dexterity = (bHeroClassValid ? HeroClass->BaseAttributes.Dexterity.GetValueAtLevel(Level) : 1) +
		(bHeroRaceValid ? HeroRace->BaseAttributes.Dexterity.GetValueAtLevel(Level) : 0);
}
