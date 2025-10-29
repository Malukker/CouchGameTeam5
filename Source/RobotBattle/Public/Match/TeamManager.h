// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Characters/RobotCharacterPositionEnum.h>
#include <Characters/Attacks/AttackStruct.h>
#include "TeamManager.generated.h"

class URobotCharacterInputData;
class IUIGamePlayInterface;
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

	TScriptInterface<IUIGamePlayInterface> UIInterface;
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnCharacters();

	FVector GetOpponentLocation();
	
private:
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotParts;

	float TeamLifeMax = 0;
	float TeamLife = 0;
	
	float TeamGuardMax = 0;
	float TeamGuard = 0;
	bool CanTakeDamage = true;
	bool CanGuard = false;
	
	float TeamChargeMax = 0;
	float TeamCharge = 0;
	
	FVector GetTeamLocation();

	UFUNCTION()
	void TeamTakeDamage(int Damage, float StunTime);
	
	UFUNCTION()
	void TeamPartLock(ERobotCharacterPositionEnum Position, bool Lock);
	
	UFUNCTION()
	void GuardReset();
	
	UFUNCTION()
	void DashInvinsibility(ERobotCharacterPositionEnum Position);
	
	UFUNCTION()
	void Guard(bool Guard);
	
	UFUNCTION()
	void Charge(ERobotCharacterPositionEnum Position);
	
	URobotCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position);

	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const;
	
};
