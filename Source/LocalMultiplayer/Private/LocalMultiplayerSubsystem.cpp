// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"	
#include "EnhancedInputSubsystems.h" 

void ULocalMultiplayerSubsystem::CreatePlayers() {
	for (int i = 0; i < 4; i++)
	{
		UGameplayStatics::CreatePlayer(GetWorld(), i);
	}
//	FScriptDelegate Delegate;
//	Delegate.BindUFunction(this, "DisconnectPlayer");
//	GetGameInstance()->OnInputDeviceConnectionChange.Add(Delegate);
}

//void ULocalMultiplayerSubsystem::DisconnectPlayer(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId) {
//	int32 DeviceID = InputDeviceId.GetId();
//	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("It HAPPENED to %d"), DeviceID));
//	int PlayerIndex = GetAssignedPlayerIndexFromGamepadDeviceID(DeviceID);
//	int PlayerLostIndex = GetLostPlayerIndexFromGamepadDeviceID(DeviceID);
//	if (PlayerIndex != -1 && NewConnectionState == EInputDeviceConnectionState::Disconnected) {
//		PlayerIndexFromGamepadProfileLostIndex.Add(DeviceID, PlayerIndex);
//		PlayerIndexFromGamepadProfileIndex.Remove(DeviceID);
//		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player %d Disconnected"), PlayerIndex));
//	}
//	else if (PlayerLostIndex != -1 && NewConnectionState == EInputDeviceConnectionState::Connected) {
//		if (CanAssignNewPlayer()) {
//			PlayerIndex = AssignNewPlayerToGamepadDeviceID(DeviceID);
//		}
//	}
//}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID) {
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID)) {
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	return -1;
}

//int ULocalMultiplayerSubsystem::GetLostPlayerIndexFromGamepadDeviceID(int DeviceID) {
//	if (PlayerIndexFromGamepadProfileLostIndex.Contains(DeviceID)) {
//		return *PlayerIndexFromGamepadProfileLostIndex.Find(DeviceID);
//	}
//	return -1;
//}
//
//bool ULocalMultiplayerSubsystem::CanAssignNewPlayer() {
//	if (PlayerIndexFromGamepadProfileIndex.Num() < LastAssignedPlayerIndex || LastAssignedPlayerIndex < 4) {
//		return true;
//	}
//	return false;
//}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID) {
//	//There a player who got is controller disconected
//	if (PlayerIndexFromGamepadProfileIndex.Num() < LastAssignedPlayerIndex) {
//		int LostPlayerIndex = GetLostPlayerIndexFromGamepadDeviceID(DeviceID);
//		//It's the disconected controller trying to make is return
//		if (LostPlayerIndex != -1) {
//			if (PlayerIndexFromGamepadProfileIndex.FindKey(LostPlayerIndex) == nullptr) {
//				PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LostPlayerIndex);
//				PlayerIndexFromGamepadProfileLostIndex.Remove(DeviceID);
//				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player %d Reconnected"), LostPlayerIndex));
//				return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
//			}
//			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player %d was replaced"), LostPlayerIndex));
//		}
//		//It's someone else, like we say "who goes hunting loses his place"
//		else {
//			for (TPair<int32, int32> pair :PlayerIndexFromGamepadProfileLostIndex)
//			{
//				if (PlayerIndexFromGamepadProfileIndex.FindKey(pair.Value) == nullptr) {
//					PlayerIndexFromGamepadProfileIndex.Add(DeviceID, pair.Value);
//					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Player %d replaced"), LostPlayerIndex));
//					return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
//				}
//			}
//		}
//	}
//	//There is no disconnected controller and there is still some empty place
	if (LastAssignedPlayerIndex < 4) {
		PlayerIndexFromGamepadProfileIndex.Add(DeviceID, LastAssignedPlayerIndex);
		LastAssignedPlayerIndex++;
		return *PlayerIndexFromGamepadProfileIndex.Find(DeviceID);
	}
	//The new controller can't be added, the game allow 4 player and no more
	return -1;
}