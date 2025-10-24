// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Characters/RobotCharacterPositionEnum.h>
#include <Characters/Attacks/AttackStruct.h>
#include "TeamManager.generated.h"

class URobotCharacterInputData;
class ARobotCharacter;
class AArenaPlayerStart;
class UInputMappingContext;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStunEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLockEvent);

UCLASS()
class ROBOTBATTLE_API ATeamManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeamManager();

	UPROPERTY(EditAnywhere)
	uint8 Team = 0;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AArenaPlayerStart> SpawnPoint;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ATeamManager> Opponent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnCharacters();

	FVector GetOpponentLocation();
	
private:
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotParts;

	int TeamLife = 0;
	
	int TeamGuardMax = 0;
	int TeamGuard = 0;
	bool CanTakeDamage = false;
	
	FVector GetTeamLocation();

	UFUNCTION()
	void TeamTakeDamage(int Damage, float StunTime);
	
	UFUNCTION()
	void TeamPartLock(ERobotCharacterPositionEnum Position);
	
	URobotCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position);

	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const;
	
};
