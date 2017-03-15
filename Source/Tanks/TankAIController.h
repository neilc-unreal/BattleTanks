// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

UCLASS()
class TANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
	
public:
	void BeginPlay() override;
	void Tick(float DeltaTime) override;

private:
	APawn* GetPlayerPawn();

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnTankDeath(ATank *DeadTank);

	// How close can the AI tank get to the player (cms)
	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius = 3000;

	// There HAS to be a better way. This sucks.
	bool IsAlive;
};
