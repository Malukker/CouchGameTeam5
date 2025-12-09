// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleSettingsMenu.h"

#include "Camera/CameraShakeWorld.h"
#include "Sound/SoundClass.h"
#include "Kismet/GameplayStatics.h"

void URobotBattleSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void URobotBattleSettingsMenu::SetMasterVolume(float NewVolume)
{
	MasterVolume = FMath::Clamp(NewVolume, 0.0f, 1.0f);
	UE_LOG(LogTemp, Warning, TEXT("Master Volume changed to: %f"), MasterVolume);
}

void URobotBattleSettingsMenu::ApplySettings()
{
	if (MasterSoundClass !=nullptr)
	{
		MasterSoundClass->Properties.Volume = MasterVolume;
		
		UE_LOG(LogTemp, Log, TEXT("Settings applied! Volume: %f"), MasterVolume);
	}
	
	UCameraShakeWorld* ShakeCDO = GetMutableDefault<UCameraShakeWorld>();
	ShakeCDO->bEnableCameraShake = bCameraShakeEnabled;
}

