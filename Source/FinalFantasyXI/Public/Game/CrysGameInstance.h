// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrysGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/**
	 * The GameMode uses this value to check which PlayerStart to go to.
	 * This can be from transitioning between maps or checkpoints upon death.
	 */
	UPROPERTY(BlueprintReadOnly)
	FName PlayerStartTag;
};
