// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateDash.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateDash::GetStateID()
{
	return ERobotCharacterStateID::Dash;
}

void URobotCharacterStateDash::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(DashAnim);
	CurrentDashTime = 0;
	if (Character->GetRobotCharacterDownChargeID()==ERobotCharacterDownChargeID::Dash)
	{
		Character->ChargeIncrementEvent.Broadcast();
	}
	OriginalFriction = CharacterMovement->GroundFriction;
	OriginalGravityScale = CharacterMovement->GravityScale;

	CharacterMovement->GroundFriction = 0;
	CharacterMovement->GravityScale = 0;
	
	const FVector Dash = FVector::ForwardVector * DashSpeed * Character->GetDashDirectionX();
	Character->LaunchCharacter(Dash, true, true);
	Character->UseDash();
}

void URobotCharacterStateDash::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	CharacterMovement->GroundFriction = OriginalFriction;
	CharacterMovement->GravityScale = OriginalGravityScale;

	CharacterMovement->StopMovementImmediately();
}

void URobotCharacterStateDash::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentDashTime += DeltaTime;

	if (CurrentDashTime >= DashDuration)
	{
		if (CharacterMovement->IsMovingOnGround()) {
			Character->ResetDash();
			if (Character->GetInputMoveX() > 0)
			{
				StateMachine->ChangeState(ERobotCharacterStateID::Walk);
			}
			else
			{
				StateMachine->ChangeState(ERobotCharacterStateID::Idle);
			}
		}
		else
		{
			StateMachine->ChangeState(ERobotCharacterStateID::Fall);
		}
	}
}
