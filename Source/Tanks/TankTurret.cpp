// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankTurret.h"




void UTankTurret::Rotate(float RelativeSpeed)
{
	//UE_LOG(LogTemp, Warning, TEXT("Rotate turret %f"), RelativeSpeed);

	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, 1.0f);
	auto RotationChange = MaxDegreesPerSeconds * RelativeSpeed * GetWorld()->DeltaTimeSeconds;
	auto NewRotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, NewRotation, 0));
}
