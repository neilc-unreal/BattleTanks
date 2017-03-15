// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::Initialize(UTankBarrel *ABarrel, UTankTurret *ATurret)
{
	UE_LOG(LogTemp, Warning, TEXT("%s UTankAimingComponent::Initialize"), *GetOwner()->GetName());
	if (!ensure(ABarrel)) {
		UE_LOG(LogTemp, Error, TEXT("no barrel to aim with"));
	}
	if (!ensure(ATurret)) {
		UE_LOG(LogTemp, Error, TEXT("no turret to aim with"));
	}
	Barrel = ABarrel;
	Turret = ATurret;
}

void UTankAimingComponent::AimAt(FVector LocationInWorldToHit) {
	auto OurTankName = GetOwner()->GetName();

	if (ensure(Barrel)) {
		FVector ProjectileStart = Barrel->GetSocketLocation("FiringPoint");
		
		FVector TossVelocity;
		bool GotAimSolution = UGameplayStatics::SuggestProjectileVelocity(
			this,
			TossVelocity,
			ProjectileStart,
			LocationInWorldToHit,
			LaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace,
			FCollisionResponseParams::DefaultResponseParam,
			TArray<AActor*>(),
			false
		);
		if (GotAimSolution) {
			FVector AimDirection = TossVelocity.GetSafeNormal();
			MoveBarrelTowards(AimDirection);
			//UE_LOG(LogTemp, Warning, TEXT("got aim solution"));
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("no aim solution"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("no barrel to aim with"));

	}
}

void UTankAimingComponent::SetFiringState(EFiringStatus NewState)
{
	if (NewState != FiringState) {
		FiringState = NewState;
		//UE_LOG(LogTemp, Warning, TEXT("%s changed firing state to %d"), *GetOwner()->GetName(), (int)FiringState);
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (ensure(Barrel)) {
		FVector CurrentBarrelDirection = Barrel->GetForwardVector().GetSafeNormal();
		FVector TargetVector = TargetBarrelDirection.GetSafeNormal();
		bool IsEqual = (CurrentBarrelDirection - TargetVector).IsNearlyZero(0.01);
		return !IsEqual;
	}
	return false;
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (ensure(Barrel)) {
		if (ensure(Turret)) {
			// Find rotation absolute target, for the aimed direction
			FRotator CurrentBarrelRotation = Barrel->GetForwardVector().Rotation();
			FRotator Wanted = AimDirection.Rotation();
			TargetBarrelDirection = AimDirection;

			// Move the barrel towards that target direction
			FRotator Difference = (Wanted - CurrentBarrelRotation).GetNormalized();

			Barrel->Elevate(Difference.Pitch);

			float RotationAmount = Difference.Yaw;
			//UE_LOG(LogTemp, Warning, TEXT("BArrel Forward: %s. Wanted: %s, Rotation difference is %2.2f"), *CurrentBarrelRotation.ToString(), *Wanted.ToString(), Difference.Yaw);

			Turret->Rotate(RotationAmount);
		}
	}
}

bool UTankAimingComponent::IsReadyToFire()
{
	return FiringState == EFiringStatus::Locked || FiringState == EFiringStatus::Aiming;
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Ammo <= 0) {
		SetFiringState(EFiringStatus::OutOfAmmo);
	}
	else {
		if (IsReloaded())
		{
			if (IsBarrelMoving())
			{
				SetFiringState(EFiringStatus::Aiming);
			}
			else
			{
				SetFiringState(EFiringStatus::Locked);
			}
		}
		else
		{
			SetFiringState(EFiringStatus::Reloading);
		}
	}
}

void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();
	TargetBarrelDirection = FVector(0);
	LastFireTime = FPlatformTime::Seconds();
}

bool UTankAimingComponent::IsReloaded()
{
	return (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) {
		return;
	}

	if (Ammo <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Can't fire, no ammo left."));
		return;
	}

	if (FiringState == EFiringStatus::Reloading) {
		UE_LOG(LogTemp, Warning, TEXT("Can't fire, reloading..."));
		return;
	}

	FVector SpawnLocation = Barrel->GetSocketLocation("FiringPoint");
	FRotator SpawnRotation = Barrel->GetSocketRotation("FiringPoint");

	AProjectile *FiredProjectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint, SpawnLocation, SpawnRotation);
	if (ensure(FiredProjectile)) {
		FiredProjectile->Launch(LaunchSpeed);
		//UE_LOG(LogTemp, Warning, TEXT("Fire shot at %s"), *SpawnLocation.ToString());
		LastFireTime = FPlatformTime::Seconds();

		Ammo--;
	}
}

