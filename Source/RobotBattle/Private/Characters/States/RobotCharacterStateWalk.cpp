// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateWalk.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/RobotCharacterSettings.h"


ERobotCharacterStateID URobotCharacterStateWalk::GetStateID()
{
	return ERobotCharacterStateID::Walk;
}

void URobotCharacterStateWalk::StateEnter(ERobotCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	CharacterMovement->MaxWalkSpeed = WalkSpeedMax;
	Character->PlayAnimMontage(WalkAnim);

	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateWalk::OnInputJump);
	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateWalk::OnInputDash);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateWalk::OnStunEvent);
	Character->LockEvent.AddDynamic(this, &URobotCharacterStateWalk::OnLockEvent);
	
	Character->DoGuardTest();
}

void URobotCharacterStateWalk::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnInputJump);
	Character->InputDashEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnInputDash);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnStunEvent);
	Character->LockEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnLockEvent);
}

void URobotCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
	else
	{
		Character->DoGuardTest();
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}
}

void URobotCharacterStateWalk::OnInputJump()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}

void URobotCharacterStateWalk::OnInputDash()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}

void URobotCharacterStateWalk::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}


void URobotCharacterStateWalk::OnLockEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Lock);
}


