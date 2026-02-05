// Copyright Soccertitan 2025


#include "Character/HeroCharacter.h"

#include "AbilitySet.h"
#include "CrimAbilitySystemComponent.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "InteractorComponent.h"
#include "InventoryBlueprintFunctionLibrary.h"
#include "TargetingSystemComponent.h"
#include "TargetPointComponent.h"
#include "AI/HeroAIController.h"
#include "Attribute/HitPointsComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/CrysCharacterMovementComponent.h"
#include "Character/CrysSkeletalMeshComponent.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
#include "InteractionType/InteractorBoxComponent.h"


AHeroCharacter::AHeroCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrysCharacterMovementComponent>(CharacterMovementComponentName).
	SetDefaultSubobjectClass<UCrysSkeletalMeshComponent>(MeshComponentName))
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->ProbeChannel = ECC_Visibility;
	CameraBoom->bUsePawnControlRotation = true; // Rotates the arm based on the controller
	CameraBoom->TargetArmLength = 500.0f;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Camera does not rotate relative to arm
	FollowCamera->bUsePawnControlRotation = false;

	TargetingSystemComponent = CreateDefaultSubobject<UTargetingSystemComponent>(TEXT("TargetingSystemComponent"));
	
	InteractorComponent = CreateDefaultSubobject<UInteractorComponent>(TEXT("InteractorComponent"));
	
	InteractorBoxComponent = CreateDefaultSubobject<UInteractorBoxComponent>(TEXT("InteractorBoxComponent"));
	InteractorBoxComponent->SetupAttachment(RootComponent);
	
	HitPointsComponent = CreateDefaultSubobject<UHitPointsComponent>(TEXT("HitPointsComponent"));

	AIControllerClass = AHeroAIController::StaticClass();
}

void AHeroCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	HitPointsComponent->OnDeathStartedDelegate.AddDynamic(this, &ThisClass::OnDeathStarted);
	HitPointsComponent->OnDeathFinishedDelegate.AddDynamic(this, &ThisClass::OnDeathFinished);
	HitPointsComponent->OnReviveStartedDelegate.AddDynamic(this, &ThisClass::OnResurrectionStarted);
	HitPointsComponent->OnReviveFinishedDelegate.AddDynamic(this, &ThisClass::OnResurrectionFinished);
}

void AHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	InitAbilitySystem();
}

void AHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	InitAbilitySystem();
}

UAbilitySystemComponent* AHeroCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

AActor* AHeroCharacter::GetAbilityTarget_Implementation(const FGameplayTagContainer& AbilityTags) const
{
	AActor* Result = IAbilityTargetInterface::GetAbilityTarget_Implementation(AbilityTags);
	if (Result == nullptr)
	{
		if (const UTargetPointComponent* TargetPoint = TargetingSystemComponent->GetTarget())
		{
			return TargetPoint->GetAttachmentRootActor();
		}
	}
	return Result;
}

void AHeroCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AHeroCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AHeroCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AHeroCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

UInventoryManagerComponent* AHeroCharacter::GetInventoryManagerComponent_Implementation() const
{
	return UInventoryBlueprintFunctionLibrary::GetInventoryManagerComponent(GetPlayerState());
}

UTargetingSystemComponent* AHeroCharacter::GetTargetingSystemComponent_Implementation() const
{
	return TargetingSystemComponent;
}

UInteractorComponent* AHeroCharacter::GetInteractorComponent_Implementation() const
{
	return InteractorComponent;
}

UHeroManagerComponent* AHeroCharacter::GetHeroManagerComponent_Implementation() const
{
	return UHeroSystemBlueprintFunctionLibrary::GetHeroManagerComponent(GetPlayerState());
}

UEquipmentManagerComponent* AHeroCharacter::GetEquipmentManagerComponent_Implementation() const
{
	return UEquipmentSystemBlueprintFunctionLibrary::GetEquipmentManagerComponent(GetPlayerState());
}

void AHeroCharacter::InitAbilitySystem()
{
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(GetPlayerState());
	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(GetPlayerState(), this);
	
	if (HasAuthority())
	{
		for (const TObjectPtr<UAbilitySet>& AbilitySet : AbilitySets)
		{
			if (AbilitySet)
			{
				AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
			}
		}
	}
}

void AHeroCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovement();
}

void AHeroCharacter::OnResurrectionFinished(AActor* OwningActor)
{
	EnableMovement();
}
