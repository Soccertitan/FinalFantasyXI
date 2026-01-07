// Copyright Soccertitan 2025


#include "Character/CrysCharacter.h"

#include "CrysGameplayTags.h"
#include "AI/CrysAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"


ACrysCharacter::ACrysCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bUseControllerRotationYaw = false;

	AIControllerClass = ACrysAIController::StaticClass();
}

void ACrysCharacter::SetCharacterName(const FText Name)
{
	if (HasAuthority())
	{
		CharacterName = Name;
		OnRep_CharacterName();
	}
}

void ACrysCharacter::BeginPlay()
{
	Super::BeginPlay();

	MergeSkeletalMeshes();
}

void ACrysCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CharacterName, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SkeletalMeshMergeParams, COND_None, REPNOTIFY_Always);
}

void ACrysCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MergeSkeletalMeshes();
	OnRep_SkeletalMeshMergeParams();
}

UAnimationTagRelationship* ACrysCharacter::GetAnimationTagRelationship() const
{
	return AnimationTagRelationship;
}

void ACrysCharacter::DisableMovement()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
}

void ACrysCharacter::EnableMovement()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(false);
	}

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ACrysCharacter::OnRep_CharacterName()
{
	BroadcastCharacterName();
}

void ACrysCharacter::OnRep_SkeletalMeshMergeParams()
{
	MergeSkeletalMeshes();
}

void ACrysCharacter::MergeSkeletalMeshes()
{
	if (SkeletalMeshMergeParams.BaseMeshesToMerge.Num() > 0 ||
		SkeletalMeshMergeParams.DynamicMeshesToMerge.Num() > 0)
	{
		if (USkeletalMesh* MergedMesh = UMeshMergeFunctionLibrary::MergeMeshes(SkeletalMeshMergeParams))
		{
			GetMesh()->SetSkeletalMesh(MergedMesh, false);
		}
	}
}

void ACrysCharacter::BroadcastCharacterName()
{
	FMessageCharacterNameUpdated Message;
	Message.OwnerActor = this;
	Message.CharacterName = GetCharacterName();

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(FCrysGameplayTags::Get().Message_CharacterNameUpdated, Message);
}

