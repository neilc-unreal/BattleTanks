// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	IsAlive = true;

	UTankAimingComponent *AimComponent = GetAimingComponent();
	if (ensure(AimComponent)) {
		FoundAimingComponent(AimComponent);
	}

	ATank *Tank = Cast<ATank>(GetPawn());
	if (Tank) {
		FoundTank(Tank);
	}
}

void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (IsAlive) {
		AimTowardCrosshair();
	}
}

void ATankPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn) {
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) {
			return;
		}

		// Subscribe to the tanks death event
		PossessedTank->OnTankDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}

void ATankPlayerController::OnTankDeath(ATank *DeadTank)
{
	if (DeadTank) {
		UE_LOG(LogTemp, Warning, TEXT("We %s died!"), *DeadTank->GetName());
		IsAlive = false;

		StartSpectatingOnly();
	}
}

bool ATankPlayerController::AimTowardCrosshair()
{
	// If no controller tank - abort!
	if (!GetAimingComponent()) {
		return false;
	}

	FVector HitLocation;
	if (GetSiteRayHitLocation(HitLocation)) {
		GetAimingComponent()->AimAt(HitLocation);
	}
	return false;
}

bool ATankPlayerController::GetSiteRayHitLocation(FVector& HitLocation) const {	 
	FVector CameraWorldLocation;
	FVector LookDirection;
	int32 ViewportX, ViewportY;
	GetViewportSize(ViewportX, ViewportY);

	FVector2D ScreenLoc = FVector2D(ViewportX * AimHorizontal, ViewportY * AimVertical);
	if (DeprojectScreenPositionToWorld(ScreenLoc.X, ScreenLoc.Y, CameraWorldLocation, LookDirection)) {
		// Raycast from the camera view, through the dot, and see what we hit

		const FVector Start = PlayerCameraManager->GetCameraLocation();
		const FVector End = Start + (LookDirection * MaximumShotRange);

		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5);
		FHitResult HitResult;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Camera)) {
			HitLocation = HitResult.ImpactPoint;
			return true;
		}
		else {
			HitLocation = FVector(0);
		}
	}

	return false;
}

UTankAimingComponent * ATankPlayerController::GetAimingComponent()
{
	auto OurTank = GetPawn();
	if (OurTank) {
		return OurTank->FindComponentByClass<UTankAimingComponent>();
	}
	return nullptr;
}

