// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/AnimNotify/StartAttackDetectionAnimNotify.h"

void UStartAttackDetectionAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	OnNotifiedStartAttack.Broadcast();
	Super::Notify(MeshComp, Animation, EventReference);
}
