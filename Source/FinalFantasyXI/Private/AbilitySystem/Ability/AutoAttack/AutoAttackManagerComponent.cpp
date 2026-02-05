// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/AutoAttack/AutoAttackManagerComponent.h"

#include "CrimAbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysGameplayTags.h"
#include "AbilitySystem/AttributeSet/AttackerAttributeSet.h"
#include "Net/UnrealNetwork.h"


UAutoAttackManagerComponent::UAutoAttackManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UAutoAttackManagerComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	FDoRepLifetimeParams Params;
	Params.bIsPushBased = true;
	Params.Condition = COND_OwnerOnly;
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, bAutoAttacking, Params);
}

void UAutoAttackManagerComponent::OnRegister()
{
	Super::OnRegister();
	CacheIsNetSimulated();
}

void UAutoAttackManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	CacheIsNetSimulated();
}

void UAutoAttackManagerComponent::PreNetReceive()
{
	Super::PreNetReceive();
	CacheIsNetSimulated();
}

void UAutoAttackManagerComponent::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent)
	{
		return;
	}
	
	AbilitySystemComponent = NewAbilitySystemComponent;
	
	if (AbilitySystemComponent)
	{
		bool bFound = false;
		const float Value = AbilitySystemComponent->GetGameplayAttributeValue(UAttackerAttributeSet::GetAutoAttackDelayAttribute(), bFound);
		AutoAttackDelay = bFound ? Value : 1.f;
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UAttackerAttributeSet::GetAutoAttackDelayAttribute()).
			AddUObject(this, &UAutoAttackManagerComponent::OnAutoAttackDelayAttributeChanged);
		
		const FGameplayTag& PauseAutoAttack = FCrysGameplayTags::Get().Gameplay_State_PauseAutoAttack;
		OnPauseAutoAttackTagChanged(PauseAutoAttack, AbilitySystemComponent->GetGameplayTagCount(PauseAutoAttack));
		AbilitySystemComponent->RegisterGameplayTagEvent(PauseAutoAttack, EGameplayTagEventType::NewOrRemoved).
			AddUObject(this, &UAutoAttackManagerComponent::OnPauseAutoAttackTagChanged);
	}
}

void UAutoAttackManagerComponent::StartAutoAttack()
{
	if (IsAutoAttacking())
	{
		return;
	}
	
	if (!HasAuthority())
	{
		Server_StartAutoAttack();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(AutoAttackTimer, this, &UAutoAttackManagerComponent::ActivateAutoAttackAbility, AutoAttackDelay, false);
	if (bAutoAttackTimerPaused)
	{
		GetWorld()->GetTimerManager().PauseTimer(AutoAttackTimer);
	}
	bAutoAttacking = true;
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
	OnRep_AutoAttacking();
}

void UAutoAttackManagerComponent::StopAutoAttack()
{
	if (!IsAutoAttacking())
	{
		return;
	}
	
	if (!HasAuthority())
	{
		Server_StopAutoAttack();
		return;
	}
	
	GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimer);
	bAutoAttacking = false;
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
	OnRep_AutoAttacking();
}

void UAutoAttackManagerComponent::RestartAutoAttackTimer()
{
	if (IsAutoAttacking() && HasAuthority())
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoAttackTimer);
		GetWorld()->GetTimerManager().SetTimer(AutoAttackTimer, this, &UAutoAttackManagerComponent::ActivateAutoAttackAbility, AutoAttackDelay, false);
		if (bAutoAttackTimerPaused)
		{
			GetWorld()->GetTimerManager().PauseTimer(AutoAttackTimer);
		}
	}
}

bool UAutoAttackManagerComponent::IsAutoAttacking() const
{
	return bAutoAttacking;
}

UAutoAttackAnimationData* UAutoAttackManagerComponent::GetPrimaryAutoAttackAnimationData() const
{
	return PrimaryAutoAttackAnimationData;
}

UAutoAttackAnimationData* UAutoAttackManagerComponent::GetSecondaryAutoAttackAnimationData() const
{
	return SecondaryAutoAttackAnimationData;
}

void UAutoAttackManagerComponent::SetPrimaryAutoAttackAnimationData(UAutoAttackAnimationData* AnimationData)
{
	if (AnimationData)
	{
		PrimaryAutoAttackAnimationData = AnimationData;
		OnPrimaryAutoAttackAnimationDataUpdatedDelegate.Broadcast(PrimaryAutoAttackAnimationData);
	}
}

void UAutoAttackManagerComponent::SetSecondaryAutoAttackAnimationData(UAutoAttackAnimationData* AnimationData)
{
	if (AnimationData)
	{
		SecondaryAutoAttackAnimationData = AnimationData;
		OnSecondaryAutoAttackAnimationDataUpdatedDelegate.Broadcast(SecondaryAutoAttackAnimationData);
	}
}

bool UAutoAttackManagerComponent::HasAuthority() const
{
	return !bCachedIsNetSimulated;
}

void UAutoAttackManagerComponent::OnRep_AutoAttacking()
{
	OnAutoAttackStateChangedDelegate.Broadcast(bAutoAttacking);
}

void UAutoAttackManagerComponent::CacheIsNetSimulated()
{
	bCachedIsNetSimulated = IsNetSimulating();
}

void UAutoAttackManagerComponent::ActivateAutoAttackAbility()
{
#if WITH_SERVER_CODE
	if (AbilitySystemComponent)
	{
		FGameplayEventData Payload;
		Payload.EventTag = FCrysGameplayTags::Get().Ability_GameplayEvent_AutoAttack;
		Payload.Instigator = AbilitySystemComponent->GetAvatarActor();
		Payload.Target = UCrysBlueprintFunctionLibrary::GetAbilityTarget(AbilitySystemComponent->GetOwner(), FCrysGameplayTags::Get().Ability_GameplayEvent_AutoAttack.GetSingleTagContainer());
		// The RandomStream Seed.
		Payload.EventMagnitude = FMath::RandRange(1, MAX_int32);
		
		FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
		AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
	}
#endif // #if WITH_SERVER_CODE	
}

void UAutoAttackManagerComponent::OnAutoAttackDelayAttributeChanged(const FOnAttributeChangeData& Data)
{
	AutoAttackDelay = Data.NewValue;
}

void UAutoAttackManagerComponent::OnPauseAutoAttackTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GetWorld()->GetTimerManager().PauseTimer(AutoAttackTimer);
		bAutoAttackTimerPaused = true;
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(AutoAttackTimer);
		bAutoAttackTimerPaused = false;
	}
}

void UAutoAttackManagerComponent::Server_StartAutoAttack_Implementation()
{
	StartAutoAttack();
}

void UAutoAttackManagerComponent::Server_StopAutoAttack_Implementation()
{
	StopAutoAttack();
}
