// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RobotControllerVibrationData.generated.h"

enum class EAttackID : uint8;
enum class ERobotID : uint8;

USTRUCT(BlueprintType)
struct FVibrationAttackType
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere)
	TMap<EAttackID,UForceFeedbackEffect*> RobotAttackType;


};




UCLASS()
class ROBOTBATTLE_API URobotControllerVibrationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<ERobotID,FVibrationAttackType> VibrationMap ;

	UPROPERTY(EditAnywhere)
	UForceFeedbackEffect* GuardBreakVibration;

};
