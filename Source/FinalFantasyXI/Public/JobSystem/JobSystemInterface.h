// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JobSystemInterface.generated.h"

class UJobManagerComponent;

// This class does not need to be modified.
UINTERFACE()
class UJobSystemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FINALFANTASYXI_API IJobSystemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent)
	UJobManagerComponent* GetJobManagerComponent() const;
};
