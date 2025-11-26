// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Animations/AnimNotify/KnockBackAnimNotify.h"

void UKnockBackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	OnKnockBackEvent.Broadcast(MeshComp->GetOwner());
}
