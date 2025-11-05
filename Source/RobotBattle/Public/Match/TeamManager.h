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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathEvent, int, Team);

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
	TEnumAsByte<ECollisionChannel> TeamCollision;
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> OpponentCollision;
	
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> AttackChannel;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<AArenaPlayerStart> SpawnPoint;
	UPROPERTY(EditAnywhere)
	TObjectPtr<ATeamManager> Opponent;

	TScriptInterface<IUIGamePlayInterface> UIInterface;

	FDeathEvent DeathEvent;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SpawnCharacters();
	void ResetCharacters();

	FVector GetOpponentLocation();
	
private:
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotParts;

	float TeamLifeMax = 0;
	float TeamLife = 0;
	
	float TeamGuardMax = 0;
	float TeamGuard = 0;
	bool CanGuard = false;
	
	int InvinsibilityFramesOrigin = 0;
	int InvinsibilityFrames = 0;
	float DashBuffer = 0;
	bool WantInvinsibility = false;
	bool IsDashing = false;
	bool CanTakeDamage = true;
	
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
	
	UFUNCTION()
	void AttackDuo(ERobotCharacterPositionEnum Position);

	URobotCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position);

	TSubclassOf<ARobotCharacter> GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const;
	
};
