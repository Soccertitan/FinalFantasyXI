// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "InventorySystemInterface.h"
#include "../CrysPlayerState.h"
#include "EquipmentSystem/EquipmentSystemInterface.h"
#include "HeroSystem/HeroSystemInterface.h"
#include "HeroPlayerState.generated.h"

class UManaPointsAttributeSet;
class UEquipmentManagerComponent;
class UHeroManagerComponent;
class UCrysManaPointsSet;
class UPrimaryAttributeSet;
class UResourcePointsAttributeSet;
class UCrimAbilitySystemComponent;
class UCrysHitPointsAttributeSet;

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API AHeroPlayerState : public ACrysPlayerState, public IAbilitySystemInterface,
	public IInventorySystemInterface, public IHeroSystemInterface, public IEquipmentSystemInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UCrysHitPointsAttributeSet> HitPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UResourcePointsAttributeSet> TacticalPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UManaPointsAttributeSet> ManaPointsAttributeSet;
	UPROPERTY()
	TObjectPtr<UPrimaryAttributeSet> CrysAttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InventoryManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInventoryManagerComponent> InventoryManagerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HeroManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHeroManagerComponent> HeroManagerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipmentManager", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipmentManagerComponent> EquipmentManagerComponent;

public:
	AHeroPlayerState();
	virtual void PostInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/** IInventorySystemInterface */
	virtual UInventoryManagerComponent* GetInventoryManagerComponent_Implementation() const override;
	/** IHeroSystemInterface */
	virtual UHeroManagerComponent* GetHeroManagerComponent_Implementation() const override;
	/** IEquipmentSystemInterface */
	virtual UEquipmentManagerComponent* GetEquipmentManagerComponent_Implementation() const override;

protected:
	/** A generic function to bind to delegates. Called in PostInitializeComponents */
	virtual void BindToDelegates();
};
