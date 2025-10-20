// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateRun.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/RobotCharacterSettings.h"


ERobotCharacterStateID URobotCharacterStateRun::GetStateID() {
	return ERobotCharacterStateID::Run;
}

void URobotCharacterStateRun::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	CharacterMovement->MaxWalkSpeed = RunSpeedMax;
	Character->PlayAnimMontage(RunAnim);

	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateRun::OnInputJump);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Run")
	);*/
}

void URobotCharacterStateRun::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateRun::OnInputJump);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Run")
	);*/
}

void URobotCharacterStateRun::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Run")
	);*/

	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold) {
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
	else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

void URobotCharacterStateRun::OnInputJump() {
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}