// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"	
#include "EnhancedInputSubsystems.h" 

void ULocalMultiplayerSubsystem::CreatePlayers() {
	for (int i = 0; i < 4; i++)
	{
		if (Controllers.Num() <= i)
		{
			Controllers.Add(UGameplayStatics::CreatePlayer(GetWorld(), i));
		}
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID) {
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID)) {
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID) {
	if (LastAssignedPlayerIndex < 4) {
		PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
		LastAssignedPlayerIndex++;
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	return -1;
}