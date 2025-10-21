// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateAttack.generated.h"


UCLASS(ClassGroup = (RobotCharacterState), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateAttack : public URobotCharacterState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Attack1Anim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Attack2Anim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Attack3Anim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackDuoAnim;

	virtual ERobotCharacterStateID GetStateID() override;
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	virtual void StateTick(float DeltaTime) override;


	UFUNCTION()
	UAnimMontage* GetAnimMontageFromCharacterTypeAttack();

	
};
