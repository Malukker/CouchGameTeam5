// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateFall.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (RobotCharacterState), meta = (BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateFall : public URobotCharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float FallHorizontalMoveSpeed = 400.0f;
	UPROPERTY(EditDefaultsOnly)
	float FallAirControl = 1.0f;
	UPROPERTY(EditDefaultsOnly)
	float FallGravityScale = 3.0f;
	

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* FallAnim;

	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;

private:
	bool WalkForward;
	
	UFUNCTION()
	void OnDashEvent();
	UFUNCTION()
	void OnStunEvent();
	UFUNCTION()
	void OnLockEvent();
};
