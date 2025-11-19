// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateLoadingAttack.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateLoadingAttack : public URobotCharacterState
{
	GENERATED_BODY()


	

private:


	virtual void StateInit(URobotCharacterStateMachine* InStateMachine) override;
	
	virtual ERobotCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	
	virtual void StateTick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* LoadingAnim;

	float LoadingTime = 2.f;
};
