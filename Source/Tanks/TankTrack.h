// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))//, hidecategories = ("Collision"))
class TANKS_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UTankTrack();

	void ApplySidewaysForce();

	void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit);

	// This is max force per track in Newtons
	UPROPERTY(EditAnywhere)
	float MaximumDrivingForce = 200000.0;

	// This is max speed the tank can achieve in km/ph
	UPROPERTY(EditAnywhere)
	float MaximumSpeed = 55;

	void DriveTrack();
	float CurrentThrottle = 0.0f;
};
