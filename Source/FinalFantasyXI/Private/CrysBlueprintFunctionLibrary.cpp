// Copyright Soccertitan 2025


#include "CrysBlueprintFunctionLibrary.h"

#include "CrysLogChannels.h"
#include "AbilitySystem/AbilityTargetInterface.h"
#include "Character/CrysCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "Settings/FinalFantasyXIGameData.h"
#include "System/CrysAssetManager.h"

ACrysPlayerController* UCrysBlueprintFunctionLibrary::GetCrysPlayerController(AActor* Actor)
{
	if (APawn* Pawn = Cast<APawn>(Actor))
	{
		return Cast<ACrysPlayerController>(Pawn->GetController());
	}

	if (APlayerState* PlayerState = Cast<APlayerState>(Actor))
	{
		return Cast<ACrysPlayerController>(PlayerState->GetPlayerController());
	}
	
	return Cast<ACrysPlayerController>(Actor);
}

ACrysPlayerState* UCrysBlueprintFunctionLibrary::GetCrysPlayerState(AActor* Actor)
{
	ACrysPlayerState* Result = Cast<ACrysPlayerState>(Actor);
	
	if (Result)
	{
		return Result;
	}
	
	if (APawn* Pawn = Cast<APawn>(Actor))
	{
		Result = Pawn->GetPlayerState<ACrysPlayerState>();
		if (Result)
		{
			return Result;
		}
	}

	if (AController* Controller = Cast<AController>(Actor))
	{
		Result = Controller->GetPlayerState<ACrysPlayerState>();
		if (Result)
		{
			return Result;
		}
	}

	return Result;
}

ACrysCharacter* UCrysBlueprintFunctionLibrary::GetCrysCharacter(AActor* Actor)
{
	ACrysCharacter* Result = Cast<ACrysCharacter>(Actor);
	
	if (Result)
	{
		return Result;
	}
	
	if (APlayerController* PC = Cast<APlayerController>(Actor))
	{
		return PC->GetPawn<ACrysCharacter>();
	}
	
	if (APlayerState* PS = Cast<APlayerState>(Actor))
	{
		return PS->GetPawn<ACrysCharacter>();
	}
	
	return Result;
}

AActor* UCrysBlueprintFunctionLibrary::GetAbilityTarget(const AActor* Actor, const FGameplayTagContainer& AbilityTags)
{
	if (Actor && Actor->Implements<UAbilityTargetInterface>())
	{
		return IAbilityTargetInterface::Execute_GetAbilityTarget(Actor, AbilityTags);
	}
	return nullptr;
}

FAttributeRelationshipItem UCrysBlueprintFunctionLibrary::FindAttributeRelationshipItem(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	const UAttributeTagRelationship* AttributeTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UFinalFantasyXIGameData>()->AttributeTagRelationship);
	if (!AttributeTagRelationship)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("AttributeTagRelationship is invalid in [%s]"), *GetDefault<UFinalFantasyXIGameData>()->GetName());
		}
		return FAttributeRelationshipItem();
	}

	return AttributeTagRelationship->FindAttributeRelationshipItem(AttributeTag, bLogNotFound);
}

bool UCrysBlueprintFunctionLibrary::IsAttributeRelationshipItemValid(const FAttributeRelationshipItem& AttributeRelationshipItem)
{
	return AttributeRelationshipItem.IsValid();
}
