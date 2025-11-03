// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraShakeWorld.h"

#include "Camera/CameraSettings.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"


UCameraShakeWorld::UCameraShakeWorld(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		const UCameraSettings* CameraSettings = GetDefault<UCameraSettings>();
		ShakePattern = NewObject<UPerlinNoiseCameraShakePattern>(this, NAME_None, RF_Transient);

		if (ShakePattern)
		{
		
			ShakePattern->Duration = CameraSettings->ShakeDuration;
			ShakePattern->LocationAmplitudeMultiplier = CameraSettings->LocationAmplitudeMultiplier;
			ShakePattern->LocationFrequencyMultiplier = CameraSettings->LocationFrequencyMultiplier;
			
			ShakePattern->RotationAmplitudeMultiplier = CameraSettings->RotationAmplitudeMultiplier;
			ShakePattern->RotationFrequencyMultiplier = CameraSettings->RotationFrequencyMultiplier;
			
			SetRootShakePattern(ShakePattern);
		}
	}
}
