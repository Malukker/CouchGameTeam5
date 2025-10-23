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

	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateJump::OnInputDash);
}

void URobotCharacterStateJump::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	Character->InputDashEvent.RemoveDynamic(this, &URobotCharacterStateJump::OnInputDash);
}

void URobotCharacterStateJump::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (CharacterMovement->Velocity.Z < 0.f) {
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
	}
	else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

void URobotCharacterStateJump::OnInputDash()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}