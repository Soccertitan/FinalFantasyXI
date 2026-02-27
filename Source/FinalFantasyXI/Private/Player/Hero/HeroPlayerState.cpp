// Copyright Soccertitan 2025


#include "Player/Hero/HeroPlayerState.h"

#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysGameplayTags.h"
#include "InventoryManagerComponent.h"
#include "AbilitySystem/Ability/Combat/AutoAttackManagerComponent.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/CombatSkillAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"
#include "AbilitySystem/AttributeSet/HeroJobAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/MovementAttributeSet.h"
#include "Attribute/ResourcePointsAttributeSet.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "HeroSystem/HeroManagerComponent.h"

AHeroPlayerState::AHeroPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HitPointsAttributeSet = CreateDefaultSubobject<UCrysHitPointsAttributeSet>("HitPointsAttributeSet");
	TacticalPointsAttributeSet = CreateDefaultSubobject<UResourcePointsAttributeSet>("TacticalPointsAttributeSet");
	ManaPointsAttributeSet = CreateDefaultSubobject<UManaPointsAttributeSet>("ManaPointsAttributeSet");
	PrimaryAttributeSet = CreateDefaultSubobject<UPrimaryAttributeSet>("PrimaryAttributeSet");
	AttackerAttributeSet = CreateDefaultSubobject<UAttackerAttributeSet>("AttackerAttributeSet");
	DefenderAttributeSet = CreateDefaultSubobject<UDefenderAttributeSet>("DefenderAttributeSet");
	CombatSkillAttributeSet = CreateDefaultSubobject<UCombatSkillAttributeSet>("CombatSkillAttributeSet");
	MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>("MovementAttributeSet");
	AbilityAttributeSet = CreateDefaultSubobject<UAbilityAttributeSet>("AbilityAttributeSet");
	JobAttributeSet = CreateDefaultSubobject<UHeroJobAttributeSet>("JobAttributeSet");

	SetNetUpdateFrequency(100.f);

	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManagerComponent");
	InventoryManagerComponent->SetIsReplicated(true);
	bReplicateUsingRegisteredSubObjectList = true;

	HeroManagerComponent = CreateDefaultSubobject<UHeroManagerComponent>("HeroManagerComponent");
	HeroManagerComponent->SetIsReplicated(true);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>("EquipmentManagerComponent");
	EquipmentManagerComponent->SetIsReplicated(true);
	
	AutoAttackManagerComponent = CreateDefaultSubobject<UAutoAttackManagerComponent>(TEXT("AutoAttackManagerComponent"));
	AutoAttackManagerComponent->SetIsReplicated(true);
}

void AHeroPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BindToDelegates();
}

void AHeroPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

UAbilitySystemComponent* AHeroPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UInventoryManagerComponent* AHeroPlayerState::GetInventoryManagerComponent_Implementation() const
{
	return InventoryManagerComponent;
}

UHeroManagerComponent* AHeroPlayerState::GetHeroManagerComponent_Implementation() const
{
	return HeroManagerComponent;
}

UEquipmentManagerComponent* AHeroPlayerState::GetEquipmentManagerComponent_Implementation() const
{
	return EquipmentManagerComponent;
}

AActor* AHeroPlayerState::GetAbilityTarget_Implementation(const FGameplayTagContainer& AbilityTags) const
{
	AActor* Result = IAbilityTargetInterface::GetAbilityTarget_Implementation(AbilityTags);
	if (!Result)
	{
		return UCrysBlueprintFunctionLibrary::GetAbilityTarget(GetPawn(), AbilityTags);
	}
	return Result;
}

FWeaponData AHeroPlayerState::GetPrimaryWeaponData_Implementation() const
{
	FWeaponData Result = EquipmentManagerComponent->GetEquippedItem(FCrysGameplayTags::Get().EquipSlot_MainHand).WeaponData;
	if (!Result.IsValid())
	{
		Result = EquipmentManagerComponent->GetBareHandedWeaponData();
	}
	return Result;
}

FWeaponData AHeroPlayerState::GetSecondaryWeaponData_Implementation() const
{
	return EquipmentManagerComponent->GetEquippedItem(FCrysGameplayTags::Get().EquipSlot_SubHand).WeaponData;
}

AActor* AHeroPlayerState::GetTargetActor_Implementation()
{
	//TODO Get actor from target system component.
	return nullptr;
}

void AHeroPlayerState::BindToDelegates()
{
}
