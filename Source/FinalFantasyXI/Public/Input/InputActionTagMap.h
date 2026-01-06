// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "InputActionTagMap.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FInputMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (Categories = "Input"))
	FGameplayTag Tag = FGameplayTag();
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> Action = nullptr;
};

/**
 * Maps a GameplayTag to an InputAction.
 */
UCLASS()
class FINALFANTASYXI_API UInputActionTagMap : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	const UInputAction* GetInputAction(const FGameplayTag& InputTag, bool bLogNotFound = false) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (NoResetToDefault, TitleProperty=Tag))
	TArray<FInputMap> Inputs;
};
