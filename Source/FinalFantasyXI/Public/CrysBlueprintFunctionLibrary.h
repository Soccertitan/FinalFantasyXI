// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AttributeTagRelationship.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CrysBlueprintFunctionLibrary.generated.h"

class ACrysCharacter;
class UCrimGameplayAbility;
class ACrysPlayerState;
class ACrysPlayerController;
struct FAnimationRelationshipItem;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/** Returns the PlayerController, will check if it's a PlayerController or try and retrieve from the PlayerState or Pawn. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static ACrysPlayerController* GetCrysPlayerController(AActor* Actor);

	/** Returns the PlayerState, will check if it's a PlayerState or try and retrieve from the PlayerController or Pawn. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static ACrysPlayerState* GetCrysPlayerState(AActor* Actor);
	
	/** Returns the Character, will check if it's a Character or try and retrieve from the PlayerState or PlayerController. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static ACrysCharacter* GetCrysCharacter(AActor* Actor);
	
	/** Gets the CrysCharacter and returns the AnimationRelationshipItem. */
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (DefaultToSelf = Actor))
	static FAnimationRelationshipItem FindAnimationRelationshipItem(AActor* Actor, const FGameplayTag& AnimationTag, bool bLogNotFound = false);
	
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (BlueprintThreadSafe, CompactNodeTitle = "IsValid"))
	static bool IsAnimationRelationshipItemValid(const FAnimationRelationshipItem& AnimationRelationshipItem);

	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary")
	static FAttributeRelationshipItem FindAttributeRelationshipItem(const FGameplayTag& AttributeTag, bool bLogNotFound = false);
	
	UFUNCTION(BlueprintPure, Category = "CrysFunctionLibrary", meta = (BlueprintThreadSafe, CompactNodeTitle = "IsValid"))
	static bool IsAttributeRelationshipItemValid(const FAttributeRelationshipItem& AttributeRelationshipItem);
};
