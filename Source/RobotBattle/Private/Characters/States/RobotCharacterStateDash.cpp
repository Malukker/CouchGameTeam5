// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateDash.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"

ERobotCharacterStateID URobotCharacterStateDash::GetStateID()
{
	return ERobotCharacterStateID::Dash;
}

void URobotCharacterStateDash::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(DashAnim);
	CurrentDashTime = 0;
	DashDirectionX = Character->GetDashDirectionX();

	OriginalSpeed = CharacterMovement->MaxWalkSpeed;
	OriginalFriction = CharacterMovement->GroundFriction;
	OriginalGravityScale = CharacterMovement->GravityScale;

	CharacterMovement->MaxWalkSpeed = DashSpeed;
	CharacterMovement->GroundFriction = 0.0f;
	CharacterMovement->GravityScale = 0.0f;
}

void URobotCharacterStateDash::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);

	CharacterMovement->GroundFriction = OriginalFriction;
	CharacterMovement->GravityScale = OriginalGravityScale;
}

void URobotCharacterStateDash::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentDashTime += DeltaTime;

	if (CurrentDashTime >= DashDuration)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Fall);
	}

	Character->AddMovementInput(FVector::ForwardVector, DashDirectionX);
}
