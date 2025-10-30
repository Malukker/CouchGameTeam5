// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EndAttackDetectionAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FEndOnNotifiedAttack);
UCLASS()
class ROBOTBATTLE_API UEndAttackDetectionAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

	public:
	FEndOnNotifiedAttack OnNotifiedEndAttack;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
