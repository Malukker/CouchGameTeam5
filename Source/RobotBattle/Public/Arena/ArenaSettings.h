// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Characters/RobotCharacter.h"
#include "ArenaSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Robot Arena Settings"))
class ROBOTBATTLE_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TMap<ERobotID ,TSubclassOf<ARobotCharacter>> RobotCharacterUpClass;
	
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TMap<ERobotID ,TSubclassOf<ARobotCharacter>> RobotCharacterDownClass;

	UPROPERTY(Config, EditAnywhere, Category="SlowMotion",meta=(ClampMin="0.1", ClampMax="1.0", UIMin="0.1", UIMax="1.0"))
	float SlowMotionDuration = 0.5f;

	UPROPERTY(Config, EditAnywhere, Category="SlowMotion",meta=(ClampMin="0.1", ClampMax="1.0", UIMin="0.1", UIMax="1.0"))
	float SlowMotionScale = 0.5f;
};
