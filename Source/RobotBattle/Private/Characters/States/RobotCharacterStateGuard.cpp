// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateGuard.h"




ERobotCharacterStateID URobotCharacterStateGuard::GetStateID()
{
	return ERobotCharacterStateID::Guard;
}

void URobotCharacterStateGuard::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
}

void URobotCharacterStateGuard::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void URobotCharacterStateGuard::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}
