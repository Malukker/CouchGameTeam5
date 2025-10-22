// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Characters/RobotCharacterPositionEnum.h>
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

	uint8 Team = 0;
	
	TObjectPtr<AArenaPlayerStart> SpawnPoint;
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotParts;
	TObjectPtr<ATeamManager> Opponent;

	uint8 TeamLife;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnCharacters();

	FVector GetOpponentLocation();
	FVector GetTeamLocation();

	void TeamTakeDamage(float Damage, bool CanBreakGuard);
	
	void TeamPartLock(ERobotCharacterPositionEnum Position);
	void TeamPartUnlock(ERobotCharacterPositionEnum Position);
	
private:
	URobotCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position);

	void InitCharacters();

	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const;
	
};
