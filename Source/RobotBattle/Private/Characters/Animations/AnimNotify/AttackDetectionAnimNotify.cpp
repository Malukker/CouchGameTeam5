// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/AnimNotify/AttackDetectionAnimNotify.h"

void UAttackDetectionAnimNotify::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	OnNotifiedAttackStart.Broadcast(MeshComp->GetOwner());
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

void UAttackDetectionAnimNotify::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	OnNotifiedAttack.Broadcast(MeshComp->GetOwner(), Data);
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);
}
