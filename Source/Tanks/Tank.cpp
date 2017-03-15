// Fill out your copyright notice in the Description page of Project Settings.

#include "Tanks.h"
#include "Tank.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CurrentHealth = StartingHealth;
}

void ATank::BeginPlay() {
	Super::BeginPlay();
}

float ATank::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = DamageAmount;

	// TODO: What's the angle between the incoming damage and the face that was hit?	
	// TODO: Damage falloff based on distance and hit angle

	CurrentHealth = FMath::Max<float>(CurrentHealth - ActualDamage, 0.0f);
	if (CurrentHealth <= 0) {
		OnTankDeath.Broadcast(this);
	}

	//UE_LOG(LogTemp, Warning, TEXT("Tank %s taking %f damage"), *GetName(), DamageAmount);
	return DamageAmount;
}
