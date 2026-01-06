// Copyright Soccertitan 2025


#include "Player/Hero/HeroPlayerController.h"

#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "TargetingSystemComponent.h"
#include "CrysLogChannels.h"
#include "EnhancedInputSubsystems.h"
#include "EquipmentSystem/EquipmentSystemBlueprintFunctionLibrary.h"
#include "Filter/TargetPointFilter_Cone.h"
#include "Input/AbilityInputManagerComponent.h"
#include "Input/CrysEnhancedInputComponent.h"
#include "Input/HeroInputSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/Hero/HeroPlayerState.h"
#include "Player/Hero/WeaponSetManagerComponent.h"

AHeroPlayerController::AHeroPlayerController()
{
	AbilityInputManagerComponent = CreateDefaultSubobject<UAbilityInputManagerComponent>(TEXT("AbilityInputManagerComponent"));
	AbilityInputManagerComponent->SetIsReplicated(false);

	WeaponSetManagerComponent = CreateDefaultSubobject<UWeaponSetManagerComponent>(TEXT("WeaponSetManagerComponent"));

	UTargetPointFilter_Cone* Filter_Cone = CreateDefaultSubobject<UTargetPointFilter_Cone>(TEXT("Filter_Cone"));
	Filter_Cone->ConeHalfAngle = 60.f;
	TargetFilters.Add(Filter_Cone);
}

void AHeroPlayerController::BeginPlay()
{
	Super::BeginPlay();

	EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (EnhancedInputSubsystem && HeroInputSet)
	{
		EnhancedInputSubsystem->AddMappingContext(HeroInputSet->ContextLocomotion, HeroInputSet->ContextLocomotionPriority);
		EnhancedInputSubsystem->AddMappingContext(HeroInputSet->ContextAbility, HeroInputSet->ContextAbilityPriority);
	}
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
	InitializeWeaponSetComponent();
	TryInitOverlay();
}

void AHeroPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	InitializeAbilitySystemComponent();
	InitializeWeaponSetComponent();
	TryInitOverlay();
}

void AHeroPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalController() && TargetSystemComponent && !TargetSystemComponent->IsCameraLocked())
	{
		TargetSystemComponent->SetTarget(TargetSystemComponent->FindNearestTarget(TargetFilters));
	}
}

void AHeroPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	SetupHeroInputSet();
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

	if (EnhancedInputSubsystem && HeroInputSet)
	{
		EnhancedInputSubsystem->AddMappingContext(HeroInputSet->ContextAbility, HeroInputSet->ContextAbilityPriority);
	}
}

void AHeroPlayerController::RemoveAbilityMappingContextAndReleaseAbilityInputs()
{
	if (EnhancedInputSubsystem && HeroInputSet)
	{
		EnhancedInputSubsystem->RemoveMappingContext(HeroInputSet->ContextAbility);

		FInputActionValue FalseActionValue(false);
		InputActionAbilityToggle(FalseActionValue);

		if (HeroInputSet->InputConfigGenericAbility)
		{
			for (const FInputMap& Input : HeroInputSet->InputConfigGenericAbility->Inputs)
			{
				InputActionGenericAbility(FalseActionValue, Input.Tag);
			}
		}

		if (HeroInputSet->InputConfigPrimaryAbility)
		{
			for (const FInputMap& Input : HeroInputSet->InputConfigPrimaryAbility->Inputs)
			{
				InputActionGenericAbility(FalseActionValue, Input.Tag);
			}
		}

		if (HeroInputSet->InputConfigSubAbility)
		{
			for (const FInputMap& Input : HeroInputSet->InputConfigSubAbility->Inputs)
			{
				InputActionGenericAbility(FalseActionValue, Input.Tag);
			}
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
	if (GetPawn())
	{
		TargetSystemComponent = GetPawn()->FindComponentByClass<UTargetingSystemComponent>();
		return;
	}
	TargetSystemComponent = nullptr;
}

void AHeroPlayerController::InitializeWeaponSetComponent()
{
	WeaponSetManagerComponent->InitializeWeaponSetManager(AbilityInputManagerComponent,
		UEquipmentSystemBlueprintFunctionLibrary::GetEquipmentManagerComponent(GetPlayerState<AHeroPlayerState>()));
}

void AHeroPlayerController::SetupHeroInputSet()
{
	UCrysEnhancedInputComponent* EnhancedInputComponent = CastChecked<UCrysEnhancedInputComponent>(InputComponent);

	if (!HeroInputSet)
	{
		UE_LOG(LogCrys, Error, TEXT("The HeroInputSet is null [%s]"), *GetName());
		return;
	}

	//-----------------------------------------------------------------
	// Locomotion Input Actions
	//-----------------------------------------------------------------
	if (HeroInputSet->InputActionMovement) EnhancedInputComponent->BindAction(HeroInputSet->InputActionMovement, ETriggerEvent::Triggered, this, &AHeroPlayerController::InputActionMovement);
	if (HeroInputSet->InputActionCamera) EnhancedInputComponent->BindAction(HeroInputSet->InputActionCamera, ETriggerEvent::Triggered, this, &AHeroPlayerController::InputActionCamera);
	if (HeroInputSet->InputActionCameraZoomIn) EnhancedInputComponent->BindAction(HeroInputSet->InputActionCameraZoomIn, ETriggerEvent::Triggered, this, &AHeroPlayerController::InputActionCameraZoomIn);
	if (HeroInputSet->InputActionCameraZoomOut) EnhancedInputComponent->BindAction(HeroInputSet->InputActionCameraZoomOut, ETriggerEvent::Triggered, this, &AHeroPlayerController::InputActionCameraZoomOut);

	//-----------------------------------------------------------------
	// Ability Input Actions
	//-----------------------------------------------------------------
	if (HeroInputSet->InputActionAbilityToggle) EnhancedInputComponent->BindAction(HeroInputSet->InputActionAbilityToggle, ETriggerEvent::Triggered, this, &AHeroPlayerController::InputActionAbilityToggle);
	
	EnhancedInputComponent->BindInputActions(
			HeroInputSet->InputConfigPrimaryAbility,
			this,
			&AHeroPlayerController::InputActionPrimaryAbility);
	EnhancedInputComponent->BindInputActions(
			HeroInputSet->InputConfigSubAbility,
			this,
			&AHeroPlayerController::InputActionSubAbility);
	EnhancedInputComponent->BindInputActions(
		HeroInputSet->InputConfigGenericAbility,
		this,
		&AHeroPlayerController::InputActionGenericAbility);
}

void AHeroPlayerController::InputActionMovement(const FInputActionValue& Value)
{
	if(APawn* ControlledPawn = GetPawn())
	{
		FRotator Rotation = ControlledPawn->GetControlRotation();
		FVector ForwardVector = UKismetMathLibrary::GetForwardVector(FRotator(0,Rotation.Yaw,0));
		FVector RightVector = UKismetMathLibrary::GetRightVector(FRotator(0,Rotation.Yaw,0));
		ControlledPawn->AddMovementInput(ForwardVector, Value[1], false);
		ControlledPawn->AddMovementInput(RightVector, Value[0], false);
	}
}

void AHeroPlayerController::InputActionCamera(const FInputActionValue& Value)
{
	if(APawn* ControlledPawn = GetPawn())
	{
		ControlledPawn->AddControllerPitchInput(Value[1]);
		ControlledPawn->AddControllerYawInput(Value[0]);
	}
}

void AHeroPlayerController::InputActionCameraZoomIn(const FInputActionValue& Value)
{
	//TODO: Add camera zoom logic.
}

void AHeroPlayerController::InputActionCameraZoomOut(const FInputActionValue& Value)
{
	//TODO: Add camera zoom logic.
}

void AHeroPlayerController::InputActionPrimaryAbility(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (Value.Get<bool>())
	{
		if (bAbilityTogglePressed == false)
		{
			OnAbilityInputPressed(InputTag);
		}
	}
	else
	{
		OnAbilityInputReleased(InputTag);
	}
}

void AHeroPlayerController::InputActionSubAbility(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (Value.Get<bool>())
	{
		if (bAbilityTogglePressed == true)
		{
			OnAbilityInputPressed(InputTag);
		}
	}
	else
	{
		OnAbilityInputReleased(InputTag);
	}
}

void AHeroPlayerController::InputActionGenericAbility(const FInputActionValue& Value, FGameplayTag InputTag)
{
	if (Value.Get<bool>())
	{
		OnAbilityInputPressed(InputTag);
	}
	else
	{
		OnAbilityInputReleased(InputTag);
	}
}

void AHeroPlayerController::InputActionAbilityToggle(const FInputActionValue& Value)
{
	bAbilityTogglePressed = Value.Get<bool>();
	OnAbilityToggleStateChangedDelegate.Broadcast(Value.Get<bool>());
}

void AHeroPlayerController::OnAbilityInputPressed(FGameplayTag InputTag)
{
	AbilityInputManagerComponent->InputTagPressed(InputTag);
	OnInputPressedDelegate.Broadcast(InputTag);
}

void AHeroPlayerController::OnAbilityInputReleased(FGameplayTag InputTag)
{
	AbilityInputManagerComponent->InputTagReleased(InputTag);
	OnInputReleasedDelegate.Broadcast(InputTag);
}
