// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateLoadingAttack.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Attacks/RobotCharacterAttacksData.h"


void URobotCharacterStateLoadingAttack::StateInit(URobotCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	URobotCharacterAttacksData* AttacksData = CharacterSettings->AttackData.LoadSynchronous();
	LoadingTime = AttacksData->LoadingAttack[Character->GetRobotBodyID()];
}

ERobotCharacterStateID URobotCharacterStateLoadingAttack::GetStateID()
{
	return ERobotCharacterStateID::LoadingAttack;
}

void URobotCharacterStateLoadingAttack::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(LoadingAnim);
	
}

void URobotCharacterStateLoadingAttack::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void URobotCharacterStateLoadingAttack::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	LoadingTime-=DeltaTime;
	if (LoadingTime<=0)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Attack);
	}
}

