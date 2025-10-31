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
	CharacterMovement->AirControl = JumpAirControl;
	float Gravity = -CharacterMovement->GetGravityZ();
	float GravityJump = (8*JumpMaxHeight)/(JumpDuration*JumpDuration);
	float VelocityJump = (GravityJump*JumpDuration)/2;
	CharacterMovement->JumpZVelocity = VelocityJump;
	CharacterMovement->GravityScale = GravityJump/Gravity;
	CharacterMovement->Velocity = FVector(JumpWalkSpeed * Character->GetInputMoveX(),CharacterMovement->Velocity.Y,CharacterMovement->Velocity.Z);
	Character->Jump();
}

void URobotCharacterStateJump::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);
	InitJumpAccordingToParameters();
	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateJump::OnDashEvent);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateJump::OnStunEvent);
	Character->LockEvent.AddDynamic(this, &URobotCharacterStateJump::OnLockEvent);
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
	Character->LockEvent.RemoveDynamic(this, &URobotCharacterStateJump::OnLockEvent);
}

void URobotCharacterStateJump::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (CharacterMovement->Velocity.Z < 0.f) {
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
	}
	else {
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) WalkForwardTest();
}


void URobotCharacterStateJump::WalkForwardTest()
{
	if (FMath::Sign(Character->GetOrientX()) != FMath::Sign(Character->GetInputMoveX()))
	{
		if (WalkForward)
		{
			Character->GuardManagerEvent.Broadcast(true);
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Guard"));
		}
		WalkForward = false;
	}
	else
	{
		if (!WalkForward)
		{
			Character->GuardManagerEvent.Broadcast(false);
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("No Guard"));
		}
		WalkForward = true;
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


void URobotCharacterStateJump::OnLockEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Lock);
}


