// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "StartAttackDetectionAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FStartOnNotifiedAttack);
UCLASS()
class ROBOTBATTLE_API UStartAttackDetectionAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FStartOnNotifiedAttack OnNotifiedStartAttack;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
