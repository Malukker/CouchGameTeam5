// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateIdle.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/States/RobotCharacterStateAttack.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateIdle::GetStateID() {
	return ERobotCharacterStateID::Idle;
}

void URobotCharacterStateIdle::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(IdleAnim);

	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateIdle::OnInputJump);
	Character->InputAttackEvent.AddDynamic(this,&URobotCharacterStateIdle::OnInputAttack);
	Character->InputDashEvent.AddDynamic(this,  &URobotCharacterStateIdle::OnInputDash);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateIdle::OnStunEvent);
	Character->LockEvent.AddDynamic(this, &URobotCharacterStateIdle::OnLockEvent);
}

void URobotCharacterStateIdle::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnInputJump);
	Character->InputAttackEvent.RemoveDynamic(this,&URobotCharacterStateIdle::OnInputAttack);
	Character->InputDashEvent.RemoveDynamic(this,  &URobotCharacterStateIdle::OnInputDash);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnStunEvent);
	Character->LockEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnLockEvent);
}

void URobotCharacterStateIdle::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) {
		if (Character->GetPositionEnum() == ERobotCharacterPositionEnum::Down)
		{
			StateMachine->ChangeState(ERobotCharacterStateID::Walk);
		}
		else if (FMath::Sign(Character->GetOrientX()) != FMath::Sign(Character->GetInputMoveX()))
		{
			StateMachine->ChangeState(ERobotCharacterStateID::Guard);
		}
	}
	if (CharacterMovement->Velocity.Z < 0.f) {
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
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

void URobotCharacterStateIdle::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}


void URobotCharacterStateIdle::OnLockEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Lock);
}

