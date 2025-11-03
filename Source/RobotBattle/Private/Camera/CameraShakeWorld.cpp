// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraShakeWorld.h"

#include "Camera/CameraSettings.h"
#include "Characters/Attacks/AttackStruct.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"

void UCameraShakeWorld::SetupShakeParametersOnAttackID(EAttackID id)
{
	ShakePattern->Duration = CameraSettings->ShakeAttacksSettings[id].ShakeDuration;
	ShakePattern->LocationAmplitudeMultiplier =CameraSettings->ShakeAttacksSettings[id].LocationAmplitudeMultiplier;
	ShakePattern->LocationFrequencyMultiplier= CameraSettings->ShakeAttacksSettings[id].LocationFrequencyMultiplier;
	ShakePattern->RotationAmplitudeMultiplier= CameraSettings->ShakeAttacksSettings[id].RotationAmplitudeMultiplier;
	ShakePattern->RotationFrequencyMultiplier= CameraSettings->ShakeAttacksSettings[id].RotationFrequencyMultiplier;
}


UCameraShakeWorld::UCameraShakeWorld(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		ShakePattern = NewObject<UPerlinNoiseCameraShakePattern>(this, NAME_None, RF_Transient);

		if (ShakePattern)
		{
			SetRootShakePattern(ShakePattern);
		}
	}
}

