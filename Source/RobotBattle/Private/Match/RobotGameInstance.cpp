// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/RobotGameInstance.h"

void URobotGameInstance::BeginPlay()
{
	RobotID.Init(0,4);
}

uint8 URobotGameInstance::GetRobotID(int Number)
{
	return RobotID[Number];
}

void URobotGameInstance::SetRobotID(int Number, uint8 ID)
{
	RobotID[Number] = ID;
}
