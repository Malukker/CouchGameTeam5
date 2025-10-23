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

	FStunEvent StunEvent;
	FLockEvent LockEvent;

	UPROPERTY(EditAnywhere)
	uint8 Team = 0;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AArenaPlayerStart> SpawnPoint;
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotParts;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ATeamManager> Opponent;

	uint8 TeamLife = 0;
	
	uint8 TeamGuardMax = 0;
	uint8 TeamGuard = 0;
	bool IsGuarding = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnCharacters();

	FVector GetOpponentLocation();
	FVector GetTeamLocation();

	void TeamTakeDamage(float Damage, float StunTime);
	
	void TeamPartLock(ERobotCharacterPositionEnum Position);
	void TeamPartUnlock(ERobotCharacterPositionEnum Position);
	
private:
	URobotCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position);

	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const;
	
};
