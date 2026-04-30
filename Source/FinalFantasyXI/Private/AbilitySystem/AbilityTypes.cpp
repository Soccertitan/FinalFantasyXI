// Copyright Soccertitan 2025


#include "AbilitySystem/AbilityTypes.h"


bool FGameplayAbilityTargetData_SingleTarget::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	Ar << TargetedActor;
	bOutSuccess = true;
	return true;
}
