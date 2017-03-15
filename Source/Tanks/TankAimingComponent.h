// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class UTankBarrel;
class UTankTurret;
class AProjectile;

UENUM()
enum class EFiringStatus : uint8 {
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKS_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = "Setup")
	void Initialize(UTankBarrel *ABarrel, UTankTurret *ATurret);

	void AimAt(FVector Location);

	UFUNCTION(BlueprintCallable)
	void Fire();

	EFiringStatus GetFiringState() const { return FiringState; }
	bool IsReadyToFire();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void BeginPlay() override;

	bool IsReloaded();

protected:

	UPROPERTY(EditAnywhere, Category = "Setup")
	TSubclassOf<AProjectile> ProjectileBlueprint;

	UPROPERTY(EditAnywhere, Category = "Firing")
	float LaunchSpeed = 80000;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
	float ReloadTimeInSeconds = 3;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	EFiringStatus FiringState = EFiringStatus::Reloading;

	UFUNCTION(BlueprintCallable, Category = "State")
	int32 GetAmmoRemaining() { return Ammo; }

	void SetFiringState(EFiringStatus NewState);



private:	
	UTankBarrel *Barrel = nullptr;
	UTankTurret *Turret = nullptr;
	
	bool IsBarrelMoving();
	void MoveBarrelTowards(FVector AimDirection);

	double LastFireTime = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 Ammo = 5;

	FVector TargetBarrelDirection;
};
