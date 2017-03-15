// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "TankProjectileMovement.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TANKS_API UTankProjectileMovement : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:
	UTankProjectileMovement();
	
	
};
