// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "MatchGameMode.generated.h"

class ARobotCharacter;
class AArenaPlayerStart;
class URobotCharacterInputData;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	URobotCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position);

	void FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors);

	void SpawnCharacters(const TArray<AArenaPlayerStart*> Spawnpoints);

	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayers() const;
};