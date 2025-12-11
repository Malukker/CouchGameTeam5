// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleSettingsMenu.h"

#include "Sound/SoundClass.h"
#include "Match/RobotGameInstance.h"

void URobotBattleSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
	GI = GetGameInstance<URobotGameInstance>();
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
	
	if (GI)
	{
		GI->bEnableCameraShake = bCameraShakeEnabled;
		GI->bEnableControllerVibration = bControllerVibrationEnabled;
	}
}

