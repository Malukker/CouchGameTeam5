// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateGuard.generated.h"


UCLASS(ClassGroup=(RobotCharacterState), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateGuard : public URobotCharacterState
{
	GENERATED_BODY()

	virtual ERobotCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	
	virtual void StateTick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UAnimMontage* GuardAnimMontage;

	UFUNCTION()
	void OnStunEvent();
};
