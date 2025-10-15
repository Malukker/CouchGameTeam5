// Fill out your copyright notice in the Description page of Project Settings.


#include "LocMultiplayerGameViewportClient.h"
#include "Kismet/GameplayStatics.h"	
#include "LocalMultiplayerSubsystem.h"
#include "LocalMultiplayerSettings.h"
#include "EnhancedInputSubsystems.h" 

bool ULocMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	FInputKeyParams params = FInputKeyParams(EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed);
	const ULocalMultiplayerSettings* settings = GetDefault<ULocalMultiplayerSettings>();
	if (GameInstance != nullptr) {
		ULocalMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
		if (EventArgs.IsGamepad())
		{
			int PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.InputDevice.GetId());
			if (PlayerIndex == -1) {
				PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.InputDevice.GetId());
				Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::Menu);
			}
			APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
			if (Controller != nullptr) return Controller->InputKey(params);
			else return Super::InputKey(EventArgs);
		}
	}
	return Super::InputKey(EventArgs);
}

bool ULocMultiplayerGameViewportClient::InputAxis(FViewport* InViewport, FInputDeviceId InputDevice, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	const ULocalMultiplayerSettings* settings = GetDefault<ULocalMultiplayerSettings>();
	if (GameInstance != nullptr){
		ULocalMultiplayerSubsystem* Subsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
		if (bGamepad)
		{
			int PlayerIndex = Subsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
			if (PlayerIndex == -1) {
				PlayerIndex = Subsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
				Subsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::Menu);
			}
			APlayerController* Controller = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), PlayerIndex);
			if (Controller != nullptr) return Controller->InputAxis(Key, Delta, DeltaTime, NumSamples, bGamepad);
			else return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
		}
	}
	return Super::InputAxis(InViewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}