// Copyright Soccertitan 2025


#include "AbilitySystem/ExecCalc/DamageExecCalc.h"

#include "CrimGameplayEffectContext.h"
#include "CrimMathStatics.h"
#include "CrysNativeGameplayTags.h"
#include "AbilitySystem/AbilityTypes.h"
#include "AbilitySystem/AttributeSet/AbilityAttributeSet.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "AbilitySystem/AttributeSet/CrysHitPointsAttributeSet.h"
#include "AbilitySystem/AttributeSet/DefenderAttributeSet.h"
#include "AbilitySystem/AttributeSet/GuardAttributeSet.h"
#include "AbilitySystem/AttributeSet/ParryAttributeSet.h"
#include "AbilitySystem/AttributeSet/ShieldAttributeSet.h"


UDamageExecCalc::UDamageExecCalc()
{
	BaseDamageAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	BaseDamageAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingPotencyAttribute();
	HitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	HitChanceAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetOutgoingProbabilityAttribute();
	CriticalHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitChanceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetCriticalHitChanceAttribute();
	DefensePierceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DefensePierceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetDefensePierceAttribute();
	
	IncomingDamageAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    IncomingDamageAttributeDef.AttributeToCapture = UHitPointsAttributeSet::GetDamageAttribute();

	AttackAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAttackAttribute();
	AttackDefenseRatioCapAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	AttackDefenseRatioCapAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetAttackDefenseRatioCapAttribute();
	CriticalHitChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitChanceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetCriticalHitChanceAttribute();
	CriticalHitBonusAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	CriticalHitBonusAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetCriticalHitBonusAttribute();
	DefensePierceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DefensePierceAttributeDef.AttributeToCapture = UAttackerAttributeSet::GetDefensePierceAttribute();
	DamageMultiplierAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;
	DamageMultiplierAttributeDef.AttributeToCapture = UAbilityAttributeSet::GetPotencyMultiplierAttribute();

	DefenseAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	DefenseAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetDefenseAttribute();
	ResistanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ResistanceAttributeDef.AttributeToCapture = UDefenderAttributeSet::GetResistanceAttribute();

	GuardChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardChanceAttributeDef.AttributeToCapture = UGuardAttributeSet::GetGuardChanceAttribute();
	GuardHalfAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardHalfAngleAttributeDef.AttributeToCapture = UGuardAttributeSet::GetGuardHalfAngleAttribute();
	GuardDamageReductionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	GuardDamageReductionAttributeDef.AttributeToCapture = UGuardAttributeSet::GetGuardDamageReductionAttribute();

	ParryChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ParryChanceAttributeDef.AttributeToCapture = UParryAttributeSet::GetParryChanceAttribute();
	ParryHalfAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ParryHalfAngleAttributeDef.AttributeToCapture = UParryAttributeSet::GetParryHalfAngleAttribute();

	BlockChanceAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockChanceAttributeDef.AttributeToCapture = UShieldAttributeSet::GetBlockChanceAttribute();
	BlockHalfAngleAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockHalfAngleAttributeDef.AttributeToCapture = UShieldAttributeSet::GetBlockHalfAngleAttribute();
	BlockDamageReductionAttributeDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	BlockDamageReductionAttributeDef.AttributeToCapture = UShieldAttributeSet::GetBlockDamageReductionAttribute();
	
	UpdateAggregatedRelevantAttributesToCapture();
	
	PerfectEvasionTagContainer.AddTag(Crys::NativeGameplayTag::Ability_State_Perfect_Evasion);
	PerfectHitTagContainer.AddTag(Crys::NativeGameplayTag::Ability_State_Perfect_Hit);

	PerfectCriticalHitTagContainer.AddTag(Crys::NativeGameplayTag::Ability_State_Perfect_CriticalHit);
	ImmuneCriticalHitTagContainer.AddTag(Crys::NativeGameplayTag::Ability_State_Immune_CriticalHit);
	
	DamageGameplayCue = Crys::NativeGameplayTag::GameplayCue_Damage;
}

void UDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	FGameplayEffectSpec* Spec = ExecutionParams.GetOwningSpecForPreExecuteMod();
	FCrimGameplayEffectContext* Context = static_cast<FCrimGameplayEffectContext*>(Spec->GetContext().Get());
	FDamageGameplayContext* DamageGameplayContext = Context->AddCustomDataFragment(FDamageGameplayContext());
	DamageGameplayContext->DamagedAttribute = IncomingDamageAttributeDef.AttributeToCapture;
	
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = Spec->CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = Spec->CapturedTargetTags.GetAggregatedTags();
	
	FGameplayCueParameters GCParams(Spec->GetContext());
	FGameplayEventData EventData;
	EventData.ContextHandle = Spec->GetContext();
	EventData.Instigator = ExecutionParams.GetSourceAbilitySystemComponent()->GetOwnerActor();
	EventData.Target = ExecutionParams.GetTargetAbilitySystemComponent()->GetOwnerActor();
	EventData.InstigatorTags = *EvaluateParams.SourceTags;
	EventData.TargetTags = *EvaluateParams.TargetTags;

	DamageGameplayContext->bHit = IsHit(ExecutionParams, OutExecutionOutput, EvaluateParams);
	
	if (DamageGameplayContext->bHit)
	{
		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
		DamageGameplayContext->bParried = IsParried(ExecutionParams, OutExecutionOutput, EvaluateParams);
		if (!DamageGameplayContext->bParried)
		{
			float BaseDamage = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BaseDamageAttributeDef, EvaluateParams, BaseDamage);
			BaseDamage = FMath::Floor(BaseDamage);
			
			DamageGameplayContext->bCriticalHit = IsCriticalHit(ExecutionParams, OutExecutionOutput, EvaluateParams);
			float Damage = FMath::Floor(CalculateDamage(BaseDamage, DamageGameplayContext, ExecutionParams, OutExecutionOutput, EvaluateParams));
			
			DamageGameplayContext->bBlocked = IsBlocked(ExecutionParams, OutExecutionOutput, EvaluateParams);
			if (DamageGameplayContext->bBlocked)
			{
				float BlockDamageReduction = 0.f;
				ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockDamageReductionAttributeDef, EvaluateParams, BlockDamageReduction);
				Damage = FMath::Floor(Damage * (1 - BlockDamageReduction));
			}
			
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitudeWithBase(IncomingDamageAttributeDef, EvaluateParams, Damage, Damage);
			Damage = FMath::Floor(Damage);
			GCParams.RawMagnitude = Damage;
			EventData.EventMagnitude = Damage;
			OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(IncomingDamageAttributeDef.AttributeToCapture, EGameplayModOp::Override, Damage));
		}
	}
	
	ExecutionParams.GetSourceAbilitySystemComponent()->HandleGameplayEvent(Crys::NativeGameplayTag::Ability_GameplayEvent_DamageSource, &EventData);
	ExecutionParams.GetTargetAbilitySystemComponent()->HandleGameplayEvent(Crys::NativeGameplayTag::Ability_GameplayEvent_DamageTarget, &EventData);
	
	ExecutionParams.GetSourceAbilitySystemComponent()->ExecuteGameplayCue(Crys::NativeGameplayTag::GameplayCue_Damage, GCParams);
	OutExecutionOutput.MarkGameplayCuesHandledManually();
}

const TArray<FGameplayEffectAttributeCaptureDefinition>& UDamageExecCalc::GetAttributeCaptureDefinitions() const
{
	return AggregatedRelevantAttributesToCapture;
}

#if WITH_EDITOR
void UDamageExecCalc::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	UpdateAggregatedRelevantAttributesToCapture();
}
#endif

void UDamageExecCalc::UpdateAggregatedRelevantAttributesToCapture()
{
	AggregatedRelevantAttributesToCapture.Empty();
	AggregatedRelevantAttributesToCapture.Add(BaseDamageAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(HitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefensePierceAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(IncomingDamageAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(AttackAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(AttackDefenseRatioCapAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(CriticalHitBonusAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DefensePierceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(DamageMultiplierAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(DefenseAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(ResistanceAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(GuardChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(GuardHalfAngleAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(GuardDamageReductionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(ParryChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(ParryHalfAngleAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Add(BlockChanceAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BlockHalfAngleAttributeDef);
	AggregatedRelevantAttributesToCapture.Add(BlockDamageReductionAttributeDef);
	
	AggregatedRelevantAttributesToCapture.Append(RelevantAttributesToCapture);
}

float UDamageExecCalc::CalculateDamage(const float BaseDamage, FDamageGameplayContext* DamageGameplayContext, const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	float Attack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackAttributeDef, EvaluateParams, Attack);
	Attack = FMath::Max(1.f, Attack);
	
	float DefensePierce = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefensePierceAttributeDef, EvaluateParams, DefensePierce);
	float Defense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DefenseAttributeDef, EvaluateParams, Defense);
	Defense = FMath::Max(1.f, Defense * (1 - DefensePierce));
	
	float DamageRatioCap = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(AttackDefenseRatioCapAttributeDef, EvaluateParams, DamageRatioCap);
	float DamageRatio = FMath::Min(Attack/Defense, DamageRatioCap);
	
	float DamageRatioCritBonus = 0.f;
	if (DamageGameplayContext->bCriticalHit)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitBonusAttributeDef, EvaluateParams, DamageRatioCritBonus);
	}
	
	float GuardDamageReduction = 0.f;
	DamageGameplayContext->bGuarded = IsGuarded(ExecutionParams, OutExecutionOutput, EvaluateParams);
	if (DamageGameplayContext->bGuarded)
	{
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardDamageReductionAttributeDef, EvaluateParams, GuardDamageReduction);
	}
	
	DamageRatio = FMath::Max(DamageRatio + DamageRatioCritBonus - GuardDamageReduction, 0.f);
	
	float DamageMultiplier = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageMultiplierAttributeDef, EvaluateParams, DamageMultiplier);
	float Resistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ResistanceAttributeDef, EvaluateParams, Resistance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float DamageRandomizerMultiplier = FMath::FRandRange(DamageRandomizerMin.GetValueAtLevel(Spec.GetLevel()), DamageRandomizerMax.GetValueAtLevel(Spec.GetLevel()));
	
	return BaseDamage * DamageMultiplier * DamageRatio * (1 - Resistance) * DamageRandomizerMultiplier;
}

bool UDamageExecCalc::IsHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	if (PerfectHitTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return true;
	}
	if (PerfectEvasionTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return false;
	}
	
	float HitChance = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HitChanceAttributeDef, EvaluateParams, HitChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	HitChance = FMath::Clamp(HitChance, MinHitChance.GetValueAtLevel(Spec.GetLevel()), MaxHitChance.GetValueAtLevel(Spec.GetLevel()));
	
	return HitChance >= FMath::RandRange(0.f, 1.f);
}

bool UDamageExecCalc::IsCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	if (PerfectCriticalHitTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return true;
	}
	if (ImmuneCriticalHitTagContainer.HasAny(*EvaluateParams.SourceTags))
	{
		return false;
	}
	
	float CriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CriticalHitChanceAttributeDef, EvaluateParams, CriticalHitChance);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	CriticalHitChance = FMath::Clamp(CriticalHitChance, MinHitChance.GetValueAtLevel(Spec.GetLevel()), MaxHitChance.GetValueAtLevel(Spec.GetLevel()));
	return CriticalHitChance >= FMath::RandRange(0.f, 1.f);
}

bool UDamageExecCalc::IsParried(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	FGameplayTagContainer RequiredTargetTags;
	RequiredTargetTags.AddTag(Crys::NativeGameplayTag::Ability_State_Parry);
	RequiredTargetTags.AddTag(Crys::NativeGameplayTag::Ability_State_CombatStance);
	
	if (EvaluateParams.SourceTags->HasTag(Crys::NativeGameplayTag::Ability_State_Ignore_Parry) == false ||
		EvaluateParams.TargetTags->HasAll(RequiredTargetTags))
	{
		AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		float ConeHalfAngle = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ParryHalfAngleAttributeDef, EvaluateParams, ConeHalfAngle);
		
		if (UCrimMathStatics::IsInCone(TargetActor->GetActorLocation(), TargetActor->GetActorForwardVector(), ConeHalfAngle, SourceActor->GetActorLocation()))
		{
			float ParryChance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(ParryChanceAttributeDef, EvaluateParams, ParryChance);
			
			const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
			ParryChance = FMath::Clamp(ParryChance, MinParryChance.GetValueAtLevel(Spec.GetLevel()), MaxParryChance.GetValueAtLevel(Spec.GetLevel()));
			
			return ParryChance >= FMath::RandRange(0.f, 1.f);
		}
	}
	
	return false;
}

bool UDamageExecCalc::IsGuarded(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	FGameplayTagContainer RequiredTargetTags;
	RequiredTargetTags.AddTag(Crys::NativeGameplayTag::Ability_State_Guard);
	RequiredTargetTags.AddTag(Crys::NativeGameplayTag::Ability_State_CombatStance);

	if (EvaluateParams.SourceTags->HasTag(Crys::NativeGameplayTag::Ability_State_Ignore_Guard) == false ||
		EvaluateParams.TargetTags->HasAll(RequiredTargetTags))
	{
		AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		float ConeHalfAngle = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardHalfAngleAttributeDef, EvaluateParams, ConeHalfAngle);
		
		if (UCrimMathStatics::IsInCone(TargetActor->GetActorLocation(), TargetActor->GetActorForwardVector(), ConeHalfAngle, SourceActor->GetActorLocation()))
		{
			float GuardChance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GuardChanceAttributeDef, EvaluateParams, GuardChance);
			
			const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
			GuardChance = FMath::Clamp(GuardChance, MinGuardChance.GetValueAtLevel(Spec.GetLevel()), MaxGuardChance.GetValueAtLevel(Spec.GetLevel()));
			
			return GuardChance >= FMath::RandRange(0.f, 1.f);
		}
	}
	
	return false;
}

bool UDamageExecCalc::IsBlocked(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput, const FAggregatorEvaluateParameters& EvaluateParams) const
{
	if (EvaluateParams.SourceTags->HasTag(Crys::NativeGameplayTag::Ability_State_Ignore_Block) == false ||
		EvaluateParams.TargetTags->HasTag(Crys::NativeGameplayTag::Ability_State_Block))
	{
		AActor* SourceActor = ExecutionParams.GetSourceAbilitySystemComponent()->GetAvatarActor();
		AActor* TargetActor = ExecutionParams.GetTargetAbilitySystemComponent()->GetAvatarActor();
		float ConeHalfAngle = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockHalfAngleAttributeDef, EvaluateParams, ConeHalfAngle);
		
		if (UCrimMathStatics::IsInCone(TargetActor->GetActorLocation(), TargetActor->GetActorForwardVector(), ConeHalfAngle, SourceActor->GetActorLocation()))
		{
			float BlockChance = 0.f;
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(BlockChanceAttributeDef, EvaluateParams, BlockChance);
			
			const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
			BlockChance = FMath::Clamp(BlockChance, MinBlockChance.GetValueAtLevel(Spec.GetLevel()), MaxBlockChance.GetValueAtLevel(Spec.GetLevel()));
			
			return BlockChance >= FMath::RandRange(0.f, 1.f);
		}
	}
	
	return false;
}
