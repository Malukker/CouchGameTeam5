// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/RobotGameInstance.h"

void URobotGameInstance::BeginPlay()
{
	RobotID.Init(0,4);
	PlayersPos.Init(0,4);
}

uint8 URobotGameInstance::GetRobotID(int Number)
{
	return RobotID[Number];
}

void URobotGameInstance::SetRobotID(int Number, uint8 ID)
{
	RobotID[Number] = ID;
}

uint8 URobotGameInstance::GetPlayerPos(int Number)
{
	return PlayersPos[Number];
}

void URobotGameInstance::SetPlayerPos(int Number, uint8 Pos)
{
	PlayersPos[Number] = Pos;
}
