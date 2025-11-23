// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraShakeWorld.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

enum class ERobotID : uint8;
enum class ERobotCharacterUpID : uint8;
enum class EAttackID : uint8;


UCLASS(Config=Game,DefaultConfig,meta=(DisplayName = "Camera Settings"))
class ROBOTBATTLE_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

#pragma region Camera Settings
	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	FName CameraMainTag;

	//UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	//FName CameraDistanceMinTag;

	//UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	//FName CameraDistanceMaxTag;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	FName CameraBoundsTag;

	//UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	//float DistanceBetweenTargetsMin;

	//UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	//float DistanceBetweenTargetsMax;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	float HeightOffset;
	
	//UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	//float SizeDampingFactor;

	UPROPERTY(Config,EditAnywhere, Category = "CameraSettings")
	float PositionDampingFactor;
#pragma endregion Camera Settings

	
#pragma region Camera Shake Settings
	
	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	TMap<ERobotID,FShakeScaleTypeStruct> ShakeScaleSettings;
	
	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	TMap<EAttackID,FShakeStruct> ShakeAttacksSettings;

	UPROPERTY(Config,EditAnywhere, Category = "CameraShakeSettings")
	float ShakeScaleUltimate = 4.f;
	

#pragma endregion Camera Shake Settings
	
};
