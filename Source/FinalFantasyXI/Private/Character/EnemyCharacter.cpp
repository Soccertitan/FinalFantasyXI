// Copyright Soccertitan 2025


#include "Character/EnemyCharacter.h"

#include "AbilitySet.h"
#include "CrimAbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/CrysAttributeSet.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "Attribute/HitPointsComponent.h"
#include "Attribute/TacticalPointsAttributeSet.h"
#include "Character/CrysCharacterMovementComponent.h"
#include "Character/CrysSkeletalMeshComponent.h"


AEnemyCharacter::AEnemyCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrysCharacterMovementComponent>(CharacterMovementComponentName).
		SetDefaultSubobjectClass<UCrysSkeletalMeshComponent>(MeshComponentName))
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	HitPointsAttributeSet = CreateDefaultSubobject<UHitPointsAttributeSet>("HitPointsAttributeSet");
	TacticalPointsAttributeSet = CreateDefaultSubobject<UTacticalPointsAttributeSet>("TacticalPointsAttributeSet");
	CrysAttributeSet = CreateDefaultSubobject<UCrysAttributeSet>("CrysAttributeSet");
	
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
	DisableMovementAndCollision();
}

void AEnemyCharacter::OnResurrectionFinished(AActor* OwningActor)
{
	EnableMovementAndCollision();
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
