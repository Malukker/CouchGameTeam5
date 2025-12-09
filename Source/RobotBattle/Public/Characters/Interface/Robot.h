// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Robot.generated.h"

enum class EAttackID : uint8;
enum class ERobotID : uint8;
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
	virtual void TakeDamageFromAttack(int Damage, float StunTime) = 0;
	virtual void StartControllerVibration(ERobotID ERobotID,EAttackID EAttackID) = 0;

	virtual void KnockBackFromNotify(FVector2D Velocity) = 0;
	virtual FVector GetRobotLocation() = 0;
	virtual bool IsTargetFollowable() = 0;
 
};
