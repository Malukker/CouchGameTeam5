// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/RobotCharacter.h"
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
	UPROPERTY(EditDefaultsOnly)
	TArray<ERobotID> RobotID;
	UPROPERTY(EditDefaultsOnly)
	TArray<uint8> PlayersPos;

	ERobotID GetRobotID(int Number);
	void SetRobotID(int Number, ERobotID ID);
	uint8 GetPlayerPos(int Number);
	void SetPlayerPos(int Number, uint8 Pos);
};
