// Copyright Soccertitan 2025


#include "HeroSystem/HeroManagerComponent.h"

#include "AbilitySet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysLogChannels.h"
#include "AbilitySystem/AttributeSet/HeroJobAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "HeroSystem/HeroRaceDefinition.h"
#include "HeroSystem/HeroSystemBlueprintFunctionLibrary.h"
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

	DOREPLIFETIME(ThisClass, HeroJobProgressContainer);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, HeroRace, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MainJob, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SubJob, COND_None, REPNOTIFY_Always);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, HeroProgress, Params);
}

void UHeroManagerComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (IsValid(AbilitySystemComponent))
	{
		RemoveBindToAttributeDelegates();
	}

	if (NewAbilitySystemComponent)
	{
		AbilitySystemComponent = NewAbilitySystemComponent;

		if (HasAuthority())
		{
			OverrideAttribute(HeroProgress.Level, UPrimaryAttributeSet::GetLevelAttribute());
			OverrideAttribute(HeroProgress.SubJobEfficiency, UHeroJobAttributeSet::GetSubJobEffectivenessAttribute());
			ApplyHeroRaceTraits();
			if (MainJob)
			{
				OverrideAttribute(FindHeroJobProgressItem(MainJob->JobTag).Level, UHeroJobAttributeSet::GetMainJobLevelAttribute());
				ApplyHeroMainJobTraits();
			}
			if (SubJob)
			{
				OverrideAttribute(FindHeroJobProgressItem(SubJob->JobTag).Level, UHeroJobAttributeSet::GetSubJobLevelAttribute());
				ApplyHeroSubJobTraits();
			}
			
			BindToAttributeDelegates();
			ApplyBaseAttributes();
		}
	}
}

TArray<FHeroJobProgressItem> UHeroManagerComponent::GetHeroJobProgressItems() const
{
	return HeroJobProgressContainer.Items;
}

FHeroJobProgressItem UHeroManagerComponent::FindHeroJobProgressItem(const FGameplayTag HeroJobTag) const
{
	return HeroJobProgressContainer.GetHeroJobProgressItem(HeroJobTag);
}

void UHeroManagerComponent::AddOrSetHeroJobProgressItem(const FHeroJobProgressItem& InHeroJobProgressItem)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!InHeroJobProgressItem.IsValid())
	{
		return;
	}

	if (FHeroJobProgressItem* ProgressItem = HeroJobProgressContainer.Items.FindByKey(InHeroJobProgressItem.JobTag))
	{
		ProgressItem->Level = InHeroJobProgressItem.Level;
		ProgressItem->Experience = InHeroJobProgressItem.Experience;
		HeroJobProgressContainer.MarkItemDirty(*ProgressItem);
		OnHeroJobProgressUpdatedDelegate.Broadcast(*ProgressItem);
	}
	else
	{
		HeroJobProgressContainer.AddHeroJobProgressItem(InHeroJobProgressItem);
	}

	if (MainJob && MainJob->JobTag == InHeroJobProgressItem.JobTag)
	{
		OverrideAttribute(InHeroJobProgressItem.Level, UHeroJobAttributeSet::GetMainJobLevelAttribute());
	}
	else if (SubJob && SubJob->JobTag == InHeroJobProgressItem.JobTag)
	{
		OverrideAttribute(InHeroJobProgressItem.Level, UHeroJobAttributeSet::GetMainJobLevelAttribute());
	}
}

void UHeroManagerComponent::AddExperienceForHeroJob(const UHeroJobDefinition* HeroJob, const int32 Experience)
{
	if (!HasAuthority() || Experience <= 0)
	{
		return;
	}

	if (!HeroJob || !HeroJob->JobTag.IsValid())
	{
		UE_LOG(LogCrys, Error, TEXT("The HeroClass does not have a valid JobTag [%s]"), *GetNameSafe(HeroJob));
		return;
	}

	HeroJobProgressContainer.AddExperience(HeroJob->JobTag, HeroJob->ExperienceRequirement, Experience);
}

void UHeroManagerComponent::RestoreHeroJobProgressItems(const TArray<FHeroJobProgressItem>& InHeroJobProgressItems)
{
	if (!HasAuthority())
	{
		return;
	}

	HeroJobProgressContainer.Reset();
	
	for (const FHeroJobProgressItem& Item : InHeroJobProgressItems)
	{
		if (Item.IsValid())
		{
			HeroJobProgressContainer.AddHeroJobProgressItem(Item);
		}
	}
	
	if (AbilitySystemComponent)
	{
		bSkipApplyingBaseStats = true;
		if (MainJob)
		{
			OverrideAttribute(FindHeroJobProgressItem(MainJob->JobTag).Level, UHeroJobAttributeSet::GetMainJobLevelAttribute());
		}
		if (SubJob)
		{
			OverrideAttribute(FindHeroJobProgressItem(SubJob->JobTag).Level, UHeroJobAttributeSet::GetSubJobLevelAttribute());
		}
		bSkipApplyingBaseStats = false;
		ApplyBaseAttributes();
	}
}

void UHeroManagerComponent::TrySetHeroJobs(UHeroJobDefinition* InMainJob, UHeroJobDefinition* InSubJob)
{
	if (!HasAuthority())
	{
		Server_TrySetHeroJobs(InMainJob, InSubJob);
		return;
	}

	if (!InMainJob || InMainJob == InSubJob)
	{
		Client_OnTrySetHeroJobs(false);
		return;
	}
	
	bSkipApplyingBaseStats = true;
	
	// Don't allow changing to a MainJob that doesn't have existing progress.
	const FHeroJobProgressItem MainJobProgress = FindHeroJobProgressItem(InMainJob->JobTag);
	if (!MainJobProgress.IsValid())
	{
		bSkipApplyingBaseStats = false;
		Client_OnTrySetHeroJobs(false);
		return;
	}
	
	bool bMainJobChanged = false;
	if (MainJob != InMainJob)
	{
		MainJob = InMainJob;
		bMainJobChanged = true;
	}
	
	bool bSubJobChanged = false;
	FHeroJobProgressItem SubJobProgress = InSubJob ? FindHeroJobProgressItem(InSubJob->JobTag) : FHeroJobProgressItem();
	if (HeroProgress.bSubJobUnlocked && SubJobProgress.IsValid())
	{
		if (SubJob != InSubJob)
		{
			SubJob = InSubJob;
			bSubJobChanged = true;
		}
	}
	else
	{
		if (SubJob)
		{
			SubJob = nullptr;
			bSubJobChanged = true;
		}
	}
	
	if (AbilitySystemComponent)
	{
		OverrideAttribute(MainJobProgress.Level, UHeroJobAttributeSet::GetMainJobLevelAttribute());
		ApplyHeroMainJobTraits();
		OverrideAttribute(SubJobProgress.Level, UHeroJobAttributeSet::GetSubJobLevelAttribute());
		ApplyHeroSubJobTraits();
	}
	
	bSkipApplyingBaseStats = false;
	if (AbilitySystemComponent)
	{
		ApplyBaseAttributes();
	}

	if (bMainJobChanged)
	{
		OnHeroMainJobChangedDelegate.Broadcast();
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MainJob, this);
	}
	if (bSubJobChanged)
	{
		OnHeroSubJobChangedDelegate.Broadcast();
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, SubJob, this);
	}

	Client_OnTrySetHeroJobs(true);
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
		ApplyBaseAttributes();
	}
	OnRep_HeroRace();
}

const FHeroProgress& UHeroManagerComponent::GetHeroProgress() const
{
	return HeroProgress;
}

void UHeroManagerComponent::SetHeroProgress(const FHeroProgress& InHeroProgress)
{
	if (!HasAuthority())
	{
		return;
	}
	
	int32 OldLevel = HeroProgress.Level;
	HeroProgress = InHeroProgress;
	if (OldLevel != HeroProgress.Level)
	{
		OverrideAttribute(HeroProgress.Level, UPrimaryAttributeSet::GetLevelAttribute());
	}

	OnHeroProgressUpdatedDelegate.Broadcast();
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, HeroProgress, this);
}

bool UHeroManagerComponent::AddExperience(const int32 Experience)
{
	if (!HasAuthority() || Experience == 0 || !HeroRace)
	{
		return false;
	}

	bool bLevelUpdated = false;
	int32 OldLevel = HeroProgress.Level;

	if (Experience > 0)
	{
		bLevelUpdated = UHeroSystemBlueprintFunctionLibrary::AddExperience(HeroProgress.Level, HeroProgress.Experience, 
			Experience, HeroRace->ExperienceRequirement, HeroProgress.MaxLevel);
	}
	else
	{
		bLevelUpdated = UHeroSystemBlueprintFunctionLibrary::SubtractExperience(HeroProgress.Level, HeroProgress.Experience, 
			FMath::Abs(Experience), HeroRace->ExperienceRequirement);
	}
	
	if (bLevelUpdated)
	{
		if (HeroProgress.Level > OldLevel)
		{
			Multi_OnHeroLevelUp(OldLevel);
		}
		else
		{
			Multi_OnHeroLevelDown(OldLevel);
		}
		OverrideAttribute(HeroProgress.Level, UPrimaryAttributeSet::GetLevelAttribute());
	}

	OnHeroProgressUpdatedDelegate.Broadcast();
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, HeroProgress, this);
	return bLevelUpdated;
}

bool UHeroManagerComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UHeroManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheIsNetSimulated();
	
	if (HeroJobProgressContainer.Items.IsEmpty())
	{
		RestoreHeroJobProgressItems(StartingJobProgress);
	}
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
	HeroJobProgressContainer.Owner = this;
}

void UHeroManagerComponent::OnRep_HeroRace()
{
	OnHeroRaceChangedDelegate.Broadcast();
}

void UHeroManagerComponent::OnRep_HeroProgress()
{
	OnHeroProgressUpdatedDelegate.Broadcast();
}

void UHeroManagerComponent::OnRep_MainJob()
{
	OnHeroMainJobChangedDelegate.Broadcast();
}

void UHeroManagerComponent::OnRep_SubJob()
{
	OnHeroSubJobChangedDelegate.Broadcast();
}

void UHeroManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UHeroManagerComponent::OverrideAttribute(const float Value, const FGameplayAttribute& Attribute)
{
	if (AbilitySystemComponent && HasAuthority())
	{
		UGameplayEffect* InstantGE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("InstantGE")));
		InstantGE->DurationPolicy = EGameplayEffectDurationType::Instant;
	
		int32 Idx = InstantGE->Modifiers.Num();
		InstantGE->Modifiers.SetNum(Idx + 1);
	
		FGameplayModifierInfo& ModifierInfo = InstantGE->Modifiers[Idx];
		ModifierInfo.ModifierMagnitude = FScalableFloat(Value);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;
		ModifierInfo.Attribute = Attribute;
		AbilitySystemComponent->ApplyGameplayEffectToSelf(InstantGE, 1.0f, AbilitySystemComponent->MakeEffectContext());
	}
}

void UHeroManagerComponent::ApplyBaseAttributes()
{
	UGameplayEffect* BaseStats = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseStats")));
	BaseStats->DurationPolicy = EGameplayEffectDurationType::Instant;
	FHeroPrimaryAttributesCalculated CalculatedAttributes(
		HeroRace, AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute()), 
		MainJob, AbilitySystemComponent->GetNumericAttribute(UHeroJobAttributeSet::GetMainJobLevelAttribute()),
		SubJob, AbilitySystemComponent->GetNumericAttribute(UHeroJobAttributeSet::GetSubJobLevelAttribute()),
		AbilitySystemComponent->GetNumericAttribute(UHeroJobAttributeSet::GetSubJobEffectivenessAttribute()));
	
	int32 Idx = BaseStats->Modifiers.Num();
	BaseStats->Modifiers.SetNum(Idx + 9);
	
	FGameplayModifierInfo& InfoMaxHP = BaseStats->Modifiers[Idx];
	InfoMaxHP.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MaxHitPoints);
	InfoMaxHP.ModifierOp = EGameplayModOp::Override;
	InfoMaxHP.Attribute = UHitPointsAttributeSet::GetMaxPointsAttribute();
	
	FGameplayModifierInfo& InfoMaxMP = BaseStats->Modifiers[Idx + 1];
	InfoMaxMP.ModifierMagnitude = FScalableFloat(CalculatedAttributes.MaxMagicPoints);
	InfoMaxMP.ModifierOp = EGameplayModOp::Override;
	InfoMaxMP.Attribute = UManaPointsAttributeSet::GetMaxPointsAttribute();
	
	FGameplayModifierInfo& InfoStrength = BaseStats->Modifiers[Idx + 2];
	InfoStrength.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Strength);
	InfoStrength.ModifierOp = EGameplayModOp::Override;
	InfoStrength.Attribute = UPrimaryAttributeSet::GetStrengthAttribute();
	
	FGameplayModifierInfo& InfoVitality = BaseStats->Modifiers[Idx + 3];
	InfoVitality.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Vitality);
	InfoVitality.ModifierOp = EGameplayModOp::Override;
	InfoVitality.Attribute = UPrimaryAttributeSet::GetVitalityAttribute();
	
	FGameplayModifierInfo& InfoDexterity = BaseStats->Modifiers[Idx +4 ];
	InfoDexterity.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Dexterity);
	InfoDexterity.ModifierOp = EGameplayModOp::Override;
	InfoDexterity.Attribute = UPrimaryAttributeSet::GetDexterityAttribute();
	
	FGameplayModifierInfo& InfoAgility = BaseStats->Modifiers[Idx + 5];
	InfoAgility.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Agility);
	InfoAgility.ModifierOp = EGameplayModOp::Override;
	InfoAgility.Attribute = UPrimaryAttributeSet::GetAgilityAttribute();
	
	FGameplayModifierInfo& InfoIntelligence = BaseStats->Modifiers[Idx + 6];
	InfoIntelligence.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Intelligence);
	InfoIntelligence.ModifierOp = EGameplayModOp::Override;
	InfoIntelligence.Attribute = UPrimaryAttributeSet::GetIntelligenceAttribute();
	
	FGameplayModifierInfo& InfoMind = BaseStats->Modifiers[Idx + 7];
	InfoMind.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Mind);
	InfoMind.ModifierOp = EGameplayModOp::Override;
	InfoMind.Attribute = UPrimaryAttributeSet::GetMindAttribute();
	
	FGameplayModifierInfo& InfoCharisma = BaseStats->Modifiers[Idx + 8];
	InfoCharisma.ModifierMagnitude = FScalableFloat(CalculatedAttributes.Charisma);
	InfoCharisma.ModifierOp = EGameplayModOp::Override;
	InfoCharisma.Attribute = UPrimaryAttributeSet::GetCharismaAttribute();

	AbilitySystemComponent->ApplyGameplayEffectToSelf(BaseStats, 1.0f, AbilitySystemComponent->MakeEffectContext());
}

void UHeroManagerComponent::ApplyHeroRaceTraits()
{
	AbilitySet_GrantedHandles_RaceTraits.TakeFromAbilitySystem(AbilitySystemComponent);

	if (HeroRace)
	{
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
}

void UHeroManagerComponent::ApplyHeroMainJobTraits()
{
	AbilitySet_GrantedHandles_MainJobTraits.TakeFromAbilitySystem(AbilitySystemComponent);

	if (MainJob)
	{
		const UAbilitySet* AbilitySet = MainJob->MainJobAbilitySet.Get();
		if (!AbilitySet)
		{
			AbilitySet = UCrysAssetManager::Get().GetAsset(MainJob->MainJobAbilitySet, false);
		}

		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &AbilitySet_GrantedHandles_MainJobTraits);
		}
	}
}

void UHeroManagerComponent::ApplyHeroSubJobTraits()
{
	AbilitySet_GrantedHandles_SubJobTraits.TakeFromAbilitySystem(AbilitySystemComponent);

	if (SubJob)
	{
		const UAbilitySet* AbilitySet = SubJob->SubJobAbilitySet.Get();
		if (!AbilitySet)
		{
			AbilitySet = UCrysAssetManager::Get().GetAsset(SubJob->SubJobAbilitySet, false);
		}

		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &AbilitySet_GrantedHandles_SubJobTraits);
		}
	}
}

void UHeroManagerComponent::BindToAttributeDelegates()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UPrimaryAttributeSet::GetLevelAttribute()).AddUObject(this, &UHeroManagerComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UHeroJobAttributeSet::GetMainJobLevelAttribute()).AddUObject(this, &UHeroManagerComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UHeroJobAttributeSet::GetSubJobLevelAttribute()).AddUObject(this, &UHeroManagerComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UHeroJobAttributeSet::GetSubJobEffectivenessAttribute()).AddUObject(this, &UHeroManagerComponent::OnAttributeChanged);
}

void UHeroManagerComponent::RemoveBindToAttributeDelegates()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UPrimaryAttributeSet::GetLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UHeroJobAttributeSet::GetMainJobLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UHeroJobAttributeSet::GetSubJobLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UHeroJobAttributeSet::GetSubJobEffectivenessAttribute()).RemoveAll(this);
}

void UHeroManagerComponent::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!bSkipApplyingBaseStats)
	{
		ApplyBaseAttributes();
	}
}

void UHeroManagerComponent::Multi_OnHeroLevelUp_Implementation(const int32 OldLevel)
{
	OnHeroLevelUpDelegate.Broadcast(OldLevel);
}

void UHeroManagerComponent::Multi_OnHeroLevelDown_Implementation(const int32 OldLevel)
{
	OnHeroLevelDownDelegate.Broadcast(OldLevel);
}

void UHeroManagerComponent::Internal_OnJobLevelUp(const FHeroJobProgressItem& HeroJobProgressItem, const int32 OldLevel)
{
	if (MainJob && MainJob->JobTag.MatchesTagExact(HeroJobProgressItem.JobTag))
	{
		OverrideAttribute(HeroJobProgressItem.Level, UHeroJobAttributeSet::GetMainJobLevelAttribute());
	}
	
	if (SubJob && SubJob->JobTag.MatchesTagExact(HeroJobProgressItem.JobTag))
	{
		OverrideAttribute(HeroJobProgressItem.Level, UHeroJobAttributeSet::GetSubJobLevelAttribute());
	}

	Multi_OnJobLevelUp(HeroJobProgressItem, OldLevel);
}

void UHeroManagerComponent::Multi_OnJobLevelUp_Implementation(const FHeroJobProgressItem& HeroJobProgressItem, const int32 OldLevel)
{
	OnHeroJobLevelUpDelegate.Broadcast(HeroJobProgressItem, OldLevel);
}

void UHeroManagerComponent::Server_TrySetHeroJobs_Implementation(const UHeroJobDefinition* InMainJob, const UHeroJobDefinition* InSubJob)
{
	TrySetHeroJobs(const_cast<UHeroJobDefinition*>(InMainJob), const_cast<UHeroJobDefinition*>(InSubJob));
}

void UHeroManagerComponent::Client_OnTrySetHeroJobs_Implementation(bool bSuccess)
{
	OnTrySetHeroJobDelegate.Broadcast(bSuccess);
}
