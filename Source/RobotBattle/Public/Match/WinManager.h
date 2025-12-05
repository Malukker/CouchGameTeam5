// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraSettings.h"
#include "GameFramework/Actor.h"
#include "WinManager.generated.h"

class ARobotCharacter;
enum class ERobotCharacterPositionEnum : uint8;

UCLASS()
class ROBOTBATTLE_API AWinManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWinManager();

	UPROPERTY(EditAnywhere)
	APlayerStart* WinnerSpawn;

	UPROPERTY(EditAnywhere)
	APlayerStart* LoserSpawn;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const;


private:
	UPROPERTY()
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotPartsWin;

	UPROPERTY()
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotPartsLose;
};
