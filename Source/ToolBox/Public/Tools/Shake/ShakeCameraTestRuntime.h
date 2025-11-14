// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Attacks/AttackStruct.h"
#include "ShakeCameraTestRuntime.generated.h"


class UToolBoxFunctionLibrary;
enum class ERobotID : uint8;
enum class EAttackID : uint8;

UCLASS()
class TOOLBOX_API UShakeCameraTestRuntime : public UUserWidget
{
	GENERATED_BODY()


public:
	
	UFUNCTION(BlueprintCallable)
	void StartCameraShake();

	UFUNCTION(BlueprintCallable)
	void SaveShakeValue();

	
	UFUNCTION(BlueprintCallable)
	void SaveShakeScaleValue();

	UFUNCTION(BlueprintCallable)
	void DestroyActor();

	

#pragma region ShakeValue

	UPROPERTY(BlueprintReadWrite)
	float Scale = 2.f;
	
	UPROPERTY(BlueprintReadWrite)
	float Duration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float LocationAmplitudeMultiplier = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float LocationFrequencyMultiplier= 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RotationAmplitudeMultiplier= 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RotationFrequencyMultiplier= 0.f;

	UPROPERTY(BlueprintReadWrite)
	EAttackID AttackID= EAttackID::None;

	UPROPERTY(BlueprintReadWrite)
	ERobotID RobotID = ERobotID::None;

	
	UPROPERTY(BlueprintReadWrite)
	float ShakeScaleUltimate = 0.f;

#pragma endregion

private:
	UPROPERTY()
	FTimerHandle TimerHandle;
};
