// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleSettingsMenu.h"

#include "Sound/SoundClass.h"
#include "Match/TeamManager.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"
#include "Camera/CameraShakeWorld.h"

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
	
	if (UWorld* World = GetWorld())
	{
		if (URobotGameInstance* GI = World->GetGameInstance<URobotGameInstance>())
		{
			GI->bEnableCameraShake = bCameraShakeEnabled;
			GI->bEnableControllerVibration = bControllerVibrationEnabled;
		}
	}
}

