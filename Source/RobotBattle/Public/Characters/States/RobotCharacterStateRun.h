// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateRun.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (RobotCharacterState), meta = (BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateRun : public URobotCharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* RunAnim;

	UPROPERTY(EditDefaultsOnly)
	float RunSpeedMax = 1000.0f;

	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;

	UFUNCTION()
	void OnInputJump();
};
