// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateFall.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateFall::GetStateID() {
	return ERobotCharacterStateID::Fall;
}

void URobotCharacterStateFall::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(FallAnim);

	CharacterMovement->MaxWalkSpeed = FallHorizontalMoveSpeed * FallAirControl;
	CharacterMovement->GravityScale = FallGravityScale;

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Idle")
	);*/
}

void URobotCharacterStateFall::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Idle")
	);*/
}

void URobotCharacterStateFall::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Idle")
	);*/

	if (CharacterMovement->IsMovingOnGround()) {
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
	else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

