// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MeshMergeFunctionLibrary.h"
#include "Engine/StreamableManager.h"
#include "GameFramework/Character.h"
#include "CrysCharacter.generated.h"

class UAbilityAnimationData;
class UCombatAnimationData;
class UAnimationTagRelationship;

USTRUCT(BlueprintType)
struct FMessageCharacterNameUpdated
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY(BlueprintReadOnly)
	FText CharacterName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCrysCharacterCombatAnimationDataSignature, UCombatAnimationData*, AnimationData);

UCLASS()
class FINALFANTASYXI_API ACrysCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACrysCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	
	UFUNCTION(BlueprintPure, Category = "CrysCharacter")
	FText GetCharacterName() {return CharacterName;}

	UFUNCTION(BlueprintCallable, Category = "CrysCharacter")
	virtual void SetCharacterName(const FText Name);
	
	UFUNCTION(BlueprintPure, Category = "CrysCharacter|Animation")
	UAbilityAnimationData* GetAbilityAnimationData() const {return AbilityAnimationData;}

	UPROPERTY(BlueprintAssignable, DisplayName = "OnPrimaryCombatAnimationDataUpdated")
	FCrysCharacterCombatAnimationDataSignature OnPrimaryCombatAnimationDataUpdatedDelegate;
	UPROPERTY(BlueprintAssignable, DisplayName = "OnSecondaryCombatAnimationDataUpdated")
	FCrysCharacterCombatAnimationDataSignature OnSecondaryCombatAnimationDataUpdatedDelegate;
	
	/** The animation data will be loading/loaded. */
	UFUNCTION(BlueprintPure, Category = "CrysCharacter|Animation")
	UCombatAnimationData* GetPrimaryCombatAnimationData() const;
	/** The animation data will be loading/loaded. */
	UFUNCTION(BlueprintPure, Category = "CrysCharacter|Animation")
	UCombatAnimationData* GetSecondaryCombatAnimationData() const;

	UFUNCTION(BlueprintCallable, Category = "CrysCharacter|Animation")
	void SetPrimaryCombatAnimationData(UCombatAnimationData* AnimationData);
	UFUNCTION(BlueprintCallable, Category = "CrysCharacter|Animation")
	void SetSecondaryCombatAnimationData(UCombatAnimationData* AnimationData);
	
	void DisableMovement();
	void EnableMovement();

protected:
	/** Animations that are used for non weapon related abilities. */
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TObjectPtr<UAbilityAnimationData> AbilityAnimationData;
	
	UFUNCTION()
	virtual void OnRep_CharacterName();

	UFUNCTION()
	virtual void OnRep_SkeletalMeshMergeParams();

	void MergeSkeletalMeshes();
	
private:
	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_CharacterName, Category = "Character|Name")
	FText CharacterName;

	UPROPERTY(EditAnywhere, ReplicatedUsing=OnRep_SkeletalMeshMergeParams, Category = "Character|MeshMerge")
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;
	
	/** Animation data for MainHand combat animations. */
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TObjectPtr<UCombatAnimationData> PrimaryCombatAnimationData;
	/** Animation data for OffHand combat animations. */
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TObjectPtr<UCombatAnimationData> SecondaryCombatAnimationData;
	
	TSharedPtr<FStreamableHandle> PrimaryAttacksStreamableHandle;
	TSharedPtr<FStreamableHandle> SecondaryAttacksStreamableHandle;
	
	static void LoadAnimationData(const UCombatAnimationData* AnimationData, TSharedPtr<FStreamableHandle>& Handle);

	void BroadcastCharacterName();
};
