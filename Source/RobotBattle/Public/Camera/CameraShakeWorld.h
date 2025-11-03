// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "CameraShakeWorld.generated.h"
class UPerlinNoiseCameraShakePattern;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API UCameraShakeWorld : public UCameraShakeBase
{
	GENERATED_BODY()

public:
	UCameraShakeWorld(const FObjectInitializer& ObjectInitializer);

private:
	UPROPERTY()
	TObjectPtr<UPerlinNoiseCameraShakePattern> ShakePattern;
};
