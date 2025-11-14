// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SlowMotion/SlowMotionTestRuntime.h"

#include "Arena/ArenaSettings.h"
#include "Camera/CameraSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/Shake/CameraShakeSave.h"
#include "Tools/SlowMotion/SlowMotionActorTest.h"

void USlowMotionTestRuntime::SetSlowMotion()
{
	ASlowMotionActorTest* Actor = Cast<ASlowMotionActorTest>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASlowMotionActorTest::StaticClass()));
	Actor->StartSlowMotion.Execute(Scale,Duration);
}

void USlowMotionTestRuntime::SaveSlowMotion()
{
	UCameraShakeSave* SaveGameInstance = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(TEXT("CameraSettingsSlot"), 0))
	{
		SaveGameInstance = Cast<UCameraShakeSave>(UGameplayStatics::LoadGameFromSlot(TEXT("CameraSettingsSlot"), 0));
	}
	else
	{
		SaveGameInstance = Cast<UCameraShakeSave>(UGameplayStatics::CreateSaveGameObject(UCameraShakeSave::StaticClass()));
	}

	SaveGameInstance->SlowMotionDuration = Duration;
	SaveGameInstance->SlowMotionScale = Scale;

	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("CameraSettingsSlot"), 0))
	{
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Saved!"), true);
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


	UArenaSettings* Settings = GetMutableDefault<UArenaSettings>();
	

	Settings->SlowMotionDuration = SaveData->SlowMotionDuration;
	Settings->SlowMotionScale = SaveData->SlowMotionScale;
	
	Settings->SaveConfig();

	UToolBoxFunctionLibrary::SlateNotification(FText::FromString("Slow Motion values applied to settings"), true);
#pragma endregion	
}

void USlowMotionTestRuntime::DestroyActor()
{
	if (AActor* ShakeActor =UGameplayStatics::GetActorOfClass(GetWorld(),ASlowMotionActorTest::StaticClass()))
	{
		ShakeActor->Destroy();
	}
}

void USlowMotionTestRuntime::Tick(float DeltaTime)
{

}
