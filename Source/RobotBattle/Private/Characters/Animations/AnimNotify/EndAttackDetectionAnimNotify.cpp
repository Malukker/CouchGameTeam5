// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/AnimNotify/EndAttackDetectionAnimNotify.h"

void UEndAttackDetectionAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OnNotifiedEndAttack.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
