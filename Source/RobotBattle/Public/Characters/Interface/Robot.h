// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Robot.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URobot : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROBOTBATTLE_API IRobot
{
	GENERATED_BODY()

public:
	virtual void TakeDamageFromAttack(int Damage, float StunTime,FVector2D KnockBackVelocity) = 0;
	
	virtual FVector GetRobotLocation() = 0;
 
};
