// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"
#include "Camera/CameraShakeWorld.h"
#include "Characters/RobotCharacter.h"
#include "Characters/CollisionChannel.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Animations/AnimNotify/KnockBackAnimNotify.h"
#include "Characters/Animations/AnimNotify/AttackDetectionAnimNotify.h"
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

	Character->SetGuard(false);
	HasTouch = false;
	
	AnimDuration = Character->PlayAnimMontage(Attacks[Character->GetCurrentTypeAttack()]);
	CurrentAnimTime = 0.0f;
	Character->HurtEvent.AddDynamic(this, &URobotCharacterStateAttack::OnStunEvent);
	Character->BoostEvent.AddDynamic(this, &URobotCharacterStateAttack::OnBoostEvent);

	if (Character->GetBoost()) UseBoost = true;
	else UseBoost = false;
	
	TArray<FAnimNotifyEvent> NotifyEvents = Attacks[Character->GetCurrentTypeAttack()]->Notifies;
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UAttackDetectionAnimNotify* AttackNotify = Cast<UAttackDetectionAnimNotify>(NotifyEvent.NotifyStateClass))
		{
			AttackNotify->OnNotifiedAttackStart.AddDynamic(this, &URobotCharacterStateAttack::StartDetectionNotifyAttack);
			AttackNotify->OnNotifiedAttack.AddDynamic(this, &URobotCharacterStateAttack::DetectionNotifyAttack);
		}

		if (UKnockBackAnimNotify* KnockBackAnimNotify = Cast<UKnockBackAnimNotify>(NotifyEvent.Notify))
		{
			KnockBackAnimNotify->OnKnockBackEvent.AddDynamic(this,&URobotCharacterStateAttack::KnockBackNotify);
		}
	}
}

void URobotCharacterStateAttack::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
	Character->HurtEvent.RemoveDynamic(this, &URobotCharacterStateAttack::OnStunEvent);
	Character->BoostEvent.RemoveDynamic(this, &URobotCharacterStateAttack::OnBoostEvent);
	UseBoost = false;

	TArray<FAnimNotifyEvent> NotifyEvents = Attacks[Character->GetCurrentTypeAttack()]->Notifies;
	for (FAnimNotifyEvent NotifyEvent : NotifyEvents)
	{
		if (UAttackDetectionAnimNotify* StartNotify = Cast<UAttackDetectionAnimNotify>(NotifyEvent.NotifyStateClass))
		{
			StartNotify->OnNotifiedAttackStart.RemoveDynamic(this, &URobotCharacterStateAttack::StartDetectionNotifyAttack);
			StartNotify->OnNotifiedAttack.RemoveDynamic(this, &URobotCharacterStateAttack::DetectionNotifyAttack);
		}

		if (UKnockBackAnimNotify* KnockBackAnimNotify = Cast<UKnockBackAnimNotify>(NotifyEvent.Notify))
		{
			KnockBackAnimNotify->OnKnockBackEvent.RemoveDynamic(this,&URobotCharacterStateAttack::KnockBackNotify);
		}
	}
	Character->ResetAttackDuoBonus();
}

void URobotCharacterStateAttack::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	CurrentAnimTime += DeltaTime;
	if (CurrentAnimTime >= AnimDuration)
	{
		StateMachine->ChangeState(ERobotCharacterStateID::Idle);
	}
}

void URobotCharacterStateAttack::StartDetectionNotifyAttack(AActor* ConcernedActor)
{
	if (ConcernedActor != GetOwner()) return;
	bIsAttackTraceEnabled = true;
}

void URobotCharacterStateAttack::DetectionNotifyAttack(AActor* ConcernedActor, FAttackStruct Data)
{
	if (ConcernedActor != GetOwner()) return;
	
	if (bIsAttackTraceEnabled)
	{
		StartPos = Character->GetMesh()->GetSocketByName(Data.StartSocket)->GetSocketLocation(Character->GetMesh());
		EndPos = Character->GetMesh()->GetSocketByName(Data.EndSocket)->GetSocketLocation(Character->GetMesh());
		FHitResult OutHit;
		ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_Pawn);
		if (Character->GetTeam() == 0) TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_AttackTraceOne);
		else if (Character->GetTeam() == 1) TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_AttackTraceTwo);
		if (UKismetSystemLibrary::SphereTraceSingle
			(
				GetWorld(),
				StartPos, EndPos, Data.TraceRadius,
				TraceTypeQuery, false, ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				OutHit, true
			))
		{
			if (OutHit.GetActor())
			{
				if (OutHit.GetActor()->Implements<URobot>())
				{
					TouchedCharacterInterface = TScriptInterface<IRobot>(OutHit.GetActor());
					TouchedCharacterInterface->TakeDamageFromAttack(
						Data.Damage * (UseBoost ? Data.BoostMultiplier : 1) + (Character->GetAttackDuoBonus() * AttackDuoBonusMultiplier),
						Data.StunTimer);
					bIsAttackTraceEnabled = false;
					HasTouch = true;
					Character->HitStopEvent.Broadcast(Data.Damage + Character->GetAttackDuoBonus());
					Character->AttackManagerEvent.Broadcast(true);
					Character->GuardResetManagerEvent.Broadcast();
					Character->AirStopManagerEvent.Broadcast();

					APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
					float ScaleShake = 1.f;
					float DurationShake = 0.f;
					UCameraShakeBase* TempShake = Camera->StartCameraShake(UCameraShakeWorld::StaticClass(),ScaleShake);
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

void URobotCharacterStateAttack::KnockBackNotify(AActor* ConcernedActor, FVector2D KnockBack)
{
	if (ConcernedActor != GetOwner()) return;
	if (!HasTouch && TouchedCharacterInterface == nullptr) return;
	TouchedCharacterInterface->KnockBackFromNotify(KnockBack);

	TouchedCharacterInterface = nullptr;
}


void URobotCharacterStateAttack::OnStunEvent()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Stun);
}


void URobotCharacterStateAttack::OnBoostEvent()
{
	if (CurrentAnimTime > 0.25f) return;
	UseBoost = true;
}
