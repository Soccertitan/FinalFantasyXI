// Copyright 2025 Soccertitan


#include "Character/AnimationTagRelationship.h"

#include "CrysLogChannels.h"

void FAnimationRelationshipItem::PostSerialize(const FArchive& Ar)
{
	EditorDisplayName = FString::Printf(TEXT("%s : %s"), *AnimationTag.ToString(), *Montage.GetAssetName());
}

bool FAnimationRelationshipItem::IsValid() const
{
	if (Montage.IsNull())
	{
		return false;
	}
	
	return true;
}

UAnimationTagRelationship::UAnimationTagRelationship()
{
	AssetType = TEXT("AnimationTagRelationship");
}

FPrimaryAssetId UAnimationTagRelationship::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

FAnimationRelationshipItem UAnimationTagRelationship::FindAnimationRelationshipItem(const FGameplayTag& AnimationTag, bool bLogNotFound) const
{
	for (const FAnimationRelationshipItem& Item : AnimationRelationshipItems)
	{
		if (Item.AnimationTag == AnimationTag)
		{
			return Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for AnimationTag [%s] in UAnimationTagRelationship [%s]"), *AnimationTag.ToString(), *GetNameSafe(this));
	}
	return FAnimationRelationshipItem();
}
