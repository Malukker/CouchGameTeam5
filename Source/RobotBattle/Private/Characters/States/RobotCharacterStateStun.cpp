// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateStun.h"

#include "Characters/States/RobotCharacterStateAttack.h"


ERobotCharacterStateID URobotCharacterStateStun::GetStateID()
{
	return ERobotCharacterStateID::Stun;
}

void URobotCharacterStateStun::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
}

void URobotCharacterStateStun::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void URobotCharacterStateStun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void URobotCharacterStateStun::DisableInput()
{
	
}



