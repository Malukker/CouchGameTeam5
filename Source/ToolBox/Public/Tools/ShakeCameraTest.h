// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "ShakeCameraTest.generated.h"

enum class EAttackID : uint8;
/**
 * 
 */
UCLASS()
class TOOLBOX_API UShakeCameraTest : public UEditorUtilityWidget
{
	GENERATED_BODY()

	public:
	
	UFUNCTION(BlueprintCallable)
	void StartCameraShake();

	UFUNCTION(BlueprintCallable)
	void SetShakeValue();

#pragma region ShakeValue
	UPROPERTY(BlueprintReadWrite)
	float Duration;

	UPROPERTY(BlueprintReadWrite)
	float LocationAmplitudeMultiplier;

	UPROPERTY(BlueprintReadWrite)
	float LocationFrequencyMultiplier;

	UPROPERTY(BlueprintReadWrite)
	float RotationAmplitudeMultiplier;

	UPROPERTY(BlueprintReadWrite)
	float RotationFrequencyMultiplier;

	UPROPERTY(BlueprintReadWrite)
	EAttackID AttackID;

#pragma endregion
};
