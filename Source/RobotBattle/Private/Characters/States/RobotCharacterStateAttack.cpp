// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"


ERobotCharacterStateID URobotCharacterStateAttack::GetStateID() {
	return ERobotCharacterStateID::Attack;
}

void URobotCharacterStateAttack::StateEnter(ERobotCharacterStateID PreviousState) {
	Super::StateEnter(PreviousState);

	Character->PlayAnimMontage(GetAnimMontageFromCharacterTypeAttack());

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter State Idle")
	);*/
}

void URobotCharacterStateAttack::StateExit(ERobotCharacterStateID NextState) {
	Super::StateExit(NextState);

	
	/*GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit State Idle")
	);*/
}

void URobotCharacterStateAttack::StateTick(float DeltaTime) {
	Super::StateTick(DeltaTime);

	/*GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Idle")
	);*/

	
}

UAnimMontage* URobotCharacterStateAttack::GetAnimMontageFromCharacterTypeAttack()
{
	switch (Character->GetCurrentTypeAttack())
	{
	case 0 :
		return Attack1Anim;
	case 1 :
		return Attack2Anim;
	case 2 :
		return Attack3Anim;
	case 3 :
		return AttackDuoAnim;

		default:
		return nullptr;
	}
}

