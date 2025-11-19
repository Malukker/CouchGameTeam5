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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<ERobotID> RobotID;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<uint8> PlayersPos;

	

	ERobotID GetRobotID(int Number);
	void SetRobotID(int Number, ERobotID ID);
	uint8 GetPlayerPos(int Number);
	void SetPlayerPos(int Number, uint8 Pos);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnInputChange(int DeviceID,EInputDeviceConnectionState State);

#if WITH_EDITORONLY_DATA
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Controllers;


#endif

#if WITH_EDITOR
	virtual void Init() override;

	
	
	UFUNCTION()
	void AddControllerOnList(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId);



#endif
};
