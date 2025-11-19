// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacterState.h"
#include "RobotCharacterStateLoadingAttack.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ROBOTBATTLE_API URobotCharacterStateLoadingAttack : public URobotCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URobotCharacterStateLoadingAttack();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	virtual void StateInit(URobotCharacterStateMachine* InStateMachine) override;
	
	virtual ERobotCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID) override;
	
	virtual void StateExit(ERobotCharacterStateID NextState) override;
	
	virtual void StateTick(float DeltaTime) override;
};
