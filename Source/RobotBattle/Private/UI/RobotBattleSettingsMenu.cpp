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
	MasterVolume = FMath::Clamp(NewVolume, 0.0f, 100.0f);
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(),nullptr,nullptr, MasterVolume, 100.0f, 0.0f);
	UE_LOG(LogTemp, Warning, TEXT("Master Volume changed to: %f"), MasterVolume);
}

void URobotBattleSettingsMenu::ApplySettings()
{
	UE_LOG(LogTemp, Log, TEXT("Settings applied! Volume: %f, Sensitivity: %f"), MasterVolume, MouseSensitivity);
}


