// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateFall.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateFall::GetStateID() {
	return ERobotCharacterStateID::Fall;
}

void URobotCharacterStateFall::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(FallAnim);

	CharacterMovement->AirControl = FallAirControl;
	CharacterMovement->GravityScale = FallGravityScale;
	CharacterMovement->Velocity.X= FallHorizontalMoveSpeed * Character->GetOrientX();

	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateFall::OnDashEvent);
}

void URobotCharacterStateFall::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);
	CharacterMovement->GravityScale = 1;
	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Idle")
	);*/
}

void URobotCharacterStateFall::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	if (CharacterMovement->IsMovingOnGround()) {
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
		
	}
	else {
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
}

void URobotCharacterStateFall::OnDashEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}

