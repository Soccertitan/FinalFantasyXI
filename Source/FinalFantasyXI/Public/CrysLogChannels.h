// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"

#include "Logging/LogMacros.h"

class UObject;

FINALFANTASYXI_API DECLARE_LOG_CATEGORY_EXTERN(LogCrys, Log, All);
FINALFANTASYXI_API DECLARE_LOG_CATEGORY_EXTERN(LogCrysAbilitySystem, Log, All);

FINALFANTASYXI_API FString GetClientServerContextString(UObject* ContextObject = nullptr);