// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateStun.generated.h"


UCLASS(ClassGroup = (RobotCharacterState), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateStun : public URobotCharacterState
{
	GENERATED_BODY()
	
	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;

private:

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* StunMontage;
	UPROPERTY()
	float StunTimer;
	UPROPERTY()
	float AnimTimer;

	UFUNCTION()
	void OnInputJump();
	UFUNCTION()
	void OnInputAttack();
	UFUNCTION()
	void OnInputDash();
	UFUNCTION()
	void OnStunEvent();
};
