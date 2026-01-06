// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "InputActionTagMap.h"
#include "EnhancedInputComponent.h"
#include "CrysEnhancedInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UCrysEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:

	/**
	 * Binds all the inputs in InputConfig to the Triggered event.
	 * The passed in functions must have a GameplayTag as a parameter.
	 */
	template<class UserClass, typename TriggeredFuncType>
	void BindInputActions(const UInputActionTagMap* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc);
};

template<class UserClass, typename TriggeredFuncType>
void UCrysEnhancedInputComponent::BindInputActions(const UInputActionTagMap* InputConfig, UserClass* Object, TriggeredFuncType TriggeredFunc)
{
	if (!InputConfig)
	{
		return;
	}

	for (const FInputMap& Input : InputConfig->Inputs)
	{
		if (Input.Action && Input.Tag.IsValid())
		{
			if (TriggeredFunc)
			{
				BindAction(Input.Action, ETriggerEvent::Triggered, Object, TriggeredFunc, Input.Tag);
			}
		}
	}
}
