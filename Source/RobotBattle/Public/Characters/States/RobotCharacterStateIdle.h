// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateIdle.generated.h"

enum class EAttackID : uint8;
/**
 * 
 */
UCLASS(ClassGroup = (RobotCharacterState), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateIdle : public URobotCharacterState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* IdleAnim;

	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnInputJump();

	UFUNCTION()
	void OnInputAttack();

	UFUNCTION()
	void OnInputDash();
	UFUNCTION()
	void OnStunEvent();
	UFUNCTION()
	void OnLockEvent();
};
