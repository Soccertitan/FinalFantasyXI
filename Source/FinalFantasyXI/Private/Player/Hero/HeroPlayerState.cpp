// Copyright Soccertitan 2025


#include "Player/Hero/HeroPlayerState.h"

#include "CrimAbilitySystemComponent.h"
#include "InventoryManagerComponent.h"
#include "AbilitySystem/AttributeSet/CrysAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "Attribute/TacticalPointsAttributeSet.h"
#include "EquipmentSystem/EquipmentManagerComponent.h"
#include "HeroSystem/HeroManagerComponent.h"

AHeroPlayerState::AHeroPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCrimAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);

	HitPointsAttributeSet = CreateDefaultSubobject<UCrysHitPointsAttributeSet>("HitPointsAttributeSet");
	TacticalPointsAttributeSet = CreateDefaultSubobject<UTacticalPointsAttributeSet>("TacticalPointsAttributeSet");
	ManaPointsAttributeSet = CreateDefaultSubobject<UManaPointsAttributeSet>("ManaPointsAttributeSet");
	CrysAttributeSet = CreateDefaultSubobject<UCrysAttributeSet>("CrysAttributeSet");

	SetNetUpdateFrequency(100.f);

	InventoryManagerComponent = CreateDefaultSubobject<UInventoryManagerComponent>("InventoryManagerComponent");
	InventoryManagerComponent->SetIsReplicated(true);
	bReplicateUsingRegisteredSubObjectList = true;

	HeroManagerComponent = CreateDefaultSubobject<UHeroManagerComponent>("HeroManagerComponent");
	HeroManagerComponent->SetIsReplicated(true);

	EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>("EquipmentManagerComponent");
	EquipmentManagerComponent->SetIsReplicated(true);
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

void AHeroPlayerState::BindToDelegates()
{
}
