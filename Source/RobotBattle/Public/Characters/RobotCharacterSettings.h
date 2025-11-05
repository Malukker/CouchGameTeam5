// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RobotCharacterSettings.generated.h"

class URobotCharacterInputData;
class URobotCharacterAttacksData;
class UInputMappingContext;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Robot Character Settings"))
class ROBOTBATTLE_API URobotCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<URobotCharacterInputData> InputDataGameplay;
	
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<URobotCharacterInputData> InputDataMenu;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContextUp;
	
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContextDown;
	
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContextMenu;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	float InputMoveXThreshold = 0.1f;
	
	UPROPERTY(Config, EditAnywhere, Category = "Attacks")
	TSoftObjectPtr<URobotCharacterAttacksData> AttackData;
};
