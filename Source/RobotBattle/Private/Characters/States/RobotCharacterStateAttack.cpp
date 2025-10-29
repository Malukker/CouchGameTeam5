// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Animations/AnimNotify/EndAttackDetectionAnimNotify.h"
#include "Characters/Animations/AnimNotify/StartAttackDetectionAnimNotify.h"
#include "Characters/Attacks/RobotCharacterAttacksData.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/KismetSystemLibrary.h"

void URobotCharacterStateAttack::StateInit(URobotCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
	ActorsToIgnore.Add(Character);
}

ERobotCharacterStateID URobotCharacterStateAttack::GetStateID()
{
	return ERobotCharacterStateID::Attack;
}

void URobotCharacterStateAttack::StateEnter(ERobotCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	URobotCharacterAttacksData* AttacksData = CharacterSettings->AttackData.LoadSynchronous();
	
	FAttackFromID ListAttacks = AttacksData->ListAttacks[Character->GetRobotBodyID()];
	CurrentAttackStruct = ListAttacks.AttacksFromID[Character->GetCurrentTypeAttack()];
	
	AttackAnim = CurrentAttackStruct.AnimMontage;
	InitAnimationNotify();
	AnimDuration = Character->PlayAnimMontage(AttackAnim);
	KeyframeDeltaTime = AnimDuration / AttackAnim->GetNumberOfSampledKeys();
	CurrentAnimDeltaTime = 0.0f;
	CurrentAnimTime = 0.0f;
	StartSocketName = CurrentAttackStruct.ConcernedBones[0];
	EndSocketName = CurrentAttackStruct.ConcernedBones[1];

	if (Character->GetCurrentTypeAttack() == EAttackID::Ultimate)
	{
		Character->ChargeManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}

void URobotCharacterStateAttack::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);

	TArray<FAnimNotifyEvent> NotifyEvents = AttackAnim->Notifies;
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UStartAttackDetectionAnimNotify* StartNotify = Cast<UStartAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			StartNotify->OnNotifiedStartAttack.Clear();
		}

		if (UEndAttackDetectionAnimNotify* EndNotify = Cast<UEndAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			EndNotify->OnNotifiedEndAttack.Clear();
		}
	}
	Character->LockManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down, false);
}

void URobotCharacterStateAttack::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentAnimDeltaTime += DeltaTime;
	CurrentAnimTime += DeltaTime;

	if (bIsAttackTraceEnabled)
	{
		if (CurrentAnimDeltaTime >= KeyframeDeltaTime)
		{
			StartPos = Character->GetMesh()->GetSocketByName(StartSocketName)->GetSocketLocation(Character->GetMesh());
			EndPos = Character->GetMesh()->GetSocketByName(EndSocketName)->GetSocketLocation(Character->GetMesh());
			FHitResult OutHit;
			ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3);
			if (UKismetSystemLibrary::SphereTraceSingle
				(
					GetWorld(),
					StartPos, EndPos, CurrentAttackStruct.TraceRadius,
					TraceTypeQuery, false, ActorsToIgnore,
					EDrawDebugTrace::ForDuration,
					OutHit, true
				))
			{
				if (OutHit.GetActor())
				{
					if (OutHit.GetActor()->Implements<URobot>())
					{
						Cast<IRobot>(OutHit.GetActor())->TakeDamageFromAttack(CurrentAttackStruct.Damage, CurrentAttackStruct.StunTime);
						bIsAttackTraceEnabled = false;
						Character->GuardResetManagerEvent.Broadcast();
					}
				}
			}
			CurrentAnimDeltaTime -= KeyframeDeltaTime;
		}
	}
	if (CurrentAnimTime >= AnimDuration)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
}

void URobotCharacterStateAttack::InitAnimationNotify()
{
	TArray<FAnimNotifyEvent> NotifyEvents = AttackAnim->Notifies;
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UStartAttackDetectionAnimNotify* StartNotify = Cast<UStartAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			StartNotify->OnNotifiedStartAttack. AddUObject(this, &URobotCharacterStateAttack::StartDetectionNotifyAttack);
		}

		if (UEndAttackDetectionAnimNotify* EndNotify = Cast<UEndAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			EndNotify->OnNotifiedEndAttack.AddUObject(this, &URobotCharacterStateAttack::EndDetectionNotifyAttack);
		}
	}
}

void URobotCharacterStateAttack::StartDetectionNotifyAttack()
{
	bIsAttackTraceEnabled = true;
	
	Character->LockManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down, true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Start Detection Notify"));
}

void URobotCharacterStateAttack::EndDetectionNotifyAttack()
{
	bIsAttackTraceEnabled = false;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("End Detection Notify"));
}
