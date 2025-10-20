// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateJump.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateJump::GetStateID() {
	return ERobotCharacterStateID::Jump;
}

void URobotCharacterStateJump::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(JumpAnim);

	CharacterMovement->MaxWalkSpeed = JumpWalkSpeed * JumpAirControl;
	CharacterMovement->JumpZVelocity = (2 * JumpMaxHeigh) / JumpDuration;
	CharacterMovement->GravityScale = 1;
	Character->Jump();

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