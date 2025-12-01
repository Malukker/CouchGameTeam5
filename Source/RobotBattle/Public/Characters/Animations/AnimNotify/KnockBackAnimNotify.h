// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "KnockBackAnimNotify.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FKnockBackAnimNotify,AActor*,ConcernedActor, FVector2D, KnockBack);
UCLASS()
class ROBOTBATTLE_API UKnockBackAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	FKnockBackAnimNotify OnKnockBackEvent;

	UPROPERTY(EditAnywhere)
	FVector2D KnockBack = {0,0};
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
