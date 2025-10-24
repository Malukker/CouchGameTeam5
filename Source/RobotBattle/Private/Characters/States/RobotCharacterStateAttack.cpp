// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateAttack.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Characters/Animations/AnimNotify/EndAttackDetectionAnimNotify.h"
#include "Characters/Animations/AnimNotify/StartAttackDetectionAnimNotify.h"
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
		if (CurrentTime / KeyframeDeltaTime < 1.f / 60.f)
		{
			FVector StartPos = Character->GetMesh()->GetSocketByName(StartSocketName)->GetSocketLocation();
			FVector EndPos = Character->GetMesh()->GetSocketByName(EndSocketName)->GetSocketLocation();
	
			UKismetSystemLibrary::SphereTraceSingle(GetWorld(), )
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

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("End Detection Notify")
	);
}
