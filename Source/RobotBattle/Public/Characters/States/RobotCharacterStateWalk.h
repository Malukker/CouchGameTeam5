// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateWalk.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (RobotCharacterState), meta = (BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateWalk : public URobotCharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* WalkAnimForward;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* WalkAnimBackward;

	UPROPERTY(EditDefaultsOnly)
	float ForwardWalkSpeedMax = 250.0f;
	
	UPROPERTY(EditDefaultsOnly)
	float BackwardWalkSpeedMax = 150.0f;

	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;

private:
	UPROPERTY()
	bool WalkForward;
	
	UFUNCTION()
	void OnInputJump();

	UFUNCTION()
	void OnInputDash();
	UFUNCTION()
	void OnStunEvent();
};
