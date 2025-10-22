// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RobotGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<uint8> RobotID;
	TArray<uint8> PlayersPos;

	void BeginPlay();

	uint8 GetRobotID(int Number);
	void SetRobotID(int Number, uint8 ID);
	uint8 GetPlayerPos(int Number);
	void SetPlayerPos(int Number, uint8 Pos);
};
