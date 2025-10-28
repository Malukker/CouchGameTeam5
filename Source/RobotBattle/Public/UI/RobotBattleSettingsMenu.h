// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleSettingsMenu.generated.h"

UCLASS()
class ROBOTBATTLE_API URobotBattleSettingsMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void SetMasterVolume(float NewVolume);

	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplySettings();

private:
	float MasterVolume = 1.0f;
	float MouseSensitivity = 1.0f;
};
