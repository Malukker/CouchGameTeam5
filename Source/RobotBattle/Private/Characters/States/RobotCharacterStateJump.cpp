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
	CharacterMovement->Velocity = FVector(JumpWalkSpeed * Character->GetOrientX(),CharacterMovement->Velocity.Y,CharacterMovement->Velocity.Z);
	Character->Jump();
}

void URobotCharacterStateJump::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);
	InitJumpAccordingToParameters();

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Idle")
	);*/
}

void URobotCharacterStateJump::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Idle")
	);*/
}

void URobotCharacterStateJump::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Idle")
	);*/

	if (CharacterMovement->Velocity.Z < 0.f) {
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
	}
	else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}


