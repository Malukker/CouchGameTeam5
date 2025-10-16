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
	FScriptDelegate Delegate;
	Delegate.BindUFunction(this, "DisconnectPlayer");
	GetGameInstance()->OnInputDeviceConnectionChange.Add(Delegate);
}

void ULocalMultiplayerSubsystem::DisconnectPlayer(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId) {
	if (NewConnectionState == EInputDeviceConnectionState::Disconnected) {
		int32 DeviceID = InputDeviceId.GetId();
		PlayerIndexFromGamepadProfileLostIndex.Add(DeviceID, GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID));
		PlayerIndexFromGamepadProfileIndex.Remove(DeviceID);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID) {
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID)) {
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	return -1;
}

int ULocalMultiplayerSubsystem::GetLostPlayerIndexFromGamepadDeviceID(int DeviceID) {
	if (PlayerIndexFromGamepadProfileLostIndex.Contains(DeviceID)) {
		return *PlayerIndexFromGamepadProfileLostIndex.Find(DeviceID);
	}
	return -1;
}

bool ULocalMultiplayerSubsystem::CanAssignNewPlayer() {
	if (PlayerIndexFromGamepadProfileIndex.Num() < LastAssignedPlayerIndex || LastAssignedPlayerIndex < 4) {
		return true;
	}
	return false;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID) {
	//There a player who got is controller disconected
	if (PlayerIndexFromGamepadProfileIndex.Num() < LastAssignedPlayerIndex) {
		int LostPlayerIndex = GetLostPlayerIndexFromGamepadDeviceID(DeviceID);
		//It's the disconected controller trying to make is return
		if (LostPlayerIndex != -1) {
			if (PlayerIndexFromGamepadProfileIndex.FindKey(LostPlayerIndex) == nullptr) {
				PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LostPlayerIndex);
				PlayerIndexFromGamepadProfileLostIndex.Remove(DeviceID);
				return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
			}
		}
		//It's someone else, like we say "who goes hunting loses his place"
		else {
			for (TPair<int32, int32> pair :PlayerIndexFromGamepadProfileLostIndex)
			{
				if (PlayerIndexFromGamepadProfileIndex.FindKey(pair.Value) == nullptr) {
					PlayerIndexFromGamepadProfileIndex.Add(DeviceID, pair.Value);
					return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
				}
			}
		}
	}
	//There is no disconnected controller and there is still some empty place
	if (LastAssignedPlayerIndex < 4) {
		PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
		LastAssignedPlayerIndex++;
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	//The new controller can't be added, the game allow 4 player and no more
	return -1;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const {
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	FModifyContextOptions Options;
	Options.bForceImmediately = true;
	UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex)
		->GetLocalPlayer()
		->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
		->AddMappingContext(LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType), 0, Options);
}