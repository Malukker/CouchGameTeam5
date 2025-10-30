// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateStun.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/States/RobotCharacterStateAttack.h"
#include "Kismet/GameplayStatics.h"


ERobotCharacterStateID URobotCharacterStateStun::GetStateID()
{
	return ERobotCharacterStateID::Stun;
}

void URobotCharacterStateStun::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
	Character->PlayAnimMontage(StunMontage);
	Timer = Character->GetStunTimer();
	TimeOn = true;
}

void URobotCharacterStateStun::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	TimeOn = false;
	
}

void URobotCharacterStateStun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	if (TimeOn)
	{
		Timer-=DeltaTime;
		if (Timer<=0)
		{
			StateMachine->ChangeState(ERobotCharacterStateID::Idle);
		}
		
	}
}



