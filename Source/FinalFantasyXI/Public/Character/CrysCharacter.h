// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "MeshMergeFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "CrysCharacter.generated.h"

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

UCLASS()
class FINALFANTASYXI_API ACrysCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACrysCharacter(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category = "CrysCharacter")
	FText GetCharacterName() {return CharacterName;}

	UFUNCTION(BlueprintCallable, Category = "CrysCharacter")
	virtual void SetCharacterName(const FText Name);

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PossessedBy(AController* NewController) override;
	
	UAnimationTagRelationship* GetAnimationTagRelationship() const;
	
	void DisableMovement();
	void EnableMovement();

protected:
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
	
	UPROPERTY(EditAnywhere, Category = "Character|Animation")
	TObjectPtr<UAnimationTagRelationship> AnimationTagRelationship;

	void BroadcastCharacterName();
};
