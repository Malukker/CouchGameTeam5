// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"

#include "Camera/CameraShakeWorld.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Animations/AnimNotify/EndAttackDetectionAnimNotify.h"
#include "Characters/Animations/AnimNotify/StartAttackDetectionAnimNotify.h"
#include "Characters/Attacks/RobotCharacterAttacksData.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Camera/CameraShakeWorld.h"

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
	AttackIndex = 0;

	if (Character->GetCurrentTypeAttack() == EAttackID::Ultimate)
	{
		Character->ResetDamageBonus();
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
			StartNotify->OnNotifiedStartAttack.RemoveDynamic(this, &URobotCharacterStateAttack::StartDetectionNotifyAttack);
		}

		if (UEndAttackDetectionAnimNotify* EndNotify = Cast<UEndAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			EndNotify->OnNotifiedEndAttack.RemoveDynamic(this, &URobotCharacterStateAttack::EndDetectionNotifyAttack);
		}
	}
	Character->LockManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down, false);
	if (Character->GetCurrentTypeAttack() == EAttackID::Ultimate)
	{
		Character->ResetDamageBonus();
		Character->AttackDuoManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
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
			CurrentAnimDeltaTime -= KeyframeDeltaTime;
			StartPos = Character->GetMesh()->GetSocketByName(CurrentAttackStruct.ConcernedBones[AttackIndex])->GetSocketLocation(Character->GetMesh());
			EndPos = Character->GetMesh()->GetSocketByName(CurrentAttackStruct.ConcernedBones[AttackIndex + 1])->GetSocketLocation(Character->GetMesh());
			FHitResult OutHit;
			ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Pawn);
			if (Character->Team == 0) TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel3);
			else if (Character->Team == 1) TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel4);
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
						Cast<IRobot>(OutHit.GetActor())->TakeDamageFromAttack(CurrentAttackStruct.Damage + Character->GetDamageBonus(), CurrentAttackStruct.StunTime);
						bIsAttackTraceEnabled = false;
						
						UCameraShakeWorld* ShakeInstance = NewObject<UCameraShakeWorld>();
						if (ShakeInstance)
						{
							float ScaleShake = 1.f;
							ShakeInstance->SetupShakeParametersOnAttackID(Character->GetCurrentTypeAttack(),Character->GetRobotCharacterUpID(),ScaleShake);
							UGameplayStatics::PlayWorldCameraShake(GetWorld(),ShakeInstance->GetClass(),OutHit.GetActor()->GetActorLocation(),0.f,1000.f,ScaleShake);
						}
					
						Character->GuardResetManagerEvent.Broadcast();
						Character->LockManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down, true);
					}
				}
			}
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
			StartNotify->OnNotifiedStartAttack.AddDynamic(this, &URobotCharacterStateAttack::StartDetectionNotifyAttack);
		}

		if (UEndAttackDetectionAnimNotify* EndNotify = Cast<UEndAttackDetectionAnimNotify>(NotifyEvent.Notify))
		{
			EndNotify->OnNotifiedEndAttack.AddDynamic(this, &URobotCharacterStateAttack::EndDetectionNotifyAttack);
		}
	}
}

void URobotCharacterStateAttack::StartDetectionNotifyAttack(AActor* ConcernedActor)
{
	if (ConcernedActor != GetOwner()) return;
	bIsAttackTraceEnabled = true;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Start Detection Notify"));
}

void URobotCharacterStateAttack::EndDetectionNotifyAttack(AActor* ConcernedActor)
{
	if (ConcernedActor != GetOwner()) return;
	bIsAttackTraceEnabled = false;
	AttackIndex += 2;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("End Detection Notify"));
}
