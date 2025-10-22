// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/RobotGameInstance.h"

void URobotGameInstance::BeginPlay()
{
	RobotID.Init(ERobotID::None,4);
	PlayersPos.Init(0,4);
}

ERobotID URobotGameInstance::GetRobotID(int Number)
{
	return RobotID[Number];
}

void URobotGameInstance::SetRobotID(int Number, ERobotID ID)
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
