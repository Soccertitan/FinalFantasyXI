// Copyright Soccertitan 2025


#include "AbilitySystem/Ability/Task/AbilityTask_WaitConfirmTarget.h"

#include "AbilitySystemComponent.h"
#include "CrysBlueprintFunctionLibrary.h"
#include "CrysLogChannels.h"
#include "CrysNativeGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "TargetPointComponent.h"
#include "AbilitySystem/AbilityTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "Engine/OverlapResult.h"
#include "Settings/FinalFantasyXIGameData.h"
#include "System/CrysAssetManager.h"

UAbilityTask_WaitConfirmTarget* UAbilityTask_WaitConfirmTarget::WaitConfirmTarget(UGameplayAbility* OwningAbility,
    AActor* InitialTarget, EAbilityTargetType TargetType, TSubclassOf<UUserWidget> TargetWidgetClass,
	FName TargetWidgetParentSocket, float TargetingRange, float CheckFrequency, float BreakTargetingDelay)
{
	UAbilityTask_WaitConfirmTarget* NewObject = NewAbilityTask<UAbilityTask_WaitConfirmTarget>(OwningAbility);
	NewObject->CurrentTarget = InitialTarget;
	NewObject->TargetWidgetClass = TargetWidgetClass;
	NewObject->TargetType =	TargetType;
	NewObject->TargetingRange = TargetingRange;
	NewObject->TargetWidgetParentSocket = TargetWidgetParentSocket;
	NewObject->CheckFrequency = FMath::Max(0.1f, CheckFrequency);
	NewObject->BreakTargetingDelay = FMath::Max(0.f, BreakTargetingDelay);
	return NewObject;
}

void UAbilityTask_WaitConfirmTarget::OnDestroy(bool bInOwnerFinished)
{
	RemoveInputActions();
	
	if (IsValid(TargetWidgetComponent))
	{
		TargetWidgetComponent->DestroyComponent();
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAbilityTask_WaitConfirmTarget::Activate()
{
	Super::Activate();
	
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		PlayerController = Ability->GetCurrentActorInfo()->PlayerController.Get();
		CameraComponent = GetAvatarActor()->FindComponentByClass<UCameraComponent>();
		BindToInputActions();
		SetupInitialTarget();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UAbilityTask_WaitConfirmTarget::OnReplicatedConfirmCallback);
		AbilitySystemComponent.Get()->AbilityTargetDataCancelledDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UAbilityTask_WaitConfirmTarget::OnReplicatedCancelCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);

		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAbilityTask_WaitConfirmTarget::SetTarget(UTargetPointComponent* TargetPoint)
{
	if (TargetPoint == nullptr)
	{
		OnLocalCancelCallback();
		return;
	}
	
	if (TargetPoint != CurrentTargetPoint)
	{
		CurrentTargetPoint = TargetPoint;
		CurrentTarget = CurrentTargetPoint->GetOwner();
		CreateAndAttachTargetSelectedWidgetComponent(CurrentTargetPoint);
		
		GetWorld()->GetTimerManager().SetTimer(
		CheckTargetPointTimerHandle,
		this,
		&UAbilityTask_WaitConfirmTarget::CheckTargetPoint,
		CheckFrequency,
		true
		);
	}
}

UTargetPointComponent* UAbilityTask_WaitConfirmTarget::FindNearestTarget() const
{
	TArray<UTargetPointComponent*> TargetablePoints;
	GetTargetablePoints(TargetablePoints);

	if (TargetablePoints.IsEmpty())
	{
		return nullptr;
	}

	UTargetPointComponent* NearestTarget = nullptr;
	float ClosestDistance = TNumericLimits<float>::Max();
	FVector Origin = GetAvatarActor()->GetActorLocation();
	
	for (UTargetPointComponent* Target : TargetablePoints)
	{
		if (IsValid(Target))
		{
			const float Distance = (Origin - Target->GetComponentLocation()).Size();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				NearestTarget = Target;
			}
		}
	}
	return NearestTarget;
}

UTargetPointComponent* UAbilityTask_WaitConfirmTarget::FindNextTarget(UTargetPointComponent* OriginPoint, bool bSearchLeft) const
{
	TArray<UTargetPointComponent*> TargetablePoints;
	GetTargetablePoints(TargetablePoints);
	UTargetPointComponent* NewTarget = OriginPoint ? OriginPoint : static_cast<UTargetPointComponent*>(CurrentTargetPoint);

	if (IsValid(NewTarget))
	{
		const FVector ReferenceLocation = CameraComponent->GetComponentLocation();
		const FVector ReferenceActor = NewTarget->GetComponentLocation();
		FVector2D ReferenceVector = {ReferenceActor.X - ReferenceLocation.X , ReferenceActor.Y - ReferenceLocation.Y};
		ReferenceVector.Normalize();

		float RightComparison;
		float LeftComparison;
		UTargetPointComponent* RightTarget = nullptr;
		UTargetPointComponent* LeftTarget = nullptr;

		if(bSearchLeft)
		{
			// Cycle to the left
			RightComparison = 0.f;
			LeftComparison = -180.f;
		}
		else
		{
			// Cycle to the right
			RightComparison = 180.f;
			LeftComparison = 0.f;
		}

		for (UTargetPointComponent* Target : TargetablePoints)
		{
			if (!IsValid(Target))
			{
				continue;
			}
			
			// Gets the rotation from the source actor to the next potential targetable actor.
			const FVector TargetLocation = Target->GetComponentLocation();
			FVector2D ComparisonActorVector = {TargetLocation.X - ReferenceLocation.X , TargetLocation.Y - ReferenceLocation.Y};
			ComparisonActorVector.Normalize();

			const float ZRotation = FMath::RadiansToDegrees(FGenericPlatformMath::Acos(
				FVector2D::DotProduct(ReferenceVector, ComparisonActorVector))) *
					FMath::Sign(FVector2D::CrossProduct(ReferenceVector, ComparisonActorVector));

			//Get the target closest to the right and furthest from the left of the original target if we choose targets to the right
			if (!bSearchLeft)
			{
				if (ZRotation > 0 && ZRotation < RightComparison)
				{
					RightComparison = ZRotation;
					RightTarget = Target;
				}
				else if (ZRotation < 0 && ZRotation < LeftComparison)
				{
					LeftComparison = ZRotation;
					LeftTarget = Target;
				}
			}
			else
			{
				if (ZRotation > 0 && ZRotation > RightComparison)
				{
					RightComparison = ZRotation;
					RightTarget = Target;
				}
				else if (ZRotation < 0 && ZRotation > LeftComparison)
				{
					LeftComparison = ZRotation;
					LeftTarget = Target;
				}
			}
		}

		if (RightTarget || LeftTarget)
		{
			// Selects target to the right
			if (!bSearchLeft)
			{
				if (RightTarget)
				{
					NewTarget = RightTarget;
				}
				else if (LeftTarget)
				{
					NewTarget = LeftTarget;
				}
			}
			else
			{
				if (LeftTarget)
				{
					NewTarget = LeftTarget;
				}
				else if (RightTarget)
				{
					NewTarget = RightTarget;
				}
			}
		}
	}
	else
	{
		NewTarget = FindNearestTarget();
	}
	
	return NewTarget;
}

void UAbilityTask_WaitConfirmTarget::GetTargetablePoints(TArray<UTargetPointComponent*>& OutResults) const
{
	OutResults.Empty();
	
	if (TargetType == EAbilityTargetType::SelfOnly)
	{
		OutResults.Add(GetAvatarActor()->FindComponentByClass<UTargetPointComponent>());
		return;
	}
	
	FCollisionQueryParams Params(SCENE_QUERY_STAT(SphereOverlapComponenets), false);
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	TArray<FOverlapResult> Overlaps;
	GetWorld()->OverlapMultiByObjectType(
	   Overlaps,
	   GetAvatarActor()->GetActorLocation(),
	   FQuat::Identity,
	   ObjectParams,
	   FCollisionShape::MakeSphere(TargetingRange),
	   Params
   );
	
	for (FOverlapResult& Overlap : Overlaps)
	{
		if (Overlap.Component.IsValid())
		{
			if (UTargetPointComponent* C = Cast<UTargetPointComponent>(Overlap.Component.Get()))
			{
				if (TargetType == EAbilityTargetType::FriendlyExcludeSelf &&
					C->GetOwner() != GetAvatarActor() &&
					UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(GetAvatarActor(), C->GetOwner()) == ETeamAttitude::Friendly)
				{
					OutResults.Add(C);
					continue;
				}
				
				if (TargetType == EAbilityTargetType::Friendly &&
					UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(GetAvatarActor(), C->GetOwner()) == ETeamAttitude::Friendly)
				{
					OutResults.Add(C);
					continue;
				}
				
				if (TargetType == EAbilityTargetType::Hostile &&
					UCrysBlueprintFunctionLibrary::GetAttitudeTowardsActor(GetAvatarActor(), C->GetOwner()) == ETeamAttitude::Hostile)
				{
					OutResults.Add(C);
					continue;
				}
				
				if (TargetType == EAbilityTargetType::Any)
				{
					OutResults.Add(C);
					continue;
				}
			}
		}
	}
}

void UAbilityTask_WaitConfirmTarget::IA_CycleTarget(const FInputActionValue& Value)
{
	const bool bSearchLeft = Value.GetMagnitude() <= 0;
	SetTarget(FindNextTarget(CurrentTargetPoint, bSearchLeft));
}

void UAbilityTask_WaitConfirmTarget::OnReplicatedConfirmCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		AActor* TargetedActor = nullptr;
		const FGameplayAbilityTargetData* Data = DataHandle.Get(0);
		if (Data && Data->GetScriptStruct() == FGameplayAbilityTargetData_SingleTarget::StaticStruct())
		{
			const FGameplayAbilityTargetData_SingleTarget* CustomData = static_cast<const FGameplayAbilityTargetData_SingleTarget*>(Data);
			TargetedActor = CustomData->TargetedActor;
		}
		OnConfirm.Broadcast(TargetedActor);
	}
	EndTask();
}

void UAbilityTask_WaitConfirmTarget::OnReplicatedCancelCallback()
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancel.Broadcast(nullptr);
	}
	EndTask();
}

void UAbilityTask_WaitConfirmTarget::OnLocalConfirmCallback()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	FGameplayAbilityTargetData_SingleTarget* Data = new FGameplayAbilityTargetData_SingleTarget();
	Data->TargetedActor = CurrentTarget;
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);

	AbilitySystemComponent.Get()->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnConfirm.Broadcast(CurrentTarget);
	}
	EndTask();
}

void UAbilityTask_WaitConfirmTarget::OnLocalCancelCallback()
{
	AbilitySystemComponent->ServerSetReplicatedTargetDataCancelled(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		AbilitySystemComponent->ScopedPredictionKey
	);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnCancel.Broadcast(nullptr);
	}
	EndTask();
}

void UAbilityTask_WaitConfirmTarget::BindToInputActions()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	EnhancedInputSubsystem->AddTagToInputMode(Crys::NativeGameplayTag::EnhancedInput_Modes_AbilityTargeting);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
	EnhancedInputComponent->BindAction(
		UCrysAssetManager::GetAsset(GetDefault<UFinalFantasyXIGameData>()->InputActionConfirmAbilityTarget),
		ETriggerEvent::Triggered,
		this,
		&UAbilityTask_WaitConfirmTarget::OnLocalConfirmCallback);
	EnhancedInputComponent->BindAction(
		UCrysAssetManager::GetAsset(GetDefault<UFinalFantasyXIGameData>()->InputActionCancelAbilityTarget),
		ETriggerEvent::Triggered,
		this,
		&UAbilityTask_WaitConfirmTarget::OnLocalCancelCallback);
	EnhancedInputComponent->BindAction(
		UCrysAssetManager::GetAsset(GetDefault<UFinalFantasyXIGameData>()->InputActionCycleAbilityTarget),
		ETriggerEvent::Triggered,
		this,
		&UAbilityTask_WaitConfirmTarget::IA_CycleTarget);
}

void UAbilityTask_WaitConfirmTarget::RemoveInputActions()
{
	if (PlayerController)
	{
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
		EnhancedInputComponent->ClearBindingsForObject(this);
		
		UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		EnhancedInputSubsystem->RemoveTagFromInputMode(Crys::NativeGameplayTag::EnhancedInput_Modes_AbilityTargeting);
	}
}

void UAbilityTask_WaitConfirmTarget::CreateAndAttachTargetSelectedWidgetComponent(UTargetPointComponent* InTargetPoint)
{
	if (!TargetWidgetClass)
	{
		UE_LOG(LogCrysAbilitySystem, Error, TEXT("AbilityTask_WaitConfirmTarget: Cannot find a TargetWidgetClass, please ensure it is a valid reference in ability [%s}"), *GetNameSafe(Ability));
		return;
	}

	if (IsValid(TargetWidgetComponent))
	{
		TargetWidgetComponent->DestroyComponent();
	}
	
	if (IsValid(PlayerController) && PlayerController->IsLocalPlayerController() && IsValid(InTargetPoint))
	{
		TargetWidgetComponent = NewObject<UWidgetComponent>(InTargetPoint->GetOwner(), MakeUniqueObjectName(InTargetPoint->GetOwner(), UWidgetComponent::StaticClass(), FName("AbilityTarget")));
		TargetWidgetComponent->SetWidgetClass(TargetWidgetClass);
		TargetWidgetComponent->SetOwnerPlayer(PlayerController->GetLocalPlayer());
		TargetWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		TargetWidgetComponent->SetupAttachment(InTargetPoint);
		TargetWidgetComponent->SetDrawAtDesiredSize(true);
		TargetWidgetComponent->SetVisibility(true);
		TargetWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TargetWidgetComponent->RegisterComponent();
	}
}

void UAbilityTask_WaitConfirmTarget::SetupInitialTarget()
{
	UTargetPointComponent* StartingTargetPoint = nullptr;
	if (CurrentTarget)
	{
		StartingTargetPoint = CurrentTarget->FindComponentByClass<UTargetPointComponent>();
	}
	
	if (!StartingTargetPoint)
	{
		StartingTargetPoint = FindNearestTarget();
	}
	
	SetTarget(StartingTargetPoint);
}

void UAbilityTask_WaitConfirmTarget::CheckTargetPoint()
{
	if (ShouldBreakTargeting() && !bIsBreakingLineOfSight)
	{
		bIsBreakingLineOfSight = true;
		GetWorld()->GetTimerManager().SetTimer(
			BreakTargetPointTimerHandle,
			this,
			&UAbilityTask_WaitConfirmTarget::BreakTargeting,
			BreakTargetingDelay
		);
	}
}

bool UAbilityTask_WaitConfirmTarget::ShouldBreakTargeting() const
{
	if (!CurrentTargetPoint)
	{
		return true;
	}

	if (!CurrentTargetPoint->GetIsTargetable())
	{
		return true;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params = FCollisionQueryParams(FName("LineTraceSingle"));
	Params.AddIgnoredActor(GetAvatarActor());

	bool BlockedHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		GetAvatarActor()->GetActorLocation(),
		CurrentTargetPoint->GetComponentLocation(),
		ECC_Visibility,
		Params
	);

	if (BlockedHit)
	{
		return true;
	}

	if (GetDistanceToPoint(CurrentTargetPoint) > TargetingRange)
	{
		return true;
	}

	return false;
}

void UAbilityTask_WaitConfirmTarget::BreakTargeting()
{
	bIsBreakingLineOfSight = false;
	if (ShouldBreakTargeting())
	{
		SetTarget(nullptr);
	}
}

float UAbilityTask_WaitConfirmTarget::GetDistanceToPoint(const UTargetPointComponent* InTargetPoint) const
{
	if (IsValid(InTargetPoint))
	{
		return (GetAvatarActor()->GetActorLocation() - InTargetPoint->GetComponentLocation()).Size();	
	}
	return 0.f;
}
