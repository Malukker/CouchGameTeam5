// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game,DefaultConfig,meta=(DisplayName = "Camera Settings"))
class ROBOTBATTLE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

#pragma region Camera Settings
	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	FName CameraMainTag;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	FName CameraDistanceMinTag;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	FName CameraDistanceMaxTag;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	FName CameraBoundsTag;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	float DistanceBetweenTargetsMin;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	float DistanceBetweenTargetsMax;
	
	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	float SizeDampingFactor;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	float PositionDampingFactor;
#pragma endregion Camera Settings
#pragma region Camera Shake Settings

	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	float ShakeDuration = 0.5f;

	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	float LocationAmplitudeMultiplier = 5.0f;

	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	float LocationFrequencyMultiplier = 20.0f;

	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	float RotationAmplitudeMultiplier = 2.5f;

	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	float RotationFrequencyMultiplier = 20.0f;

#pragma endregion Camera Shake Settings
	
};
