// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateGuard.h"

#include "MathUtil.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"


ERobotCharacterStateID URobotCharacterStateGuard::GetStateID()
{
	return ERobotCharacterStateID::Guard;
}

void URobotCharacterStateGuard::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Character->PlayAnimMontage(GuardAnimMontage);
	Character->GuardManagerEvent.Broadcast(true);
}

void URobotCharacterStateGuard::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	Character->GuardManagerEvent.Broadcast(false);
}

void URobotCharacterStateGuard::StateTick(float DeltaTime)
{
	if (FMathf::Sign(Character->GetOrientX()) == FMathf::Sign(Character->GetInputMoveX()))
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
}
