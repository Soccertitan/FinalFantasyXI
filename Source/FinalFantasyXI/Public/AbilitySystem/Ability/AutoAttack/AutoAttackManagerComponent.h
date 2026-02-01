// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoAttackManagerComponent.generated.h"

/**
 * Manages the timer for auto attacks. Notifies when the timer starts/ends. And updates timers based on specified 
 * conditions. Like equipping a new weapon.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINALFANTASYXI_API UAutoAttackManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAutoAttackManagerComponent();

protected:
	virtual void BeginPlay() override;

};
