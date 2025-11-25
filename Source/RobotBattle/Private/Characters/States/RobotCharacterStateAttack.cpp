// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"

#include "Camera/CameraShakeWorld.h"
#include "Characters/RobotCharacter.h"
#include "Characters/CollisionChannel.h"
#include "Characters/RobotCharacterSettings.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Animations/AnimNotify/EndAttackDetectionAnimNotify.h"
#include "Characters/Animations/AnimNotify/StartAttackDetectionAnimNotify.h"
#include "Characters/Attacks/RobotCharacterAttacksData.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

ERobotCharacterStateID URobotCharacterStateAttack::GetStateID()
{
	return ERobotCharacterStateID::Attack;
}

void URobotCharacterStateAttack::StateEnter(ERobotCharacterStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	HasTouch = false;
	
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
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateAttack::OnStunEvent);
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
	Character->LockManagerEvent.Broadcast(false);
	if (Character->GetCurrentTypeAttack() == EAttackID::Ultimate)
	{
		Character->ResetDamageBonus();
	}
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateAttack::OnStunEvent);
	if (Character->DoWantSwitch())
	{
		Character->EnergyManagerEvent.Broadcast();
	}

	if (HasTouch == false)
	{
		Character->AttackManagerEvent.Broadcast(false);
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
			if (Character->Team == 0) TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_AttackTraceOne);
			else if (Character->Team == 1) TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_AttackTraceTwo);
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
						Cast<IRobot>(OutHit.GetActor())->TakeDamageFromAttack(CurrentAttackStruct.Damage + Character->GetDamageBonus(), CurrentAttackStruct.StunTimer,CurrentAttackStruct.KnockBackVector);
						bIsAttackTraceEnabled = false;
						HasTouch = true;
						Character->HitStopEvent.Broadcast(CurrentAttackStruct.Damage + Character->GetDamageBonus());
						Character->AttackManagerEvent.Broadcast(true);
						Character->GuardResetManagerEvent.Broadcast();
						Character->LockManagerEvent.Broadcast(true);

						APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
						float ScaleShake = 1.f;
						float DurationShake = 0.f;
						UCameraShakeBase* TempShake =Camera->StartCameraShake(UCameraShakeWorld::StaticClass(),ScaleShake);
						UCameraShakeWorld* ShakeInstance = Cast<UCameraShakeWorld>(TempShake);
						ShakeInstance->SetupShakeParametersOnAttackID(Character->GetCurrentTypeAttack(),Character->GetRobotBodyID(),ScaleShake,DurationShake);
						//Delay for the shake
						FTimerHandle TimerHandle;
						GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Camera, ShakeInstance]()
						{
							Camera->StopCameraShake(ShakeInstance, false);
						}, DurationShake, false);
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
	Character->CustomTimeDilation = 1.f;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("End Detection Notify"));
}


void URobotCharacterStateAttack::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}