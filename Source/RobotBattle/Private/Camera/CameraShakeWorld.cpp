// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraShakeWorld.h"

#include "Camera/CameraSettings.h"
#include "Characters/Attacks/AttackStruct.h"
#include "Characters/Enums/RobotCharacterUpID.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"
#include "WorldPartition/ContentBundle/ContentBundleLog.h"

void UCameraShakeWorld::SetupShakeParametersOnAttackID(EAttackID AttackID,ERobotCharacterUpID UpID,float& Scale )
{
	if (UpID==ERobotCharacterUpID::None)
	{
		UE_LOG(LogTemp, Log, TEXT("ID UP NONE PAS DE SHAKE"));
		return;
	}
	ShakePattern->Duration = CameraSettings->ShakeAttacksSettings[AttackID].ShakeDuration;
	ShakePattern->LocationAmplitudeMultiplier =CameraSettings->ShakeAttacksSettings[AttackID].LocationAmplitudeMultiplier;
	ShakePattern->LocationFrequencyMultiplier= CameraSettings->ShakeAttacksSettings[AttackID].LocationFrequencyMultiplier;
	ShakePattern->RotationAmplitudeMultiplier= CameraSettings->ShakeAttacksSettings[AttackID].RotationAmplitudeMultiplier;
	ShakePattern->RotationFrequencyMultiplier= CameraSettings->ShakeAttacksSettings[AttackID].RotationFrequencyMultiplier;

	Scale=CameraSettings->ShakeScaleSettings[UpID].ShakeScaleTypes[AttackID];
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

