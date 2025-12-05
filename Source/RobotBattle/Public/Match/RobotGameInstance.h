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


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TeamWin;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int TeamLoose;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool Win=false;

	

	ERobotID GetRobotID(int Number);
	void SetRobotID(int Number, ERobotID ID);
	uint8 GetPlayerPos(int Number);
	void SetPlayerPos(int Number, uint8 Pos);
	
	
};
