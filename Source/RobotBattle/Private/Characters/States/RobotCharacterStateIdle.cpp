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
	if (Character->IsWalkingForward())
	{
		WalkForward = true;
		Character->PlayAnimMontage(IdleAnim);
		Character->GuardManagerEvent.Broadcast(false);
	}
	else
	{
		WalkForward = false;
		Character->PlayAnimMontage(GuardAnimMontage);
		Character->GuardManagerEvent.Broadcast(true);
	}

	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateIdle::OnInputJump);
	Character->InputAttackEvent.AddDynamic(this,&URobotCharacterStateIdle::OnInputAttack);
	Character->InputDashEvent.AddDynamic(this,  &URobotCharacterStateIdle::OnInputDash);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateIdle::OnStunEvent);
}

void URobotCharacterStateIdle::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnInputJump);
	Character->InputAttackEvent.RemoveDynamic(this,&URobotCharacterStateIdle::OnInputAttack);
	Character->InputDashEvent.RemoveDynamic(this,  &URobotCharacterStateIdle::OnInputDash);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnStunEvent);
}

void URobotCharacterStateIdle::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) {
		if (Character->GetPositionEnum() == ERobotCharacterPositionEnum::Down && Character->DoHaveEnergy())
		{
			StateMachine->ChangeState(ERobotCharacterStateID::Walk);
		}
		else if (Character->IsWalkingForward())
		{
			if (!WalkForward)
			{
				Character->PlayAnimMontage(IdleAnim);
				Character->GuardManagerEvent.Broadcast(false);
			}
			WalkForward = true;
		}
		else
		{
			if (WalkForward)
			{
				Character->PlayAnimMontage(GuardAnimMontage);
				Character->GuardManagerEvent.Broadcast(true);
			}
			WalkForward = false;
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
	if (Character->GetCurrentTypeAttack()==EAttackID::Ultimate)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::LoadingAttack);
		return;
	}
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

