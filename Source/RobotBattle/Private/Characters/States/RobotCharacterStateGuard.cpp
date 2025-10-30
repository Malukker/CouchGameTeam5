// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateGuard.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"


ERobotCharacterStateID URobotCharacterStateGuard::GetStateID()
{
	return ERobotCharacterStateID::Guard;
}

void URobotCharacterStateGuard::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
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
	Super::StateTick(DeltaTime);
	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold) {
			StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
}
