// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankAIController.h"
#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

APawn* ATankAIController::GetPlayerPawn() {
	auto playerController = Cast<ATankPlayerController>(GetWorld()->GetFirstPlayerController());
	if (ensure(playerController)) {
		return playerController->GetPawn();
	}
	return nullptr;
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) {
			return;
		}

		// Subscribe to the tanks death event
		PossessedTank->OnTankDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
	}
}

void ATankAIController::OnTankDeath(ATank *DeadTank)
{
	if (DeadTank) {
		UE_LOG(LogTemp, Warning, TEXT("Tank %s died!"), *DeadTank->GetName());
		IsAlive = false;

		DeadTank->DetachFromControllerPendingDestroy();
	}
}

void ATankAIController::BeginPlay() {
	Super::BeginPlay();
	IsAlive = true;
}

void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (!IsAlive) {
		return;
	}

	// Find the player to aim at
	auto PlayerPawn = GetPlayerPawn();
	if (PlayerPawn) {
		UTankAimingComponent *AimComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
		AimComponent->AimAt(PlayerPawn->GetActorLocation());

		// Fire if ready
		if (AimComponent->IsReadyToFire()) {
			AimComponent->Fire();
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("Can't fire, cos state is %d..."), (int)AimComponent->GetFiringState());
		}

		// Begin AI pathfinding to the player
		MoveToActor(PlayerPawn, AcceptanceRadius);
	}
}



