// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "CrysGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API ACrysGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ACrysGameMode();

	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = L"") override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void Logout(AController* Exiting) override;
};
