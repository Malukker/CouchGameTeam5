// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateWalk.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/RobotCharacterSettings.h"


ERobotCharacterStateID URobotCharacterStateWalk::GetStateID() {
	return ERobotCharacterStateID::Walk;
}

void URobotCharacterStateWalk::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	CharacterMovement->MaxWalkSpeed = WalkSpeedMax;
	Character->PlayAnimMontage(WalkAnim);

	Character->InputMoveXFastEvent.AddDynamic(this, &URobotCharacterStateWalk::OnInputMoveXFast);
	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateWalk::OnInputJump);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Walk")
	);*/
}

void URobotCharacterStateWalk::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	Character->InputMoveXFastEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnInputMoveXFast);
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnInputJump);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Walk")
	);*/
}

void URobotCharacterStateWalk::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Walk")
	);*/

	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold) {
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
	else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

void URobotCharacterStateWalk::OnInputMoveXFast(float InputMoveX) {
	StateMachine->ChangeState(ERobotCharacterStateID::Run);
}

void URobotCharacterStateWalk::OnInputJump() {
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}