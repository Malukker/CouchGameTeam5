// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateDash.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"

ERobotCharacterStateID URobotCharacterStateDash::GetStateID()
{
	return ERobotCharacterStateID::Dash;
}

void URobotCharacterStateDash::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(DashAnim);
	CurrentDashTime = 0;
}

void URobotCharacterStateDash::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void URobotCharacterStateDash::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentDashTime += DeltaTime;

	if (CurrentDashTime >= DashDuration)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
	}

	Character->AddMovementInput()
}
