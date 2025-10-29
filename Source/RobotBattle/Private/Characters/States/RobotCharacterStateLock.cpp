// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateLock.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


ERobotCharacterStateID URobotCharacterStateLock::GetStateID()
{
	return ERobotCharacterStateID::Lock;
}

void URobotCharacterStateLock::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	if (Character->GetPositionEnum() == ERobotCharacterPositionEnum::Down)
	{
		OriginalGravityScale = CharacterMovement->GravityScale;
		CharacterMovement->GravityScale = 0;
		CharacterMovement->StopMovementImmediately();
	}
	
	Character->UnlockEvent.AddDynamic(this, &URobotCharacterStateLock::OnEventUnlock);
}


void URobotCharacterStateLock::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);

	if (Character->GetPositionEnum() == ERobotCharacterPositionEnum::Down)
	{
		CharacterMovement->GravityScale = OriginalGravityScale;
	}
	
	Character->UnlockEvent.RemoveDynamic(this, &URobotCharacterStateLock::OnEventUnlock);
	
}

void URobotCharacterStateLock::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void URobotCharacterStateLock::OnEventUnlock()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	
}
