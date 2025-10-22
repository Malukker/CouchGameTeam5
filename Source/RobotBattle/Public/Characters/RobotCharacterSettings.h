// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RobotCharacterSettings.generated.h"

class URobotCharacterInputData;
class UInputMappingContext;

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Smash Character Settings"))
class ROBOTBATTLE_API URobotCharacterSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<URobotCharacterInputData> InputData;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContextUp;
	
	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	TSoftObjectPtr<UInputMappingContext> InputMappingContextDown;

	UPROPERTY(Config, EditAnywhere, Category = "Inputs")
	float InputMoveXThreshold = 0.1f;
};
