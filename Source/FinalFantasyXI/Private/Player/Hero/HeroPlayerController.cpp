// Copyright Soccertitan 2025


#include "Player/Hero/HeroPlayerController.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimActionManagerComponent.h"
#include "TargetingSystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Player/Hero/HeroPlayerState.h"

AHeroPlayerController::AHeroPlayerController()
{
	ActionManagerComponent = CreateDefaultSubobject<UCrimActionManagerComponent>(TEXT("ActionManagerComponent"));
	
	AbilityInputManagerComponent = CreateDefaultSubobject<UAbilityInputManagerComponent>(TEXT("AbilityInputManagerComponent"));
	AbilityInputManagerComponent->SetIsReplicated(false);
}

void AHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
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

	ActionManagerComponent->InitActionManagerComponent();
}

void AHeroPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	AbilityInputManagerComponent->ProcessAbilityInput(DeltaTime, bGamePaused);

	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void AHeroPlayerController::OnRootWidgetAdded_Implementation()
{
	Super::OnRootWidgetAdded_Implementation();

	RemoveAbilityMappingContextAndReleaseAbilityInputs();
}

void AHeroPlayerController::OnRootWidgetRemoved_Implementation()
{
	Super::OnRootWidgetRemoved_Implementation();

	if (EnhancedInputSubsystem && AbilityActionSet)
	{
		EnhancedInputSubsystem->AddMappingContext(AbilityActionSet->InputMappingContext, AbilityActionSet->InputMappingContextPriority);
	}
}

void AHeroPlayerController::RemoveAbilityMappingContextAndReleaseAbilityInputs()
{
	if (EnhancedInputSubsystem && AbilityActionSet)
	{
		EnhancedInputSubsystem->RemoveMappingContext(AbilityActionSet->InputMappingContext);

		AbilityInputManagerComponent->ReleaseAbilityInput();
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
	if (GetPawn())
	{
		TargetSystemComponent = GetPawn()->FindComponentByClass<UTargetingSystemComponent>();
		return;
	}
	TargetSystemComponent = nullptr;
}
