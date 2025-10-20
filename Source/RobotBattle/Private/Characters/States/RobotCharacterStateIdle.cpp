// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateIdle.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"

ERobotCharacterStateID URobotCharacterStateIdle::GetStateID() {
	return ERobotCharacterStateID::Idle;
}

void URobotCharacterStateIdle::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(IdleAnim);

	Character->InputMoveXFastEvent.AddDynamic(this, &URobotCharacterStateIdle::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateIdle::OnInputJump);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Idle")
	);*/
}

void URobotCharacterStateIdle::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	Character->InputMoveXFastEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnInputMoveXFast);
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateIdle::OnInputJump);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Idle")
	);*/
}

void URobotCharacterStateIdle::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Idle")
	);*/

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) {
		StateMachine->ChangeState(ERobotCharacterStateID::Walk);
	}
}

void URobotCharacterStateIdle::OnInputMoveXFast(float InputMoveX) {
	StateMachine->ChangeState(ERobotCharacterStateID::Run);
}

void URobotCharacterStateIdle::OnInputJump() {
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}