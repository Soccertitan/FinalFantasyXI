// Copyright Soccertitan 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/GameState.h"
#include "CrysGameState.generated.h"

struct FAbilityVerbMessage;
class UCrimAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class FINALFANTASYXI_API ACrysGameState : public AGameState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	// The ability system component subobject for game-wide things (primarily gameplay cues)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCrimAbilitySystemComponent> AbilitySystemComponent;

public:
	ACrysGameState();

	virtual void PostInitializeComponents() override;

	//~IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~End of IAbilitySystemInterface

	// Send a message that all clients will (probably) get
	// (use only for client notifications like eliminations, server join messages, etc... that can handle being lost)
	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "GameState")
	void MulticastMessageToClients(const FAbilityVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "GameState")
	void MulticastReliableMessageToClients(const FAbilityVerbMessage Message);
};
