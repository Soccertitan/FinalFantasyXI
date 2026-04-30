// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "AbilityTypes.generated.h"

/**
 * Added to the GameplayEffectContext during the DamageExecCalc.
 */
USTRUCT(BlueprintType)
struct FDamageGameplayContext
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	bool bHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bCriticalHit = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bGuarded = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bParried = false;
	
	UPROPERTY(BlueprintReadOnly)
	bool bBlocked = false;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttribute DamagedAttribute;
};

/** Defines who the ability can target. */
UENUM(BlueprintType)
enum class EAbilityTargetType : uint8
{
	Any,
	SelfOnly,
	FriendlyExcludeSelf,
	Friendly,
	Hostile
};

USTRUCT()
struct FGameplayAbilityTargetData_SingleTarget : public FGameplayAbilityTargetData
{
	GENERATED_BODY()
	
	UPROPERTY()
	TObjectPtr<AActor> TargetedActor;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_SingleTarget::StaticStruct();
	}

	virtual FString ToString() const override
	{
		return TEXT("FGameplayAbilityTargetData_SingleTarget");
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};
template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_SingleTarget> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_SingleTarget>
{
	enum
	{
		WithNetSerializer = true // This is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
