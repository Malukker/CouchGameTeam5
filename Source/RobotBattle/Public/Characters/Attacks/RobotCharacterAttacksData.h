// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AttackStruct.h"
#include "RobotCharacterAttacksData.generated.h"

USTRUCT(BlueprintType)
struct FAttackFromID
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TMap<EAttackID,FAttackStruct> AttacksFromID = {};
};

UCLASS()
class ROBOTBATTLE_API URobotCharacterAttacksData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<ERobotID, FAttackFromID> ListAttacks = {};

	UPROPERTY(EditAnywhere)
	TMap<ERobotID,float> LoadingAttack;
};

