// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateJump.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (RobotCharacterState), meta = (BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateJump : public URobotCharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float JumpWalkSpeed = 400.0f;
	UPROPERTY(EditDefaultsOnly)
	float JumpMaxHeight = 280.0f;
	UPROPERTY(EditDefaultsOnly)
	float JumpDuration = 1.0f;
	UPROPERTY(EditDefaultsOnly)
	float JumpAirControl = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* JumpAnim;

	

	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;


private:
	UFUNCTION()
	void InitJumpAccordingToParameters();

	UFUNCTION()
	void OnDashEvent();
	UFUNCTION()
	void OnStunEvent();
	UFUNCTION()
	void OnLockEvent();
};
