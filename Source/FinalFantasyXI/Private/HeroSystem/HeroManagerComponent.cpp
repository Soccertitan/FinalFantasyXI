// Copyright Soccertitan 2025


#include "HeroSystem/HeroManagerComponent.h"

#include "AbilitySet.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysLogChannels.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "Attribute/ResourcePointsAttributeSet.h"
#include "HeroSystem/HeroRaceDefinition.h"
#include "Net/UnrealNetwork.h"
#include "System/CrysAssetManager.h"


UHeroManagerComponent::UHeroManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UHeroManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, HeroClassProgressContainer);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HeroClass, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HeroRace, COND_None, REPNOTIFY_Always);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MaxLevel, Params);
}

void UHeroManagerComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UPrimaryAttributeSet::GetLevelAttribute()).RemoveAll(this);
	}

	if (NewAbilitySystemComponent)
	{
		AbilitySystemComponent = NewAbilitySystemComponent;

		if (HasAuthority())
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
				UPrimaryAttributeSet::GetLevelAttribute()).AddUObject(this, &UHeroManagerComponent::OnLevelAttributeChanged);

			if (HeroRace)
			{
				ApplyHeroRaceTraits();
			}
			if (HeroClass)
			{
				ApplyHeroClassTraits();
				FHeroClassProgressItem HeroClassProgressItem = FindHeroClassProgressItem(HeroClass->HeroClassTag);
				if (!HeroClassProgressItem.IsValid())
				{
					// Don't have a valid HeroClassProgress item. Create a new one and initialize it at level 1.
					HeroClassProgressItem.Level = 1;
					HeroClassProgressItem.Experience = 0;
					HeroClassProgressItem.HeroClassTag = HeroClass->HeroClassTag;
					HeroClassProgressContainer.AddHeroClassProgressItem(HeroClassProgressItem);
				}
				SetHeroClassLevel(HeroClassProgressItem.Level);
			}
		}
	}
}

TArray<FHeroClassProgressItem> UHeroManagerComponent::GetHeroClassProgressItems() const
{
	return HeroClassProgressContainer.Items;
}

FHeroClassProgressItem UHeroManagerComponent::FindHeroClassProgressItem(const FGameplayTag HeroClassTag) const
{
	return HeroClassProgressContainer.GetHeroClassProgressItem(HeroClassTag);
}

void UHeroManagerComponent::AddOrSetHeroClassProgress(const FHeroClassProgressItem& InHeroClassProgressItem)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!InHeroClassProgressItem.IsValid())
	{
		return;
	}

	if (FHeroClassProgressItem* ProgressItem = HeroClassProgressContainer.Items.FindByKey(InHeroClassProgressItem.HeroClassTag))
	{
		ProgressItem->Experience = InHeroClassProgressItem.Experience;
		ProgressItem->Level = InHeroClassProgressItem.Level;
		HeroClassProgressContainer.MarkItemDirty(*ProgressItem);
		OnHeroClassProgressUpdatedDelegate.Broadcast(this, *ProgressItem);
	}
	else
	{
		HeroClassProgressContainer.AddHeroClassProgressItem(InHeroClassProgressItem);
	}

	if (HeroClass && HeroClass->HeroClassTag == InHeroClassProgressItem.HeroClassTag)
	{
		SetHeroClassLevel(InHeroClassProgressItem.Level);
	}
}

void UHeroManagerComponent::AddExperience(const int32 Experience)
{
	AddExperienceForHeroClass(HeroClass, Experience);
}

void UHeroManagerComponent::AddExperienceForHeroClass(const UHeroClassDefinition* InHeroClass, const int32 Experience)
{
	if (!HasAuthority() || Experience <= 0)
	{
		return;
	}

	if (!InHeroClass || !InHeroClass->HeroClassTag.IsValid())
	{
		UE_LOG(LogCrys, Error, TEXT("The InHeroClass does not have a valid HeroClassTag [%s]"), *GetNameSafe(InHeroClass));
		return;
	}

	HeroClassProgressContainer.AddExperience(InHeroClass->HeroClassTag, InHeroClass->ExperienceRequirement, Experience);
}

void UHeroManagerComponent::RestoreHeroClassProgressItems(const TArray<FHeroClassProgressItem>& InHeroClassProgressItems)
{
	if (!HasAuthority())
	{
		return;
	}

	HeroClassProgressContainer.Reset();
	
	for (const FHeroClassProgressItem& Item : InHeroClassProgressItems)
	{
		if (Item.IsValid())
		{
			HeroClassProgressContainer.AddHeroClassProgressItem(Item);
		}
	}

	SetHeroClass(GetHeroClass());
}

void UHeroManagerComponent::TrySetHeroClass(UHeroClassDefinition* InHeroClass)
{
	if (!HasAuthority())
	{
		Server_TrySetHeroClass(InHeroClass);
		return;
	}
	
	if (!InHeroClass)
	{
		Client_TrySetHeroClassFailed();
		return;
	}

	if (HeroClass == InHeroClass)
	{
		Client_TrySetHeroClassFailed();
		return;
	}

	FHeroClassProgressItem HeroProgress = HeroClassProgressContainer.GetHeroClassProgressItem(InHeroClass->HeroClassTag);
	if (!HeroProgress.IsValid())
	{
		Client_TrySetHeroClassFailed();
		return;
	}

	HeroClass = InHeroClass;
	if (AbilitySystemComponent)
	{
		ApplyHeroClassTraits();
		SetHeroClassLevel(HeroProgress.Level);
	}
	OnRep_HeroClass();
}

void UHeroManagerComponent::SetHeroClass(UHeroClassDefinition* InHeroClass)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!InHeroClass)
	{
		return;
	}

	HeroClass = InHeroClass;
	if (AbilitySystemComponent)
	{
		FHeroClassProgressItem Item = FindHeroClassProgressItem(HeroClass->HeroClassTag);
		if (!Item.IsValid())
		{
			// Don't have a valid HeroClassProgress item. Create a new one and initialize it at level 1.
			Item.Level = 1;
			Item.Experience = 0;
			Item.HeroClassTag = HeroClass->HeroClassTag;
			HeroClassProgressContainer.AddHeroClassProgressItem(Item);
		}
		ApplyHeroClassTraits();
		SetHeroClassLevel(Item.Level);
	}
	OnRep_HeroClass();
}

void UHeroManagerComponent::SetHeroClassLevel(const int32 Level)
{
	if (AbilitySystemComponent && HasAuthority())
	{
		UGameplayEffect* SetLevel = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("SetLevel")));
		SetLevel->DurationPolicy = EGameplayEffectDurationType::Instant;

		int32 Idx = SetLevel->Modifiers.Num();
		SetLevel->Modifiers.SetNum(Idx + 1);

		FGameplayModifierInfo& InfoLevel = SetLevel->Modifiers[Idx];
		InfoLevel.ModifierMagnitude = FScalableFloat(Level);
		InfoLevel.ModifierOp = EGameplayModOp::Override;
		InfoLevel.Attribute = UPrimaryAttributeSet::GetLevelAttribute();
		AbilitySystemComponent->ApplyGameplayEffectToSelf(SetLevel, 1.0f, AbilitySystemComponent->MakeEffectContext());
	}
}

void UHeroManagerComponent::SetHeroRace(UHeroRaceDefinition* InHeroRace)
{
	if (!InHeroRace || !HasAuthority())
	{
		return;
	}

	if (HeroRace == InHeroRace)
	{
		return;
	}

	HeroRace = InHeroRace;
	if (AbilitySystemComponent)
	{
		ApplyHeroRaceTraits();
		ApplyBaseAttributes(AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute()));
	}
	OnRep_HeroRace();
}

void UHeroManagerComponent::SetMaxLevel(const int32 InMaxLevel)
{
	if (!HasAuthority())
	{
		return;
	}

	MaxLevel = InMaxLevel;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MaxLevel, this);
}

bool UHeroManagerComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UHeroManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheIsNetSimulated();
}

void UHeroManagerComponent::PreNetReceive()
{
	Super::PreNetReceive();

	CacheIsNetSimulated();
}

void UHeroManagerComponent::OnRegister()
{
	Super::OnRegister();

	CacheIsNetSimulated();
	HeroClassProgressContainer.Owner = this;
}

void UHeroManagerComponent::OnLevelAttributeChanged(const FOnAttributeChangeData& Data)
{
	ApplyBaseAttributes(Data.NewValue);
}

void UHeroManagerComponent::OnRep_HeroClass()
{
	OnHeroClassChangedDelegate.Broadcast(this);
}

void UHeroManagerComponent::OnRep_HeroRace()
{
	OnHeroRaceChangedDelegate.Broadcast(this);
}

void UHeroManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UHeroManagerComponent::ApplyBaseAttributes(const float Level)
{
	UGameplayEffect* BaseStats = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseStats")));
	BaseStats->DurationPolicy = EGameplayEffectDurationType::Instant;
	FHeroBaseAttributesCalculated CalculatedAttributes(HeroClass, HeroRace, Level);

	int32 Idx = BaseStats->Modifiers.Num();
	BaseStats->Modifiers.SetNum(Idx + 9);

	FGameplayModifierInfo& InfoMaxHP = BaseStats->Modifiers[Idx];
	InfoMaxHP.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MaxHitPoints);
	InfoMaxHP.ModifierOp = EGameplayModOp::Override;
	InfoMaxHP.Attribute = UHitPointsAttributeSet::GetMaxPointsAttribute();

	FGameplayModifierInfo& InfoMaxTP = BaseStats->Modifiers[Idx + 1];
	InfoMaxTP.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MaxTacticalPoints);
	InfoMaxTP.ModifierOp = EGameplayModOp::Override;
	InfoMaxTP.Attribute = UResourcePointsAttributeSet::GetMaxPointsAttribute();

	FGameplayModifierInfo& InfoMeleeAttack = BaseStats->Modifiers[Idx + 2];
	InfoMeleeAttack.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MeleeAttack);
	InfoMeleeAttack.ModifierOp = EGameplayModOp::Override;
	// InfoMeleeAttack.Attribute = UCrysAttributeSet::GetMeleeAttackAttribute();

	FGameplayModifierInfo& InfoMeleeDefense = BaseStats->Modifiers[Idx + 3];
	InfoMeleeDefense.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MeleeDefense);
	InfoMeleeDefense.ModifierOp = EGameplayModOp::Override;
	// InfoMeleeDefense.Attribute = UCrysAttributeSet::GetMeleeDefenseAttribute();

	FGameplayModifierInfo& InfoRangeAttack = BaseStats->Modifiers[Idx +4 ];
	InfoRangeAttack.ModifierMagnitude = FScalableFloat(CalculatedAttributes.RangeAttack);
	InfoRangeAttack.ModifierOp = EGameplayModOp::Override;
	// InfoRangeAttack.Attribute = UCrysAttributeSet::GetRangeAttackAttribute();

	FGameplayModifierInfo& InfoRangeDefense = BaseStats->Modifiers[Idx + 5];
	InfoRangeDefense.ModifierMagnitude = FScalableFloat(CalculatedAttributes.RangeDefense);
	InfoRangeDefense.ModifierOp = EGameplayModOp::Override;
	// InfoRangeDefense.Attribute = UCrysAttributeSet::GetRangeDefenseAttribute();

	FGameplayModifierInfo& InfoTechniqueAttack = BaseStats->Modifiers[Idx + 6];
	InfoTechniqueAttack.ModifierMagnitude = FScalableFloat(CalculatedAttributes.TechniqueAttack);
	InfoTechniqueAttack.ModifierOp = EGameplayModOp::Override;
	// InfoTechniqueAttack.Attribute = UCrysAttributeSet::GetTechniqueAttackAttribute();

	FGameplayModifierInfo& InfoTechniqueDefense = BaseStats->Modifiers[Idx + 7];
	InfoTechniqueDefense.ModifierMagnitude = FScalableFloat(CalculatedAttributes.TechniqueDefense);
	InfoTechniqueDefense.ModifierOp = EGameplayModOp::Override;
	// InfoTechniqueDefense.Attribute = UCrysAttributeSet::GetTechniqueDefenseAttribute();

	FGameplayModifierInfo& InfoDexterity = BaseStats->Modifiers[Idx + 8];
	InfoDexterity.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Dexterity);
	InfoDexterity.ModifierOp = EGameplayModOp::Override;
	// InfoDexterity.Attribute = UCrysAttributeSet::GetDexterityAttribute();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(BaseStats, 1.0f, AbilitySystemComponent->MakeEffectContext());
	OnHeroBaseStatsChangedDelegate.Broadcast(this);
}

void UHeroManagerComponent::ApplyHeroRaceTraits()
{
	AbilitySet_GrantedHandles_RaceTraits.TakeFromAbilitySystem(AbilitySystemComponent);

	const UAbilitySet* AbilitySet = HeroRace->Traits.Get();
	if (!AbilitySet)
	{
		AbilitySet = UCrysAssetManager::Get().GetAsset(HeroRace->Traits, false);
	}

	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &AbilitySet_GrantedHandles_RaceTraits);
	}
}

void UHeroManagerComponent::ApplyHeroClassTraits()
{
	AbilitySet_GrantedHandles_ClassTraits.TakeFromAbilitySystem(AbilitySystemComponent);

	const UAbilitySet* AbilitySet = HeroClass->Traits.Get();
	if (!AbilitySet)
	{
		AbilitySet = UCrysAssetManager::Get().GetAsset(HeroClass->Traits, false);
	}

	if (AbilitySet)
	{
		AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &AbilitySet_GrantedHandles_ClassTraits);
	}
}

void UHeroManagerComponent::Client_TrySetHeroClassFailed_Implementation()
{
	OnTrySetHeroClassFailedDelegate.Broadcast(this);
}

void UHeroManagerComponent::Internal_OnLevelUp(const FHeroClassProgressItem& HeroClassProgressItem, const int32 OldLevel)
{
	if (HeroClass && HeroClass->HeroClassTag.MatchesTagExact(HeroClassProgressItem.HeroClassTag))
	{
		SetHeroClassLevel(HeroClassProgressItem.Level);
	}
	
	Multi_OnLevelUp(HeroClassProgressItem, OldLevel);
}

void UHeroManagerComponent::Multi_OnLevelUp_Implementation(const FHeroClassProgressItem& HeroClassProgressItem, const int32 OldLevel)
{
	OnHeroClassLevelUpDelegate.Broadcast(this, HeroClassProgressItem, OldLevel);
}

void UHeroManagerComponent::Server_TrySetHeroClass_Implementation(const UHeroClassDefinition* InHeroClass)
{
	TrySetHeroClass(const_cast<UHeroClassDefinition*>(InHeroClass));
}
