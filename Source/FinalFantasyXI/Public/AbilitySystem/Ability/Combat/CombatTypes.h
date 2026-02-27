// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

#include "CombatTypes.generated.h"

class UGameplayEffect;
/**
 * Used in abilities to determine power, range, etc..
 */
USTRUCT(BlueprintType)
struct FCrysAbilityParams
{
	GENERATED_BODY()
	
	/** Base power of the ability. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat BasePower;
	
	/** The 'cast time' of time ability. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat CastTime;
	
	/** The maximum range of the ability.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Range;
	
	/** The target must be within the half angle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Angle;
};

/** Runtime data used for combat/auto attacks. */
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_BODY()
	
	/** The level of the weapon that affects the GE and all the other scalable floats. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 0;
	
	/** The skill the weapon uses to determine effectiveness. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "WeaponSkill"))
	FGameplayTag WeaponSkill;
	
	/** The base damage of the weapon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Damage;
	
	/** The auto attack range. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Range;
	
	/** Base auto attack delay in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FScalableFloat Delay;
	
	/** The effect class to use during an auto attack. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> AutoAttackGameplayEffectClass;
	
	bool IsValid() const;
};
