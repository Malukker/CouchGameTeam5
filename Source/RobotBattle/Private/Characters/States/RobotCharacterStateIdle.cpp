// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateIdle.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/States/RobotCharacterStateAttack.h"

ERobotCharacterStateID URobotCharacterStateIdle::GetStateID() {
	return ERobotCharacterStateID::Idle;
}

void URobotCharacterStateIdle::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(IdleAnim);

	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateIdle::OnInputJump);
	Character->InputAttackEvent.AddDynamic(this,&URobotCharacterStateIdle::OnInputAttack);
	Character->InputDashEvent.AddDynamic(this,  &URobotCharacterStateIdle::OnInputDash);
}

void URobotCharacterStateIdle::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnInputJump);
	Character->InputAttackEvent.RemoveDynamic(this,&URobotCharacterStateIdle::OnInputAttack);
	Character->InputDashEvent.RemoveDynamic(this,  &URobotCharacterStateIdle::OnInputDash);
}

void URobotCharacterStateIdle::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) {
		StateMachine->ChangeState(ERobotCharacterStateID::Walk);
	}
}

void URobotCharacterStateIdle::OnInputJump() {
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}

void URobotCharacterStateIdle::OnInputAttack()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Attack);
}

void URobotCharacterStateIdle::OnInputDash()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}
