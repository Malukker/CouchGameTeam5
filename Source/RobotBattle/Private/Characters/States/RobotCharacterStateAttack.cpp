// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Animations/AnimNotify/EndAttackDetectionAnimNotify.h"
#include "Characters/Animations/AnimNotify/StartAttackDetectionAnimNotify.h"
#include "Characters/Interface/Robot.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"

ERobotCharacterStateID URobotCharacterStateAttack::GetStateID()
{
	return ERobotCharacterStateID::Attack;
}

void URobotCharacterStateAttack::StateEnter(ERobotCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);
	AttackAnim = CurrentAttackStruct.AnimMontage;
	if (AttackAnim)
	{
		InitAnimationNotify();
		float AnimDuration = Character->PlayAnimMontage(AttackAnim);
		KeyframeDeltaTime = AnimDuration / AttackAnim->GetNumberOfSampledKeys();
		CurrentTime = 0.0f;
		StartSocketName = CurrentAttackStruct.ConcernedBones[0];
		EndSocketName = CurrentAttackStruct.ConcernedBones[1];
	}
}

void URobotCharacterStateAttack::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void URobotCharacterStateAttack::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentTime += DeltaTime;

	if (bIsAttackTraceEnabled)
	{
		if (CurrentTime >= KeyframeDeltaTime)
		{
			const FVector StartPos =
				Character->GetMesh()->GetSocketByName(StartSocketName)->GetSocketLocation(Character->GetMesh());
			const FVector EndPos =
				Character->GetMesh()->GetSocketByName(EndSocketName)->GetSocketLocation(Character->GetMesh());
			const TArray<AActor*> ActorsToIgnore;
			FHitResult OutHit;

			if (UKismetSystemLibrary::SphereTraceSingle
				(
					GetWorld(),
					StartPos, EndPos, CurrentAttackStruct.TraceRadius,
					TraceTypeQuery1, false, ActorsToIgnore,
					EDrawDebugTrace::ForDuration,
					OutHit, true
				))
			{
				if (OutHit.GetActor())
				{
					if (OutHit.GetActor()->Implements<URobot>())
					{
						Cast<IRobot>(OutHit.GetActor())->TakeDamageFromAttack(CurrentAttackStruct.Damage);
					}
				}
			}
			CurrentTime -= KeyframeDeltaTime;
		}
	}
}

void URobotCharacterStateAttack::InitAnimationNotify()
{
	TArray<FAnimNotifyEvent> NotifyEvents = AttackAnim->Notifies;
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UStartAttackDetectionAnimNotify* StartNotify = Cast<UStartAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			StartNotify->OnNotifiedStartAttack.
			             AddUObject(this, &URobotCharacterStateAttack::StartDetectionNotifyAttack);
		}

		if (UEndAttackDetectionAnimNotify* EndNotify = Cast<UEndAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			EndNotify->OnNotifiedEndAttack.AddUObject(this, &URobotCharacterStateAttack::EndDetectionNotifyAttack);
		}
	}
}

void URobotCharacterStateAttack::StartDetectionNotifyAttack()
{
	//TODO
	//Lance les traces
	//La taille

	bIsAttackTraceEnabled = true;

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Red,
		TEXT("Start Detection Notify")
	);
}

void URobotCharacterStateAttack::EndDetectionNotifyAttack()
{
	//TODO

	bIsAttackTraceEnabled = false;
	CurrentTime = 0;
	StateMachine->ChangeState(ERobotCharacterStateID::Idle);

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("End Detection Notify")
	);
}
