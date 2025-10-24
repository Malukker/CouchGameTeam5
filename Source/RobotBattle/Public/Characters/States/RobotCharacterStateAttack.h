// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "Characters/Attacks/AttackStruct.h"
#include "RobotCharacterStateAttack.generated.h"


UCLASS(ClassGroup = (RobotCharacterState), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateAttack : public URobotCharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FAttackStruct CurrentAttackStruct;

	UPROPERTY()
	UAnimMontage* AttackAnim;

	virtual ERobotCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	
	virtual void StateTick(float DeltaTime) override;

#pragma region Animation
	
private:
	float KeyframeDeltaTime;

	float CurrentTime;
	
	FName StartSocketName;

	FName EndSocketName;

	void InitAnimationNotify();
	
	void StartDetectionNotifyAttack();
	
	void EndDetectionNotifyAttack();
	
#pragma endregion

#pragma region Attack
	
	bool bIsAttackTraceEnabled;
	
#pragma endregion
};
