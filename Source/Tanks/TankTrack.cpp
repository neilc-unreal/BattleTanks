// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankTrack.h"




UTankTrack::UTankTrack()
{
}


void UTankTrack::ApplySidewaysForce()
{
	// Calculate slippage speed
	auto TankRightVector = GetRightVector();
	auto TankSpeed = GetComponentVelocity();
	auto SplippageSpeed = FVector::DotProduct(TankRightVector, TankSpeed);

	// Work out required acceleration this frame to correct (Two tracks?)
	// Because A=speed * time
	auto DeltaTime = GetWorld()->GetDeltaSeconds(); 
	auto CorrectionAcceleration = -SplippageSpeed / DeltaTime * GetRightVector();

	// Apply F
	auto Tank = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	auto CorrectionForce = Tank->GetMass() * CorrectionAcceleration / 2; // two tracks!
	Tank->AddForce(CorrectionForce);
}

void UTankTrack::BeginPlay()
{
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::OnHit(UPrimitiveComponent *HitComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, FVector NormalImpulse, const FHitResult &Hit) 
{
	// Don't apply force, when we're off the ground.
	// Otherwise we "fly" which sucks.
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0.0f;
	//UE_LOG(LogTemp, Warning, TEXT("Driving track"));
}

void UTankTrack::DriveTrack()
{
	auto ForceApplied = GetForwardVector() * CurrentThrottle * MaximumDrivingForce;
	auto ForceLocation = GetComponentLocation();

	// Apply the force to the Tank physics body
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}

void UTankTrack::SetThrottle(float throttle)
{
	// UE_LOG(LogTemp, Warning, TEXT("Track throttle set to %f"), throttle);
	CurrentThrottle = CurrentThrottle + throttle;
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle, -1, 1);
}

