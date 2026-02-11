// Copyright Soccertitan 2025


#include "Player/Hero/HeroPlayerController.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "TargetingSystemComponent.h"
#include "ActionSystem/CrysActionManagerComponent.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Player/Hero/HeroPlayerState.h"

AHeroPlayerController::AHeroPlayerController()
{
	AbilityInputManagerComponent = CreateDefaultSubobject<UAbilityInputManagerComponent>(TEXT("AbilityInputManagerComponent"));
	AbilityInputManagerComponent->SetIsReplicated(false);
	
	ActionManagerComponent = CreateDefaultSubobject<UCrysActionManagerComponent>(TEXT("ActionManagerComponent"));
	ActionManagerComponent->SetIsReplicated(false);
}

void AHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AHeroPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	InitializeAbilitySystemComponent();
}

void AHeroPlayerController::AcknowledgePossession(class APawn* P)
{
	Super::AcknowledgePossession(P);

	InitializeAbilitySystemComponent();
	InitializeTargetSystemComponent();
	TryInitOverlay();
}

void AHeroPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystemComponent();
	TryInitOverlay();
}

void AHeroPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AHeroPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	AbilityInputManagerComponent->ProcessAbilityInput(DeltaTime, bGamePaused);

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

UTargetingSystemComponent* AHeroPlayerController::GetTargetingSystemComponent_Implementation() const
{
	return TargetSystemComponent;
}

void AHeroPlayerController::OnTargetedPointUpdated(UTargetPointComponent* TargetPointComponent)
{
	if (TargetPointComponent)
	{
		if (EnhancedInputSubsystem)
		{
			EnhancedInputSubsystem->AddMappingContext(TargetedPointInputMappingContext, TargetedPointInputMappingContextPriority);
		}
	}
	else
	{
		if (EnhancedInputSubsystem)
		{
			EnhancedInputSubsystem->RemoveMappingContext(TargetedPointInputMappingContext);
		}
	}
}

void AHeroPlayerController::TryInitOverlay()
{
	if (AbilitySystemComponent)
	{
		InitializeHUD();
	}
}

void AHeroPlayerController::InitializeAbilitySystemComponent()
{
	AbilitySystemComponent = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(
		GetPlayerState<AHeroPlayerState>(), true);
	if (AbilitySystemComponent)
	{
		AbilityInputManagerComponent->InitializeAbilitySystemComponent(AbilitySystemComponent);
	}
}

void AHeroPlayerController::InitializeTargetSystemComponent()
{
	if (TargetSystemComponent)
	{
		TargetSystemComponent->OnTargetedPointUpdatedDelegate.RemoveAll(this);
	}

	TargetSystemComponent = nullptr;
	if (GetPawn())
	{
		TargetSystemComponent = GetPawn()->FindComponentByClass<UTargetingSystemComponent>();
	}
	
	if (TargetSystemComponent)
	{
		OnTargetedPointUpdated(TargetSystemComponent->GetTargetedPoint());
		TargetSystemComponent->OnTargetedPointUpdatedDelegate.AddUniqueDynamic(this, &ThisClass::OnTargetedPointUpdated);
	}
}
