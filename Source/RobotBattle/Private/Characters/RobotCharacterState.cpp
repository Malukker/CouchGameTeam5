// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacterState.h"
#include "Characters/RobotCharacterStateID.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/RobotCharacter.h"

// Sets default values for this component's properties
URobotCharacterState::URobotCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


ERobotCharacterStateID URobotCharacterState::GetStateID() {
	return ERobotCharacterStateID::None;
}

void URobotCharacterState::StateInit(URobotCharacterStateMachine* InStateMachine) {
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();
	CharacterSettings = GetDefault<URobotCharacterSettings>();
	CharacterMovement = Character->GetCharacterMovement();

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Magenta,
		FString::Printf(TEXT("Init State %d"), GetStateID())
	);
}

void URobotCharacterState::StateEnter(ERobotCharacterStateID PreviousState) {

}

void URobotCharacterState::StateExit(ERobotCharacterStateID NextState) {

}

void URobotCharacterState::StateTick(float DeltaTime) {

}