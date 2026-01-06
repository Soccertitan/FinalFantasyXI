// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Ability/GameplayAbilityDefinition.h"
#include "CrysGameplayAbilityDefinition.generated.h"


/**
 * Defines information about a GameplayAbility.
 */
UCLASS(Const)
class FINALFANTASYXI_API UCrysGameplayAbilityDefinition : public UGameplayAbilityDefinition
{
	GENERATED_BODY()

public:
	UCrysGameplayAbilityDefinition();
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext Context) const override;

	/** The classes allowed to use this ability. If empty, all classes allowed. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "HeroClass"))
	FGameplayTagContainer AllowedHeroClasses;
};
