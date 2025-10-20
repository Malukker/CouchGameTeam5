// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LocalMultiplayerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class LOCALMULTIPLAYER_API ULocalMultiplayerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CreatePlayers();
	UFUNCTION(BlueprintCallable)
	//void DisconnectPlayer(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceIde);
	int GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID);
	//int GetLostPlayerIndexFromGamepadDeviceID(int DeviceID);
	//bool CanAssignNewPlayer();
	int AssignNewPlayerToGamepadDeviceID(int DeviceID);

protected:
	UPROPERTY()
	uint8 LastAssignedPlayerIndex = 0;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileIndex;

	UPROPERTY()
	TMap<int, int> PlayerIndexFromGamepadProfileLostIndex;
};
