// Copyright Soccertitan 2025


#include "CrysBlueprintFunctionLibrary.h"

#include "CrysLogChannels.h"
#include "Character/AnimationTagRelationship.h"
#include "Character/CrysCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Player/CrysPlayerController.h"
#include "Player/CrysPlayerState.h"
#include "Settings/PhantasyStarOnlineGameData.h"
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

FAnimationRelationshipItem UCrysBlueprintFunctionLibrary::FindAnimationRelationshipItem(AActor* Actor, const FGameplayTag& AnimationTag, bool bLogNotFound)
{
	if (const ACrysCharacter* Character = GetCrysCharacter(Actor))
	{
		if (UAnimationTagRelationship* AnimationTagRelationship = Character->GetAnimationTagRelationship())
		{
			return AnimationTagRelationship->FindAnimationRelationshipItem(AnimationTag, bLogNotFound);
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogCrys, Error, TEXT("AnimationTagRelationship is invalid in [%s]"), *GetNameSafe(Actor));
	}
	return FAnimationRelationshipItem();
}

bool UCrysBlueprintFunctionLibrary::IsAnimationRelationshipItemValid(const FAnimationRelationshipItem& AnimationRelationshipItem)
{
	return AnimationRelationshipItem.IsValid();
}

FAttributeRelationshipItem UCrysBlueprintFunctionLibrary::FindAttributeRelationshipItem(const FGameplayTag& AttributeTag, bool bLogNotFound)
{
	const UAttributeTagRelationship* AttributeTagRelationship = UCrysAssetManager::GetAsset(GetDefault<UPhantasyStarOnlineGameData>()->AttributeTagRelationship);
	if (!AttributeTagRelationship)
	{
		if (bLogNotFound)
		{
			UE_LOG(LogCrys, Error, TEXT("AttributeTagRelationship is invalid in [%s]"), *GetDefault<UPhantasyStarOnlineGameData>()->GetName());
		}
		return FAttributeRelationshipItem();
	}

	return AttributeTagRelationship->FindAttributeRelationshipItem(AttributeTag, bLogNotFound);
}

bool UCrysBlueprintFunctionLibrary::IsAttributeRelationshipItemValid(const FAttributeRelationshipItem& AttributeRelationshipItem)
{
	return AttributeRelationshipItem.IsValid();
}
