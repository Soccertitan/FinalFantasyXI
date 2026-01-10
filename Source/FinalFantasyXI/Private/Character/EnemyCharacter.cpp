// Copyright Soccertitan 2025


#include "Character/EnemyCharacter.h"

#include "AbilitySet.h"
#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/CombatAttributeSet.h"
#include "AbilitySystem/AttributeSet/CombatSkillAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/MovementAttributeSet.h"
#include "Attribute/HitPointsComponent.h"
#include "Attribute/ResourcePointsAttributeSet.h"
#include "Character/CrysCharacterMovementComponent.h"
#include "Character/CrysSkeletalMeshComponent.h"


AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrysCharacterMovementComponent>(CharacterMovementComponentName).
		SetDefaultSubobjectClass<UCrysSkeletalMeshComponent>(MeshComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HitPointsAttributeSet = CreateDefaultSubobject<UCrysHitPointsAttributeSet>("HitPointsAttributeSet");
	TacticalPointsAttributeSet = CreateDefaultSubobject<UResourcePointsAttributeSet>("TacticalPointsAttributeSet");
	ManaPointsAttributeSet = CreateDefaultSubobject<UManaPointsAttributeSet>("ManaPointsAttributeSet");
	PrimaryAttributeSet = CreateDefaultSubobject<UPrimaryAttributeSet>("PrimaryAttributeSet");
	CombatAttributeSet = CreateDefaultSubobject<UCombatAttributeSet>("CombatAttributeSet");
	CombatSkillAttributeSet = CreateDefaultSubobject<UCombatSkillAttributeSet>("CombatSkillAttributeSet");
	MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>("MovementAttributeSet");
	AbilityAttributeSet = CreateDefaultSubobject<UAbilityAttributeSet>("AbilityAttributeSet");
	
	HitPointsComponent = CreateDefaultSubobject<UHitPointsComponent>(TEXT("HitPointsComponent"));

	SetNetUpdateFrequency(100.f);
}

void AEnemyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	HitPointsComponent->OnDeathStartedDelegate.AddDynamic(this, &ThisClass::OnDeathStarted);
	HitPointsComponent->OnDeathFinishedDelegate.AddDynamic(this, &ThisClass::OnDeathFinished);
	HitPointsComponent->OnReviveStartedDelegate.AddDynamic(this, &ThisClass::OnResurrectionStarted);
	HitPointsComponent->OnReviveFinishedDelegate.AddDynamic(this, &ThisClass::OnResurrectionFinished);
}

void AEnemyCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AEnemyCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AEnemyCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AEnemyCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

UAbilitySystemComponent* AEnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AEnemyCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovement();
}

void AEnemyCharacter::OnResurrectionFinished(AActor* OwningActor)
{
	EnableMovement();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitAbilitySystem();
}

void AEnemyCharacter::InitAbilitySystem()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
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
