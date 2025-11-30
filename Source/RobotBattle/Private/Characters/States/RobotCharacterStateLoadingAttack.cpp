// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateLoadingAttack.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateMachine.h"


ERobotCharacterStateID URobotCharacterStateLoadingAttack::GetStateID()
{
	return ERobotCharacterStateID::LoadingAttack;
}

void URobotCharacterStateLoadingAttack::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(LoadingAnim);
	Character->ResetDamageBonus();

	Character->ChargeManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	
	LoadingTime = LoadingTimeDefault;
}

void URobotCharacterStateLoadingAttack::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	Character->AttackDuoManagerEvent.Broadcast(ERobotCharacterPositionEnum::None);
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

