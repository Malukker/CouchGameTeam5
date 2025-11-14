// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeWorld.h"
#include "Characters/RobotCharacter.h"
#include "GameFramework/SaveGame.h"
#include "CameraShakeSave.generated.h"


/**
 * 
 */
UCLASS()
class TOOLBOX_API UCameraShakeSave : public USaveGame
{
	GENERATED_BODY()
public:

#pragma region ShakeValues
	UPROPERTY(VisibleAnywhere, Category="Shake Settings")
	TMap<ERobotID,FShakeScaleTypeStruct> ShakeScaleSettings;

	
	UPROPERTY(VisibleAnywhere, Category="Shake Settings")
	TMap<EAttackID,FShakeStruct> ShakeAttacksSettings;

	UPROPERTY(VisibleAnywhere, Category="Shake Settings")
	float UltimateScale;
#pragma endregion

#pragma region SlowMotion Settings

	UPROPERTY(VisibleAnywhere, Category="SlowMotion Settings")
	float SlowMotionScale;

	UPROPERTY(VisibleAnywhere, Category="SlowMotion Settings")
	float SlowMotionDuration;

#pragma endregion	
};
