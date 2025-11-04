// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "CameraShakeWorld.generated.h"
class UCameraSettings;
enum class EAttackID : uint8;
class UPerlinNoiseCameraShakePattern;


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
	UCameraShakeWorld(const FObjectInitializer& ObjectInitializer);

	
	void SetupShakeParametersOnAttackID(EAttackID id);

private:

	UPROPERTY()
	const UCameraSettings* CameraSettings = GetDefault<UCameraSettings>();
	UPROPERTY()
	TObjectPtr<UPerlinNoiseCameraShakePattern> ShakePattern;
};
