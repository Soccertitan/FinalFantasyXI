// Copyright Soccertitan 2025


#include "AbilitySystem/AttributeTagRelationship.h"

#include "CrysLogChannels.h"

bool FAttributeRelationshipItem::IsValid() const
{
	if (GameplayAttribute.IsValid())
	{
		return true;
	}
	return false;
}

UAttributeTagRelationship::UAttributeTagRelationship()
{
	AssetType = TEXT("AttributeTagRelationship");
}

FPrimaryAssetId UAttributeTagRelationship::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(AssetType, GetFName());
}

FAttributeRelationshipItem UAttributeTagRelationship::FindAttributeRelationshipItem(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAttributeRelationshipItem& Item : AttributeTagRelationships)
	{
		if (Item.AttributeTag == AttributeTag)
		{
			return Item;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("Cannot find info for AttributeTag [%s] in UAttributeTagRelationship [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAttributeRelationshipItem();
}
