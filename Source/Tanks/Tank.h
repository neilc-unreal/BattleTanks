// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

class ATank;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTankDeathDelegate, ATank*, Tank);

UCLASS()
class TANKS_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "State")
	float GetHealth() { return CurrentHealth;  }

	UFUNCTION(BlueprintPure, Category = "State")
	float GetHealthPercentage() { return CurrentHealth / StartingHealth; }

	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	FTankDeathDelegate OnTankDeath;

private:

	UPROPERTY(EditAnywhere, Category = "Setup")
	float StartingHealth = 1000.0;

	UPROPERTY(EditAnywhere, Category = "Setup")
	float CurrentHealth = 0.0f;

};

