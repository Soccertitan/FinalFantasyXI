// Copyright 2025 Soccertitan

#pragma once

#include "CoreMinimal.h"
#include "CrimAbilitySystemInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "CrysSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API UCrysSkeletalMeshComponent : public USkeletalMeshComponent, public ICrimAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	virtual void InitializeWithAbilitySystem_Implementation(UCrimAbilitySystemComponent* NewAbilitySystemComponent) override;
};
