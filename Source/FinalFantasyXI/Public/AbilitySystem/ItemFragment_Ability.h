// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Item/Item.h"
#include "Item/ItemDefinition.h"
#include "ItemFragment_Ability.generated.h"

class UCrysGameplayAbilityDefinition;

USTRUCT(BlueprintType)
struct FItemShard_Ability : public FItemShard
{
	GENERATED_BODY()

	/** The current level of the ability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, SaveGame)
	int32 Level = 1;

	virtual bool IsMatching(const TInstancedStruct<FItemShard>& Fragment) const override;
};

USTRUCT(BlueprintType)
struct FItemFragment_Ability : public FItemFragment
{
	GENERATED_BODY()

	/** The GameplayAbilityDefinition to grant. */
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AssetBundles = "Ability,UI"))
	// TSoftObjectPtr<UCrysGameplayAbilityDefinition> AbilityDefinition;

	virtual void SetDefaultValues(TInstancedStruct<FItem>& Item) const override;
};
