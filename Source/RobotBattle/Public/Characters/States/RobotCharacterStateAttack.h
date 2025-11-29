// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "Characters/Attacks/AttackStruct.h"
#include "RobotCharacterStateAttack.generated.h"


class IRobot;

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
	
	UPROPERTY(EditAnywhere)
	TMap<EAttackID,UAnimMontage*> Attacks = {};
	
	UPROPERTY(EditAnywhere)
	float AttackDuoBonusMultiplier = 0;

	UPROPERTY()
	bool HasTouch = false;
	UPROPERTY()
	bool bIsAttackTraceEnabled = false;

	UPROPERTY()
	float AnimDuration = 0;
	UPROPERTY()
	float CurrentAnimTime = 0;

	UPROPERTY()
	FVector StartPos;

	UPROPERTY()
	FVector EndPos;

	UPROPERTY()
	TArray<AActor*> ActorsToIgnore;
	
	UFUNCTION()
	void DetectionNotifyAttack(AActor* ConcernedActor, FAttackStruct Data);
	
	UFUNCTION()
	void StartDetectionNotifyAttack(AActor* ConcernedActor);

	UFUNCTION()
	void KnockBackNotify(AActor* ConcernedActor, FVector2D KnockBack);

	UPROPERTY()
	TScriptInterface<IRobot> TouchedCharacterInterface;
	
#pragma endregion

	UFUNCTION()
	void OnStunEvent();

	
};
