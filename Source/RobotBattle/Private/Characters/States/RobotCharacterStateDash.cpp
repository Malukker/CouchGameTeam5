// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateDash.h"

#include "MathUtil.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateDash::GetStateID()
{
	return ERobotCharacterStateID::Dash;
}

void URobotCharacterStateDash::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	CurrentDashTime = 0;
	if (Character->GetRobotCharacterDownChargeID()==ERobotCharacterDownChargeID::Dash)
	{
		Character->ChargeManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down);
	}
	if (FMathf::Sign(Character->GetOrientX()) != FMathf::Sign(Character->GetDashDirectionX()))
	{
		Character->InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down);
		Character->PlayAnimMontage(DashAnimBackward);
	}
	else
	{
		Character->PlayAnimMontage(DashAnimForward);
	}
	
	OriginalFriction = CharacterMovement->GroundFriction;
	OriginalGravityScale = CharacterMovement->GravityScale;

	CharacterMovement->GroundFriction = 0;
	CharacterMovement->GravityScale = 0;
	
	FVector Dash;
	if (Character->DoHaveEnergy())
	{
		Dash = FVector::ForwardVector * DashSpeed * Character->GetDashDirectionX();
	}
	else
	{
		Dash = FVector::ForwardVector * DashSpeed * Character->GetDashDirectionX() * Character->GetNerfStatsMultiplier();
	}
	Character->LaunchCharacter(Dash, true, true);
	Character->UseDash();
}

void URobotCharacterStateDash::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	CharacterMovement->GroundFriction = OriginalFriction;
	CharacterMovement->GravityScale = OriginalGravityScale;
	
	if (CharacterMovement->IsMovingOnGround()) {
		Character->ResetDash();
	}
	if (FMathf::Sign(Character->GetOrientX()) != FMathf::Sign(Character->GetDashDirectionX()))
	{
		Character->InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down);
	}

	CharacterMovement->StopMovementImmediately();
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateDash::OnStunEvent);
	
	if (Character->DoWantSwitch())
	{
		Character->EnergyManagerEvent.Broadcast();
	}
}

void URobotCharacterStateDash::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentDashTime += DeltaTime;

	if (CurrentDashTime >= DashDuration)
	{
		if (CharacterMovement->IsMovingOnGround()) {
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


void URobotCharacterStateDash::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}


void URobotCharacterStateDash::OnLockEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Lock);
}

