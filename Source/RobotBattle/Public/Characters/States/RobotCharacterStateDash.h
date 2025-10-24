// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateDash.generated.h"


UCLASS(ClassGroup=(RobotCharacterState), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateDash : public URobotCharacterState
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly)
	float DashSpeed = 800.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float DashDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DashAnim;

	virtual ERobotCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	
	virtual void StateTick(float DeltaTime) override;

private:
	float CurrentDashTime;
	
	float OriginalFriction;

	float OriginalGravityScale;
};
