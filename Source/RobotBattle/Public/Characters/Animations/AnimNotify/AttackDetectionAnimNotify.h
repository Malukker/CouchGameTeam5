// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Characters/Attacks/AttackStruct.h"
#include "AttackDetectionAnimNotify.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNotifiedAttack, AActor*, ConcernedActor, FAttackStruct, Data);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifiedAttackStart, AActor*, ConcernedActor);
UCLASS()
class ROBOTBATTLE_API UAttackDetectionAnimNotify : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FOnNotifiedAttack OnNotifiedAttack;
	FOnNotifiedAttackStart OnNotifiedAttackStart;

	UPROPERTY(EditAnywhere)
	FAttackStruct Data;

	virtual  void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
};
