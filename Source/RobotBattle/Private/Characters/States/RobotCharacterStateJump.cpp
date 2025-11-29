// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateJump.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateJump::GetStateID() {
	return ERobotCharacterStateID::Jump;
}

void URobotCharacterStateJump::InitJumpAccordingToParameters()
{
	if (!CharacterMovement)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::Red,TEXT("CHARACTER COMPONENT NULL"));
		return;
	}
	Character->PlayAnimMontage(JumpAnim);
	
	float Gravity = -CharacterMovement->GetGravityZ();
	float GravityJump;
	if (Character->DoHaveEnergy()) GravityJump = (8*JumpMaxHeight)/(JumpDuration*JumpDuration);
	else GravityJump = (8 * (JumpMaxHeight * Character->GetNerfStatsMultiplier()))/(JumpDuration*JumpDuration);
	float VelocityJump = (GravityJump*JumpDuration)/2;
	CharacterMovement->JumpZVelocity = VelocityJump;
	CharacterMovement->GravityScale = GravityJump/Gravity;
	if (Character->DoHaveEnergy())
	{
		CharacterMovement->AirControl = JumpAirControl;
		CharacterMovement->Velocity = FVector(
			JumpWalkSpeed * Character->GetInputMoveX(),
			CharacterMovement->Velocity.Y,
			CharacterMovement->Velocity.Z);
	}
	else
	{
		CharacterMovement->AirControl = JumpAirControl * Character->GetNerfStatsMultiplier();
		CharacterMovement->Velocity = FVector(
			JumpWalkSpeed * Character->GetNerfStatsMultiplier() * Character->GetInputMoveX(),
			CharacterMovement->Velocity.Y,
			CharacterMovement->Velocity.Z);
	}
	Character->Jump();
}

void URobotCharacterStateJump::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);
	InitJumpAccordingToParameters();
	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateJump::OnDashEvent);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateJump::OnStunEvent);
	Character->EnergyEvent.AddDynamic(this, &URobotCharacterStateJump::OnEnergyEvent);
	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Idle")
	);*/
}

void URobotCharacterStateJump::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	Character->InputDashEvent.RemoveDynamic(this, &URobotCharacterStateJump::OnDashEvent);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateJump::OnStunEvent);
	Character->EnergyEvent.RemoveDynamic(this, &URobotCharacterStateJump::OnEnergyEvent);
}

void URobotCharacterStateJump::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (CharacterMovement->Velocity.Z < 0.f) {
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
	}
	else
	{
		if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold)
		{
			Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
			if (Character->IsWalkingForward())
			{
				if (!WalkForward)
				{
					Character->GuardManagerEvent.Broadcast(false);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("No Guard"));
				}
				WalkForward = true;
			}
			else
			{
				if (WalkForward)
				{
					Character->GuardManagerEvent.Broadcast(true);
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Guard"));
				}
				WalkForward = false;
			}
		}
	}
}


void URobotCharacterStateJump::OnDashEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}

void URobotCharacterStateJump::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}

void URobotCharacterStateJump::OnEnergyEvent()
{
	if (Character->DoHaveEnergy())
	{
		CharacterMovement->AirControl = JumpAirControl;
	}
	else
	{
		CharacterMovement->AirControl = JumpAirControl * Character->GetNerfStatsMultiplier();
	}
}