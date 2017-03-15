// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankBarrel.h"
#include "UnrealMathUtility.h"



void UTankBarrel::Elevate(float RelativeSpeed)

{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0f, 1.0f);

	auto ElevationChange = MaxDegreesPerSeconds * RelativeSpeed * GetWorld()->DeltaTimeSeconds;
	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	RawNewElevation = FMath::Clamp<float>(RawNewElevation, MinimumElevationInDegrees, MaximumElevationInDegrees);

	//UE_LOG(LogTemp, Warning, TEXT("%2.2f Elevate at %f per second"), GetWorld()->GetTimeSeconds(), RelativeSpeed);

	SetRelativeRotation(FRotator(RawNewElevation, 0, 0));
}
