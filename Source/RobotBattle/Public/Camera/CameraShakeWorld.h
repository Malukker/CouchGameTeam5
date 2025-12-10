// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "CameraShakeWorld.generated.h"
enum class ERobotID : uint8;
class UCameraSettings;
enum class EAttackID : uint8;
class UWaveOscillatorCameraShakePattern;


USTRUCT(BlueprintType)
struct FShakeStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float ShakeDuration = 0.5f;

	UPROPERTY(EditAnywhere)
	float LocationAmplitudeMultiplier = 5.0f;

	UPROPERTY(EditAnywhere)
	float LocationFrequencyMultiplier = 20.0f;

	UPROPERTY(EditAnywhere)
	float RotationAmplitudeMultiplier = 2.5f;

	UPROPERTY(EditAnywhere)
	float RotationFrequencyMultiplier = 20.0f;
	

};


USTRUCT(BlueprintType)
struct FShakeScaleTypeStruct
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TMap<EAttackID,float> ShakeScaleTypes;
};


UCLASS()
class ROBOTBATTLE_API UCameraShakeWorld : public UCameraShakeBase
{
	GENERATED_BODY()

public:
	void SetupShakeParametersOnAttackID(EAttackID AttackID,ERobotID UpID,float& Scale,float& Duration);
	void SetupShakeParametersForTool(float LocationAmplitudeMultiplier,float LocationFrequencyMultiplier,float RotationAmplitudeMultiplier,float RotationFrequencyMultiplier);
	UCameraShakeWorld(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadWrite, BlueprintReadWrite, Category="Shake")
	bool bEnableCameraShake = true; 

private:

	UPROPERTY()
	const UCameraSettings* CameraSettings = GetDefault<UCameraSettings>();
	UPROPERTY()
	TObjectPtr<UWaveOscillatorCameraShakePattern> ShakePattern;
};
