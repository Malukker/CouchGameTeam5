// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/ShakeCameraTest.h"

#include "Camera/CameraSettings.h"
#include "Camera/CameraShakeWorld.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/UEdMode.h"

void UShakeCameraTest::StartCameraShake()
{
	//Start a camera shake
	APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
	UCameraShakeBase* TempShake =Camera->StartCameraShake(UCameraShakeWorld::StaticClass(),Scale);
	UCameraShakeWorld* ShakeInstance = Cast<UCameraShakeWorld>(TempShake);
	ShakeInstance->SetupShakeParametersForTool(LocationAmplitudeMultiplier,LocationFrequencyMultiplier,RotationAmplitudeMultiplier,RotationFrequencyMultiplier);
}

void UShakeCameraTest::SaveShakeValue()
{
	if (AttackID == EAttackID::None) return;
	 UCameraSettings* Settings = GetMutableDefault<UCameraSettings>();
	
	FShakeStruct ShakeStruct = Settings->ShakeAttacksSettings[AttackID];
	ShakeStruct.LocationAmplitudeMultiplier = LocationAmplitudeMultiplier;
	ShakeStruct.LocationFrequencyMultiplier = LocationFrequencyMultiplier;
	ShakeStruct.RotationAmplitudeMultiplier = RotationAmplitudeMultiplier;
	ShakeStruct.RotationFrequencyMultiplier = RotationFrequencyMultiplier;
	ShakeStruct.ShakeDuration = Duration;
	Settings->SaveConfig();
	
}

void UShakeCameraTest::SaveShakeScaleValue()
{
	if (RobotID == ERobotID::None || AttackID == EAttackID::None) return;
	UCameraSettings* Settings = GetMutableDefault<UCameraSettings>();
	if (Scale > 0.f)
	{
		FShakeScaleTypeStruct ScaleTypeStruct = Settings->ShakeScaleSettings[RobotID];
		ScaleTypeStruct.ShakeScaleTypes[AttackID] = Scale;
	}

	if (ShakeScaleUltimate < 0.f)
	{
		Settings->ShakeScaleUltimate = ShakeScaleUltimate;
	}
	Settings->SaveConfig();
}
