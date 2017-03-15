// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tanks.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class UTankAimingComponent;
class ATank;

/**
 * 
 */
UCLASS()
class TANKS_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
	
		
public:
	UFUNCTION(BlueprintCallable, Category = Setup)

	void BeginPlay() override;

	void Tick(float DeltaTime) override;
	void SetPawn(APawn* InPawn) override;

	UPROPERTY(EditAnywhere)
	float AimHorizontal = 0.5f;

	UPROPERTY(EditAnywhere)
	float AimVertical = 0.333f;

	UPROPERTY(EditAnywhere)
	float MaximumShotRange = 100000; // kms (measuredin cms I think)

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent *AimComponent);

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundTank(ATank *Tank);

private:
	//Move the barrel towards the crosshair
	bool AimTowardCrosshair();
	bool GetSiteRayHitLocation(FVector& HitLocation) const;
	
	UTankAimingComponent *GetAimingComponent();

	UFUNCTION()
	void OnTankDeath(ATank *DeadTank);

	bool IsAlive;
};
