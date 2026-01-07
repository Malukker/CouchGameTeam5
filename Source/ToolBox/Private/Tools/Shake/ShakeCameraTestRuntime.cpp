// Fill out your copyright notice in the Description page of Project Settings.



#include "Tools/Shake/ShakeCameraTestRuntime.h"

#include "Tools/ToolBoxFunctionLibrary.h"
#include "Camera/CameraSettings.h"
#include "Characters/Attacks/AttackStruct.h"

#include "Kismet/GameplayStatics.h"
#include "Tools/Shake/CameraShakeSave.h"
#include "Tools/Shake/ShakeActorTest.h"




void UShakeCameraTestRuntime::StartCameraShake()
{
	
	APlayerCameraManager* Camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
          
    UCameraShakeBase* TempShake =Camera->StartCameraShake(UCameraShakeWorld::StaticClass(),Scale);
	if (Duration==0 || Scale==0)
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("ShakeTestFailed"),false,FText::FromString("Check Duration or Scale"));
		return;
	}
	UCameraShakeWorld* ShakeInstance = Cast<UCameraShakeWorld>(TempShake);
	ShakeInstance->SetupShakeParametersForTool(LocationAmplitudeMultiplier,LocationFrequencyMultiplier,RotationAmplitudeMultiplier,RotationFrequencyMultiplier);
	
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Camera,ShakeInstance]()
	{
		Camera->StopCameraShake(ShakeInstance, false);
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("ShakeTest Ended"), true);
	}, Duration, false);
	
	UToolBoxFunctionLibrary::SlateNotification(FText::FromString("ShakeTestSuccess"),true);
}

void UShakeCameraTestRuntime::SaveShakeValue()
{
	if (AttackID == EAttackID::None)
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("SaveShakeFailed"),false,FText::FromString("AttackID not valid"));
		return;
	} 
	
	UCameraShakeSave* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(TEXT("CameraSettingsSlot"), 0))
	{
		SaveGameInstance = Cast<UCameraShakeSave>(UGameplayStatics::LoadGameFromSlot(TEXT("CameraSettingsSlot"), 0));
	}
	else
	{
		SaveGameInstance = Cast<UCameraShakeSave>(UGameplayStatics::CreateSaveGameObject(UCameraShakeSave::StaticClass()));
	}

	FShakeStruct ShakeStruct ;
	ShakeStruct.LocationAmplitudeMultiplier = LocationAmplitudeMultiplier;
	ShakeStruct.LocationFrequencyMultiplier = LocationFrequencyMultiplier;
	ShakeStruct.RotationAmplitudeMultiplier = RotationAmplitudeMultiplier;
	ShakeStruct.RotationFrequencyMultiplier = RotationFrequencyMultiplier;
	ShakeStruct.ShakeDuration = Duration;

	
	SaveGameInstance->ShakeAttacksSettings.Add(AttackID, ShakeStruct);


	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("CameraSettingsSlot"), 0))
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Shake Saved!"), true);
	}
	else
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Save Failed!"), false);
	}

#pragma region SaveToSettings
	UCameraShakeSave* SaveData = Cast<UCameraShakeSave>(
		UGameplayStatics::LoadGameFromSlot(TEXT("CameraSettingsSlot"), 0));

	if (!SaveData)
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("No saved data found"), false);
		return;
	}


	UCameraSettings* Settings = GetMutableDefault<UCameraSettings>();
	
	for (const TPair<EAttackID, FShakeStruct>& Pair : SaveData-> ShakeAttacksSettings)
	{
		Settings->ShakeAttacksSettings.Add(Pair.Key, Pair.Value);
	}
	
	
	Settings->SaveConfig();

	UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Camera shake values applied to settings"), true);
#pragma endregion
}

void UShakeCameraTestRuntime::SaveShakeScaleValue()
{
	if ((RobotID == ERobotID::None || AttackID == EAttackID::None)&& ShakeScaleUltimate <= 0.f )
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("SaveScaleValueFailed"),false,FText::FromString("AttackID or RobotID not valid"));
		return;
	}
	
	UCameraShakeSave* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(TEXT("CameraSettingsSlot"), 0))
	{
		SaveGameInstance = Cast<UCameraShakeSave>(UGameplayStatics::LoadGameFromSlot(TEXT("CameraSettingsSlot"), 0));
	}
	else
	{
		SaveGameInstance = Cast<UCameraShakeSave>(UGameplayStatics::CreateSaveGameObject(UCameraShakeSave::StaticClass()));
	}
	

	if (Scale > 0.f)
	{
		FShakeScaleTypeStruct* ExistingStruct = SaveGameInstance->ShakeScaleSettings.Find(RobotID);

		if (ExistingStruct)
		{
			ExistingStruct->ShakeScaleTypes.Add(AttackID, Scale);
		}
		else
		{
			FShakeScaleTypeStruct NewStruct;
			NewStruct.ShakeScaleTypes.Add(AttackID, Scale);
			SaveGameInstance->ShakeScaleSettings.Add(RobotID, NewStruct);
		}
	}

	if (ShakeScaleUltimate > 0.f)
	{
		SaveGameInstance->UltimateScale = ShakeScaleUltimate;
	}
	
	
	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("CameraSettingsSlot"), 0))
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Save Scale Saved!"), true);
	}
	else
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Save Scale Failed!"), false);
	}

#pragma region SaveToSettings
	UCameraShakeSave* SaveData = Cast<UCameraShakeSave>(
			UGameplayStatics::LoadGameFromSlot(TEXT("CameraSettingsSlot"), 0));

	if (!SaveData)
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("No saved data found"), false);
		return;
	}


	UCameraSettings* Settings = GetMutableDefault<UCameraSettings>();
	


	for (const TPair<ERobotID,FShakeScaleTypeStruct>& Pair : SaveData-> ShakeScaleSettings)
	{
		Settings->ShakeScaleSettings.Add(Pair.Key, Pair.Value);
	}

	Settings->ShakeScaleUltimate = SaveData->UltimateScale;
	
	
	Settings->SaveConfig();

	UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Camera scale values applied to settings"), true);
#pragma endregion
}

void UShakeCameraTestRuntime::DestroyActor()
{

	if (AActor* ShakeActor =UGameplayStatics::GetActorOfClass(GetWorld(),AShakeActorTest::StaticClass()))
	{
		ShakeActor->Destroy();
	}
	
}


