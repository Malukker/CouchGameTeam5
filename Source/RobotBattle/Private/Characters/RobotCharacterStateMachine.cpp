// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterState.h"
#include "Characters/RobotCharacterStateID.h"

void URobotCharacterStateMachine::Init(ARobotCharacter* InCharacter) {
	Character = InCharacter;

	FindStates();
	InitStates();

	ChangeState(ERobotCharacterStateID::Idle);
}

ARobotCharacter* URobotCharacterStateMachine::GetCharacter() const{
	return Character;
}

void URobotCharacterStateMachine::FindStates() {
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(URobotCharacterState::StaticClass());
	for (UActorComponent* StateComponent : FoundComponents) {
		URobotCharacterState* State = Cast<URobotCharacterState>(StateComponent);
		if (State == nullptr) continue;
		if (State->GetStateID() == ERobotCharacterStateID::None)continue;

		AllStates.Add(State);
	}
}

void URobotCharacterStateMachine::InitStates() {
	for (URobotCharacterState* State : AllStates) {
		State->StateInit(this);
	}
}

URobotCharacterState* URobotCharacterStateMachine::GetState(ERobotCharacterStateID StateID) {
	for (URobotCharacterState* State : AllStates) {
		if (StateID == State->GetStateID()) {
			return State;
		}
	}
	return nullptr;
}

void URobotCharacterStateMachine::ChangeState(ERobotCharacterStateID NextStateID) {
	URobotCharacterState* NextState = GetState(NextStateID);
	//Do Nothing if NextState not found
	if (NextState == nullptr) return;

	if (CurrentState != nullptr) {
		CurrentState->StateExit(NextStateID);
	}

	ERobotCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if (CurrentState != nullptr) {
		CurrentState->StateEnter(PreviousStateID);
	}
}

void URobotCharacterStateMachine::Tick(float DeltaTime) {
	if (CurrentState == nullptr) return;
	CurrentState->StateTick(DeltaTime);
}