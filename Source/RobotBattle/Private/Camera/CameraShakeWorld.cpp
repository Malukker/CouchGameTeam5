// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraShakeWorld.h"

#include "Camera/CameraSettings.h"
#include "Characters/Attacks/AttackStruct.h"
#include "Shakes/WaveOscillatorCameraShakePattern.h"

void UCameraShakeWorld::SetupShakeParametersOnAttackID(EAttackID AttackID,ERobotID UpID,float& Scale,float& Duration )
{
	if (UpID==ERobotID::None || bEnableCameraShake == false)
	{
		UE_LOG(LogTemp, Log, TEXT("ID UP NONE PAS DE SHAKE"));
		return;
	}
	
	Duration = CameraSettings->ShakeAttacksSettings[AttackID].ShakeDuration;
	ShakePattern->LocationAmplitudeMultiplier =CameraSettings->ShakeAttacksSettings[AttackID].LocationAmplitudeMultiplier;
	ShakePattern->LocationFrequencyMultiplier= CameraSettings->ShakeAttacksSettings[AttackID].LocationFrequencyMultiplier;
	ShakePattern->RotationAmplitudeMultiplier= CameraSettings->ShakeAttacksSettings[AttackID].RotationAmplitudeMultiplier;
	ShakePattern->RotationFrequencyMultiplier= CameraSettings->ShakeAttacksSettings[AttackID].RotationFrequencyMultiplier;

	Scale=CameraSettings->ShakeScaleSettings[UpID].ShakeScaleTypes[AttackID];
}

void UCameraShakeWorld::SetupShakeParametersForTool(float LocationAmplitudeMultiplier,
	float LocationFrequencyMultiplier, float RotationAmplitudeMultiplier, float RotationFrequencyMultiplier)
{
	
	ShakePattern->LocationAmplitudeMultiplier = LocationAmplitudeMultiplier;
	ShakePattern->LocationFrequencyMultiplier =  LocationFrequencyMultiplier;
	ShakePattern->RotationAmplitudeMultiplier = RotationAmplitudeMultiplier; 
	ShakePattern->RotationFrequencyMultiplier = RotationFrequencyMultiplier; 
}


UCameraShakeWorld::UCameraShakeWorld(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		ShakePattern = CreateDefaultSubobject<UWaveOscillatorCameraShakePattern>(TEXT("MyPerlinPattern"));
		ShakePattern->Duration = 500.F;
		

		if (ShakePattern)
		{
			SetRootShakePattern(ShakePattern);
		}
	}
}

