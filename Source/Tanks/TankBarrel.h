// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
// Barrels to make things 
UCLASS(meta = (BlueprintSpawnableComponent))//, hidecategories = ("Collision"))
class TANKS_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void Elevate(float RelativeSpeed);
	
private:
	UPROPERTY(EditAnywhere, Category = Setup)
		float MaxDegreesPerSeconds = 5;	
	
	UPROPERTY(EditAnywhere, Category = Setup)
		float MaximumElevationInDegrees = 30;

	UPROPERTY(EditAnywhere, Category = Setup)
		float MinimumElevationInDegrees = 0;
};
