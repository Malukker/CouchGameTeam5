// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateLock.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"


ERobotCharacterStateID URobotCharacterStateLock::GetStateID()
{
	return ERobotCharacterStateID::Lock;
}

void URobotCharacterStateLock::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Character->LockEvent.AddDynamic(this, &URobotCharacterStateLock::OnEventLock);
	Super::StateEnter(PreviousStateID);
}

void URobotCharacterStateLock::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	Character->LockEvent.RemoveDynamic(this, &URobotCharacterStateLock::OnEventLock);
	
}

void URobotCharacterStateLock::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void URobotCharacterStateLock::OnEventLock()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	
}
