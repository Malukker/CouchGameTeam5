// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateStun.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/States/RobotCharacterStateAttack.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


ERobotCharacterStateID URobotCharacterStateStun::GetStateID()
{
	return ERobotCharacterStateID::Stun;
}

void URobotCharacterStateStun::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	StunTimer = Character->GetStunTimer();
	AnimTimer = Character->PlayAnimMontage(StunMontage);
	if (AnimTimer<=0) AnimTimer=1.f;
	
	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateStun::OnInputJump);
	Character->InputAttackEvent.AddDynamic(this,&URobotCharacterStateStun::OnInputAttack);
	Character->InputDashEvent.AddDynamic(this,  &URobotCharacterStateStun::OnInputDash);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateStun::OnStunEvent);
}

void URobotCharacterStateStun::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateStun::OnInputJump);
	Character->InputAttackEvent.RemoveDynamic(this,&URobotCharacterStateStun::OnInputAttack);
	Character->InputDashEvent.RemoveDynamic(this,  &URobotCharacterStateStun::OnInputDash);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateStun::OnStunEvent);
}

void URobotCharacterStateStun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	StunTimer-=DeltaTime;
	AnimTimer-=DeltaTime;
	if (StunTimer<=0)
	{
		if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) {
			if (Character->GetPositionEnum() == ERobotCharacterPositionEnum::Down)
			{
				StateMachine->ChangeState(ERobotCharacterStateID::Walk);
			}
			else
			{
				StateMachine->ChangeState(ERobotCharacterStateID::Idle);
			}
		}
		if (CharacterMovement->Velocity.Z < 0.f) {
			StateMachine->ChangeState(ERobotCharacterStateID::Fall);
		}
	}
	if (AnimTimer<=0)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
}

void URobotCharacterStateStun::OnInputJump() {
	if (StunTimer>0)return;
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}

void URobotCharacterStateStun::OnInputAttack()
{
	if (StunTimer>0)return;
	if (Character->GetCurrentTypeAttack()==EAttackID::Ultimate)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::LoadingAttack);
		return;
	}
	StateMachine->ChangeState(ERobotCharacterStateID::Attack);
}

void URobotCharacterStateStun::OnInputDash()
{
	if (StunTimer>0)return;
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}

void URobotCharacterStateStun::OnStunEvent()
{
	if (StunTimer>0)return;
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}

