// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RobotGameInstance.generated.h"


enum class  ERobotID : uint8;
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
	
	UPROPERTY()
	bool ChangeCharacter = false;
	
	UFUNCTION(BlueprintCallable)
	void SetChangeCharacter(bool value);
	
	UFUNCTION(BlueprintCallable)
	bool GetChangeCharacter();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TeamWin;

	ERobotID GetRobotID(int Number);
	void SetRobotID(int Number, ERobotID ID);
	uint8 GetPlayerPos(int Number);
	void SetPlayerPos(int Number, uint8 Pos);
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	bool bEnableCameraShake = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Settings")
	bool bEnableControllerVibration = true;
};
