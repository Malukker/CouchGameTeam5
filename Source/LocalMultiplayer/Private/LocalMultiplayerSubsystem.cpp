// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"	
#include "EnhancedInputSubsystems.h" 

void ULocalMultiplayerSubsystem::CreatePlayers(ELocalMultiplayerInputMappingType MappingType) {
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	for (int i = 0; i < 4; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID) {
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID)) {
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID) {
	PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
	LastAssignedPlayerIndex++;
	return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const {
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex)
		->GetLocalPlayer()
		->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
		->AddMappingContext(LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType), 0);
}