// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankMovementComponent.h"
#include "TankTrack.h"



void UTankMovementComponent::Initialize(UTankTrack *Left, UTankTrack *Right)
{
	LeftTrack = Left;
	RightTrack = Right;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	//UE_LOG(LogTemp, Warning, TEXT("Move forward by %f"), Throw);
	if (ensure(LeftTrack && RightTrack)) {
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(Throw);
	}
}

void UTankMovementComponent::IntendRotateClockwise(float Throw)
{
	//UE_LOG(LogTemp, Warning, TEXT("Rotate clockwise by %f"), Throw);
	if (ensure(LeftTrack && RightTrack)) {
		LeftTrack->SetThrottle(Throw);
		RightTrack->SetThrottle(-Throw);
	}
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) {
	//UE_LOG(LogTemp, Warning, TEXT("%s move with velocity %s, max speed %d"), *GetOwner()->GetName(), *MoveVelocity.ToString(), bForceMaxSpeed);
	auto AIDirectionIntention = MoveVelocity.GetSafeNormal();
	auto TankForward = GetOwner()->GetActorForwardVector().GetSafeNormal();

	auto HowMuchToDrive = FVector::DotProduct(TankForward, AIDirectionIntention);
	IntendMoveForward(HowMuchToDrive);

	auto HowMuchToTurn = FVector::CrossProduct(TankForward, AIDirectionIntention);
	IntendRotateClockwise(HowMuchToTurn.Z);
}

