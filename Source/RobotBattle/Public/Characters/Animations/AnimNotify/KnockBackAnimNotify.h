// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KnockBackAnimNotify.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKnockBackAnimNotify,AActor*,ConcernedActor);
UCLASS()
class ROBOTBATTLE_API UKnockBackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FKnockBackAnimNotify OnKnockBackEvent;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
