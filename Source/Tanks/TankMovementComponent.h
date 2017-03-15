// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/NavMovementComponent.h"
#include "TankMovementComponent.generated.h"

class UTankTrack;

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class TANKS_API UTankMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankTrack *Left, UTankTrack *Right);

	UFUNCTION(BlueprintCallable)
	void IntendMoveForward(float Throw);

	UFUNCTION(BlueprintCallable)
	void IntendRotateClockwise(float Throw);

private:
	// Called (indirectly) by pathfinding, from the AI controllers
	void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

	UTankTrack *LeftTrack = nullptr;
	UTankTrack *RightTrack = nullptr;
};
