// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleSettingsMenu.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"

void URobotBattleSettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();
}

void URobotBattleSettingsMenu::SetMasterVolume(float NewVolume)
{
	MasterVolume = FMath::Clamp(NewVolume, 0.0f, 1.0f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),nullptr,nullptr, MasterVolume, 1.0f, 0.0f);
}

void URobotBattleSettingsMenu::SetMouseSensitivity(float NewSensitivity)
{
	MouseSensitivity = FMath::Clamp(NewSensitivity, 0.0f, 1.0f);
	UE_LOG(LogTemp, Log, TEXT("Mouse sensitivity set to %f"), MouseSensitivity);
}

void URobotBattleSettingsMenu::ApplySettings()
{
	UE_LOG(LogTemp, Log, TEXT("Settings applied! Volume: %f, Sensitivity: %f"), MasterVolume, MouseSensitivity);
}


