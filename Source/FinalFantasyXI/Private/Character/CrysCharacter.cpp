// Copyright Soccertitan 2025


#include "Character/CrysCharacter.h"

#include "CrysGameplayTags.h"
#include "AbilitySystem/Ability/Combat/CombatAnimationData.h"
#include "AI/CrysAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "Net/UnrealNetwork.h"
#include "System/CrysAssetManager.h"


ACrysCharacter::ACrysCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReplicates = true;
	bUseControllerRotationYaw = false;

	AIControllerClass = ACrysAIController::StaticClass();
}

void ACrysCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, CharacterName, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SkeletalMeshMergeParams, COND_None, REPNOTIFY_Always);
}

void ACrysCharacter::BeginPlay()
{
	Super::BeginPlay();

	MergeSkeletalMeshes();

	if (PrimaryCombatAnimationData)
	{
		LoadAnimationData(PrimaryCombatAnimationData, PrimaryAttacksStreamableHandle);
	}
	if (SecondaryCombatAnimationData)
	{
		LoadAnimationData(SecondaryCombatAnimationData, PrimaryAttacksStreamableHandle);
	}
}

void ACrysCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	MergeSkeletalMeshes();
	OnRep_SkeletalMeshMergeParams();
}

void ACrysCharacter::SetCharacterName(const FText Name)
{
	if (HasAuthority())
	{
		CharacterName = Name;
		OnRep_CharacterName();
	}
}

UCombatAnimationData* ACrysCharacter::GetPrimaryCombatAnimationData() const
{
	return PrimaryCombatAnimationData;
}

UCombatAnimationData* ACrysCharacter::GetSecondaryCombatAnimationData() const
{
	return SecondaryCombatAnimationData;
}

void ACrysCharacter::SetPrimaryCombatAnimationData(UCombatAnimationData* AnimationData)
{
	if (AnimationData != PrimaryCombatAnimationData)
	{
		PrimaryAttacksStreamableHandle.Reset();
		PrimaryCombatAnimationData = AnimationData;
		OnPrimaryCombatAnimationDataUpdatedDelegate.Broadcast(PrimaryCombatAnimationData);
		if (AnimationData)
		{
			LoadAnimationData(AnimationData, PrimaryAttacksStreamableHandle);
		}
	}
}

void ACrysCharacter::SetSecondaryCombatAnimationData(UCombatAnimationData* AnimationData)
{
	if (AnimationData != SecondaryCombatAnimationData)
	{
		SecondaryAttacksStreamableHandle.Reset();
		SecondaryCombatAnimationData = AnimationData;
		OnSecondaryCombatAnimationDataUpdatedDelegate.Broadcast(SecondaryCombatAnimationData);
		if (AnimationData)
		{
			LoadAnimationData(AnimationData, SecondaryAttacksStreamableHandle);
		}
	}
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

void ACrysCharacter::LoadAnimationData(const UCombatAnimationData* AnimationData, TSharedPtr<FStreamableHandle>& Handle)
{
	const TArray<FName>& LoadBundles = {"Animation"};
	Handle = UCrysAssetManager::Get().PreloadPrimaryAssets({AnimationData->GetPrimaryAssetId()}, LoadBundles, false);
}

void ACrysCharacter::BroadcastCharacterName()
{
	FMessageCharacterNameUpdated Message;
	Message.OwnerActor = this;
	Message.CharacterName = GetCharacterName();

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(this);
	MessageSystem.BroadcastMessage(FCrysGameplayTags::Get().Message_CharacterNameUpdated, Message);
}

