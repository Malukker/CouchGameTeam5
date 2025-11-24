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
	
	virtual ERobotCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	
	virtual void StateTick(float DeltaTime) override;

#pragma region Attack
	
private:
	UPROPERTY()
	FAttackStruct CurrentAttackStruct;
	
	bool bIsAttackTraceEnabled;

	int AttackIndex;
	
	FVector StartPos;
	
	FVector EndPos;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY()
	UAnimMontage* AttackAnim;
	
	float KeyframeDeltaTime;
	
	float AnimDuration;

	float CurrentAnimDeltaTime;
	
	float CurrentAnimTime;

	void InitAnimationNotify();

	UFUNCTION()
	void StartDetectionNotifyAttack(AActor* ConcernedActor);
	
	UFUNCTION()
	void EndDetectionNotifyAttack(AActor* ConcernedActor);
	
	
#pragma endregion

	UFUNCTION()
	void OnStunEvent();

	
};
