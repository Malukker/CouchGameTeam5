// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Characters/RobotCharacter.h"
#include "ArenaSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta = (DisplayName = "Smash Arena Settings"))
class ROBOTBATTLE_API UArenaSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ARobotCharacter> RobotCharacterClassP0;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ARobotCharacter> RobotCharacterClassP1;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ARobotCharacter> RobotCharacterClassP2;

	UPROPERTY(Config, EditAnywhere, Category="Characters")
	TSubclassOf<ARobotCharacter> RobotCharacterClassP3;
};
