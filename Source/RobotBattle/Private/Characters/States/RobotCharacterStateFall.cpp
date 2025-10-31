// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateFall.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterSettings.h"
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
	CharacterMovement->Velocity.X= FallHorizontalMoveSpeed * Character->GetInputMoveX();

	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateFall::OnDashEvent);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateFall::OnStunEvent);
	Character->LockEvent.AddDynamic(this, &URobotCharacterStateFall::OnLockEvent);
}

void URobotCharacterStateFall::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);
	Character->InputDashEvent.RemoveDynamic(this, &URobotCharacterStateFall::OnDashEvent);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateFall::OnStunEvent);
	Character->LockEvent.RemoveDynamic(this, &URobotCharacterStateFall::OnLockEvent);
	CharacterMovement->GravityScale = 1;
	Character->ResetDash();
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
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}

	if (FMath::Abs(Character->GetInputMoveX()) > CharacterSettings->InputMoveXThreshold) WalkForwardTest();
}


void URobotCharacterStateFall::WalkForwardTest()
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

void URobotCharacterStateFall::OnDashEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}


void URobotCharacterStateFall::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}


void URobotCharacterStateFall::OnLockEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Lock);
}

