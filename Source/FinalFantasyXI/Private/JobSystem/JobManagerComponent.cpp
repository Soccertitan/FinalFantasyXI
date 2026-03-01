// Copyright Soccertitan 2025


#include "JobSystem/JobManagerComponent.h"

#include "AbilitySet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CrimAbilitySystemComponent.h"
#include "CrysLogChannels.h"
#include "AbilitySystem/AttributeSet/JobAttributeSet.h"
#include "AbilitySystem/AttributeSet/ManaPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/PrimaryAttributeSet.h"
#include "Attribute/HitPointsAttributeSet.h"
#include "JobSystem/RaceDefinition.h"
#include "JobSystem/JobSystemBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "System/CrysAssetManager.h"


UJobManagerComponent::UJobManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UJobManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, JobProgressContainer);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, RaceDefinition, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MainJob, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, SubJob, COND_None, REPNOTIFY_Always);

	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, JobManagerData, Params);
}

void UJobManagerComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
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
			OverrideAttribute(JobManagerData.Level, UPrimaryAttributeSet::GetLevelAttribute());
			OverrideAttribute(JobManagerData.SubJobEfficiency, UJobAttributeSet::GetSubJobEffectivenessAttribute());
			ApplyRaceTraits();
			if (MainJob)
			{
				OverrideAttribute(FindJobProgressItem(MainJob->JobTag).Level, UJobAttributeSet::GetMainJobLevelAttribute());
				ApplyMainJobTraits();
			}
			if (SubJob)
			{
				OverrideAttribute(FindJobProgressItem(SubJob->JobTag).Level, UJobAttributeSet::GetSubJobLevelAttribute());
				ApplySubJobTraits();
			}
			
			BindToAttributeDelegates();
			ApplyBaseAttributes();
		}
	}
}

TArray<FJobProgressItem> UJobManagerComponent::GetJobProgressItems() const
{
	return JobProgressContainer.Items;
}

FJobProgressItem UJobManagerComponent::FindJobProgressItem(const FGameplayTag JobTag) const
{
	return JobProgressContainer.GetJobProgressItem(JobTag);
}

void UJobManagerComponent::AddOrSetJobProgressItem(const FJobProgressItem& InJobProgressItem)
{
	if (!HasAuthority())
	{
		return;
	}

	if (!InJobProgressItem.IsValid())
	{
		return;
	}

	if (FJobProgressItem* ProgressItem = JobProgressContainer.Items.FindByKey(InJobProgressItem.JobTag))
	{
		ProgressItem->Level = InJobProgressItem.Level;
		ProgressItem->Experience = InJobProgressItem.Experience;
		JobProgressContainer.MarkItemDirty(*ProgressItem);
		OnJobProgressUpdatedDelegate.Broadcast(*ProgressItem);
	}
	else
	{
		JobProgressContainer.AddJobProgressItem(InJobProgressItem);
	}

	if (MainJob && MainJob->JobTag == InJobProgressItem.JobTag)
	{
		OverrideAttribute(InJobProgressItem.Level, UJobAttributeSet::GetMainJobLevelAttribute());
	}
	else if (SubJob && SubJob->JobTag == InJobProgressItem.JobTag)
	{
		OverrideAttribute(InJobProgressItem.Level, UJobAttributeSet::GetMainJobLevelAttribute());
	}
}

void UJobManagerComponent::AddExperienceForJob(const UJobDefinition* Job, const int32 Experience)
{
	if (!HasAuthority() || Experience <= 0)
	{
		return;
	}

	if (!Job || !Job->JobTag.IsValid())
	{
		UE_LOG(LogCrys, Error, TEXT("The Job does not have a valid JobTag [%s]"), *GetNameSafe(Job));
		return;
	}

	JobProgressContainer.AddExperience(Job->JobTag, Job->ExperienceRequirement, Experience);
}

void UJobManagerComponent::RestoreJobProgressItems(const TArray<FJobProgressItem>& InJobProgressItems)
{
	if (!HasAuthority())
	{
		return;
	}

	JobProgressContainer.Reset();
	
	for (const FJobProgressItem& Item : InJobProgressItems)
	{
		if (Item.IsValid())
		{
			JobProgressContainer.AddJobProgressItem(Item);
		}
	}
	
	if (AbilitySystemComponent)
	{
		bSkipApplyingBaseStats = true;
		if (MainJob)
		{
			OverrideAttribute(FindJobProgressItem(MainJob->JobTag).Level, UJobAttributeSet::GetMainJobLevelAttribute());
		}
		if (SubJob)
		{
			OverrideAttribute(FindJobProgressItem(SubJob->JobTag).Level, UJobAttributeSet::GetSubJobLevelAttribute());
		}
		bSkipApplyingBaseStats = false;
		ApplyBaseAttributes();
	}
}

void UJobManagerComponent::TrySetJobs(UJobDefinition* InMainJob, UJobDefinition* InSubJob)
{
	if (!HasAuthority())
	{
		Server_TrySetJobs(InMainJob, InSubJob);
		return;
	}

	if (!InMainJob || InMainJob == InSubJob)
	{
		Client_OnTrySetJobs(false);
		return;
	}
	
	bSkipApplyingBaseStats = true;
	
	// Don't allow changing to a MainJob that doesn't have existing progress.
	const FJobProgressItem MainJobProgress = FindJobProgressItem(InMainJob->JobTag);
	if (!MainJobProgress.IsValid())
	{
		bSkipApplyingBaseStats = false;
		Client_OnTrySetJobs(false);
		return;
	}
	
	bool bMainJobChanged = false;
	if (MainJob != InMainJob)
	{
		MainJob = InMainJob;
		bMainJobChanged = true;
	}
	
	bool bSubJobChanged = false;
	FJobProgressItem SubJobProgress = InSubJob ? FindJobProgressItem(InSubJob->JobTag) : FJobProgressItem();
	if (JobManagerData.bSubJobUnlocked && SubJobProgress.IsValid())
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
		OverrideAttribute(MainJobProgress.Level, UJobAttributeSet::GetMainJobLevelAttribute());
		ApplyMainJobTraits();
		OverrideAttribute(SubJobProgress.Level, UJobAttributeSet::GetSubJobLevelAttribute());
		ApplySubJobTraits();
	}
	
	bSkipApplyingBaseStats = false;
	if (AbilitySystemComponent)
	{
		ApplyBaseAttributes();
	}

	if (bMainJobChanged)
	{
		OnMainJobChangedDelegate.Broadcast();
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MainJob, this);
	}
	if (bSubJobChanged)
	{
		OnSubJobChangedDelegate.Broadcast();
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, SubJob, this);
	}

	Client_OnTrySetJobs(true);
}

void UJobManagerComponent::SetRaceDefinition(URaceDefinition* InRaceDefinition)
{
	if (!InRaceDefinition || !HasAuthority())
	{
		return;
	}

	if (RaceDefinition == InRaceDefinition)
	{
		return;
	}

	RaceDefinition = InRaceDefinition;
	if (AbilitySystemComponent)
	{
		ApplyRaceTraits();
		ApplyBaseAttributes();
	}
	OnRep_RaceDefinition();
}

const FJobManagerData& UJobManagerComponent::GetJobManagerData() const
{
	return JobManagerData;
}

void UJobManagerComponent::SetJobManagerData(const FJobManagerData& InData)
{
	if (!HasAuthority())
	{
		return;
	}
	
	int32 OldLevel = JobManagerData.Level;
	JobManagerData = InData;
	if (OldLevel != JobManagerData.Level)
	{
		OverrideAttribute(JobManagerData.Level, UPrimaryAttributeSet::GetLevelAttribute());
	}

	OnJobManagerDataUpdatedDelegate.Broadcast();
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, JobManagerData, this);
}

bool UJobManagerComponent::AddExperience(const int32 Experience)
{
	if (!HasAuthority() || Experience == 0 || !RaceDefinition)
	{
		return false;
	}

	bool bLevelUpdated = false;
	int32 OldLevel = JobManagerData.Level;

	if (Experience > 0)
	{
		bLevelUpdated = UJobSystemBlueprintFunctionLibrary::AddExperience(JobManagerData.Level, JobManagerData.Experience, 
			Experience, RaceDefinition->ExperienceRequirement, JobManagerData.MaxLevel);
	}
	else
	{
		bLevelUpdated = UJobSystemBlueprintFunctionLibrary::SubtractExperience(JobManagerData.Level, JobManagerData.Experience, 
			FMath::Abs(Experience), RaceDefinition->ExperienceRequirement);
	}
	
	if (bLevelUpdated)
	{
		if (JobManagerData.Level > OldLevel)
		{
			Multi_OnLevelUp(OldLevel);
		}
		else
		{
			Multi_OnLevelDown(OldLevel);
		}
		OverrideAttribute(JobManagerData.Level, UPrimaryAttributeSet::GetLevelAttribute());
	}

	OnJobManagerDataUpdatedDelegate.Broadcast();
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, JobManagerData, this);
	return bLevelUpdated;
}

bool UJobManagerComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UJobManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	CacheIsNetSimulated();
	
	if (JobProgressContainer.Items.IsEmpty())
	{
		RestoreJobProgressItems(StartingJobProgress);
	}
}

void UJobManagerComponent::PreNetReceive()
{
	Super::PreNetReceive();

	CacheIsNetSimulated();
}

void UJobManagerComponent::OnRegister()
{
	Super::OnRegister();

	CacheIsNetSimulated();
	JobProgressContainer.Owner = this;
}

void UJobManagerComponent::OnRep_RaceDefinition()
{
	OnRaceChangedDelegate.Broadcast();
}

void UJobManagerComponent::OnRep_JobManagerData()
{
	OnJobManagerDataUpdatedDelegate.Broadcast();
}

void UJobManagerComponent::OnRep_MainJob()
{
	OnMainJobChangedDelegate.Broadcast();
}

void UJobManagerComponent::OnRep_SubJob()
{
	OnSubJobChangedDelegate.Broadcast();
}

void UJobManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UJobManagerComponent::OverrideAttribute(const float Value, const FGameplayAttribute& Attribute)
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

void UJobManagerComponent::ApplyBaseAttributes()
{
	UGameplayEffect* BaseStats = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("BaseStats")));
	BaseStats->DurationPolicy = EGameplayEffectDurationType::Instant;
	FPrimaryAttributesCalculated CalculatedAttributes(
		RaceDefinition, AbilitySystemComponent->GetNumericAttribute(UPrimaryAttributeSet::GetLevelAttribute()), 
		MainJob, AbilitySystemComponent->GetNumericAttribute(UJobAttributeSet::GetMainJobLevelAttribute()),
		SubJob, AbilitySystemComponent->GetNumericAttribute(UJobAttributeSet::GetSubJobLevelAttribute()),
		AbilitySystemComponent->GetNumericAttribute(UJobAttributeSet::GetSubJobEffectivenessAttribute()));
	
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

void UJobManagerComponent::ApplyRaceTraits()
{
	AbilitySet_GrantedHandles_RaceTraits.TakeFromAbilitySystem(AbilitySystemComponent);

	if (RaceDefinition)
	{
		const UAbilitySet* AbilitySet = RaceDefinition->Traits.Get();
		if (!AbilitySet)
		{
			AbilitySet = UCrysAssetManager::Get().GetAsset(RaceDefinition->Traits, false);
		}

		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, &AbilitySet_GrantedHandles_RaceTraits);
		}
	}
}

void UJobManagerComponent::ApplyMainJobTraits()
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

void UJobManagerComponent::ApplySubJobTraits()
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

void UJobManagerComponent::BindToAttributeDelegates()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UPrimaryAttributeSet::GetLevelAttribute()).AddUObject(this, &UJobManagerComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetMainJobLevelAttribute()).AddUObject(this, &UJobManagerComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobLevelAttribute()).AddUObject(this, &UJobManagerComponent::OnAttributeChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobEffectivenessAttribute()).AddUObject(this, &UJobManagerComponent::OnAttributeChanged);
}

void UJobManagerComponent::RemoveBindToAttributeDelegates()
{
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UPrimaryAttributeSet::GetLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetMainJobLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobLevelAttribute()).RemoveAll(this);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		UJobAttributeSet::GetSubJobEffectivenessAttribute()).RemoveAll(this);
}

void UJobManagerComponent::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	if (!bSkipApplyingBaseStats)
	{
		ApplyBaseAttributes();
	}
}

void UJobManagerComponent::Multi_OnLevelUp_Implementation(const int32 OldLevel)
{
	OnLevelUpDelegate.Broadcast(OldLevel);
}

void UJobManagerComponent::Multi_OnLevelDown_Implementation(const int32 OldLevel)
{
	OnLevelDownDelegate.Broadcast(OldLevel);
}

void UJobManagerComponent::Internal_OnJobLevelUp(const FJobProgressItem& JobProgressItem, const int32 OldLevel)
{
	if (MainJob && MainJob->JobTag.MatchesTagExact(JobProgressItem.JobTag))
	{
		OverrideAttribute(JobProgressItem.Level, UJobAttributeSet::GetMainJobLevelAttribute());
	}
	
	if (SubJob && SubJob->JobTag.MatchesTagExact(JobProgressItem.JobTag))
	{
		OverrideAttribute(JobProgressItem.Level, UJobAttributeSet::GetSubJobLevelAttribute());
	}

	Multi_OnJobLevelUp(JobProgressItem, OldLevel);
}

void UJobManagerComponent::Multi_OnJobLevelUp_Implementation(const FJobProgressItem& JobProgressItem, const int32 OldLevel)
{
	OnJobLevelUpDelegate.Broadcast(JobProgressItem, OldLevel);
}

void UJobManagerComponent::Server_TrySetJobs_Implementation(const UJobDefinition* InMainJob, const UJobDefinition* InSubJob)
{
	TrySetJobs(const_cast<UJobDefinition*>(InMainJob), const_cast<UJobDefinition*>(InSubJob));
}

void UJobManagerComponent::Client_OnTrySetJobs_Implementation(bool bSuccess)
{
	OnTrySetJobDelegate.Broadcast(bSuccess);
}
