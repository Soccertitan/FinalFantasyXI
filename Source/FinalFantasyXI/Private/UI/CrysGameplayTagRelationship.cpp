// Copyright Soccertitan 2025


#include "UI/CrysGameplayTagRelationship.h"

#include "CrysLogChannels.h"
#include "Settings/FinalFantasyXIGameData.h"
#include "System/CrysAssetManager.h"

FCrysGameplayTagRelationshipItem UCrysGameplayTagRelationship::FindAttributeRelationshipItem(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FCrysGameplayTagRelationshipItem& Item : CrysGameplayTagRelationshipItems)
	{
		if (Item.Tag == Tag)
		{
			return Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for Tag [%s] in UCrysGameplayTagRelationship [%s]"), *Tag.ToString(), *GetNameSafe(this));
	}
	return FCrysGameplayTagRelationshipItem();
}

FCrysGameplayTagRelationshipItem UCrysGameplayTagRelationship::FindCrysGameplayTagRelationshipItem(const FGameplayTag& Tag, bool bLogNotFound)
{
	const UCrysGameplayTagRelationship* CrysGameplayTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UFinalFantasyXIGameData>()->CrysGameplayTagRelationship);
	if (!CrysGameplayTagRelationship)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("CrysGameplayTagRelationship is invalid in [%s]"), *GetDefault<UFinalFantasyXIGameData>()->GetName());
		}
		return FCrysGameplayTagRelationshipItem();
	}

	return CrysGameplayTagRelationship->FindAttributeRelationshipItem(Tag, bLogNotFound);
}
