// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateWalk.h"

#include <filesystem>

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/RobotCharacterSettings.h"


ERobotCharacterStateID URobotCharacterStateWalk::GetStateID()
{
	return ERobotCharacterStateID::Walk;
}

void URobotCharacterStateWalk::StateEnter(ERobotCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	if (Character->IsWalkingForward())
	{
		CharacterMovement->MaxWalkSpeed = ForwardWalkSpeedMax;
		WalkForward = true;
		Character->GuardManagerEvent.Broadcast(false);
		Character->PlayAnimMontage(WalkAnimForward);
	}
	else
	{
		CharacterMovement->MaxWalkSpeed = BackwardWalkSpeedMax;
		WalkForward = false;
		Character->GuardManagerEvent.Broadcast(true);
		Character->PlayAnimMontage(WalkAnimBackward);
	}

	Character->InputJumpEvent.AddDynamic(this, &URobotCharacterStateWalk::OnInputJump);
	Character->InputDashEvent.AddDynamic(this, &URobotCharacterStateWalk::OnInputDash);
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateWalk::OnStunEvent);
}

void URobotCharacterStateWalk::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	
	Character->InputJumpEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnInputJump);
	Character->InputDashEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnInputDash);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateWalk::OnStunEvent);
}

void URobotCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->InputMoveXThreshold)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
	else
	{
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
		if (Character->IsWalkingForward())
		{
			if (!WalkForward)
			{
				CharacterMovement->MaxWalkSpeed = ForwardWalkSpeedMax;
				Character->GuardManagerEvent.Broadcast(false);
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("No Guard"));
				Character->PlayAnimMontage(WalkAnimForward);
			}
			WalkForward = true;
		}
		else
		{
			if (WalkForward)
			{
				CharacterMovement->MaxWalkSpeed = BackwardWalkSpeedMax;
				Character->GuardManagerEvent.Broadcast(true);
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Guard"));
				Character->PlayAnimMontage(WalkAnimBackward);
			}
			WalkForward = false;
		}
	}
}

void URobotCharacterStateWalk::OnInputJump()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Jump);
}

void URobotCharacterStateWalk::OnInputDash()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Dash);
}

void URobotCharacterStateWalk::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}
