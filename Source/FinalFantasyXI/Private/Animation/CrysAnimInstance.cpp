// Copyright Soccertitan 2025


#include "Animation/CrysAnimInstance.h"

#include "AbilitySystemGlobals.h"
#include "CrimAbilitySystemBlueprintFunctionLibrary.h"
#include "CrimAbilitySystemComponent.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

UCrysAnimInstance::UCrysAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCrysAnimInstance::InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent)
{
	check(NewAbilitySystemComponent);
	GameplayTagPropertyMap.Initialize(this, NewAbilitySystemComponent);
	GameplayTagPropertyMap.ApplyCurrentTags();
}

#if WITH_EDITOR
EDataValidationResult UCrysAnimInstance::IsDataValid(class FDataValidationContext& Context) const
{
	Super::IsDataValid(Context);

	GameplayTagPropertyMap.IsDataValid(this, Context);

	return ((Context.GetNumErrors() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif


void UCrysAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	if (AActor* OwningActor = GetOwningActor())
	{
		if (UCrimAbilitySystemComponent* ASC = UCrimAbilitySystemBlueprintFunctionLibrary::GetAbilitySystemComponent(OwningActor))
		{
			Execute_InitializeWithAbilitySystem(this, ASC);
		}
	}
}
