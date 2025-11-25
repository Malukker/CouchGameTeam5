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
	void InversePlayer();

	FVector GetOpponentLocation();
	float GetLife();
	
private:
	TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>> RobotParts;

	UPROPERTY()
	float TeamLifeMax = 0;
	UPROPERTY()
	float TeamLife = 0;
	
	UPROPERTY()
	float TeamGuardMax = 0;
	UPROPERTY()
	float TeamGuard = 0;
	UPROPERTY()
	bool CanGuard = false;
	
	UPROPERTY()
	int InvinsibilityFramesOrigin = 0;
	UPROPERTY()
	int InvinsibilityFrames = 0;
	UPROPERTY()
	float DashBuffer = 0;
	UPROPERTY()
	bool WantInvinsibility = false;
	UPROPERTY()
	bool IsDashing = false;
	UPROPERTY()
	bool CanTakeDamage = true;
	
	UPROPERTY()
	float UltimateBuffer = 0;
	UPROPERTY()
	bool IsUltimating = false;
	UPROPERTY()
	int WantUltimate = -1;
	
	UPROPERTY()
	float TeamChargeMax = 0;
	UPROPERTY()
	float TeamCharge = 0;

	UPROPERTY()
	float OriginalGravityScale = -1;

	UPROPERTY()
	int Combo = 0;
	UPROPERTY()
	float ComboTimer = 0.f;
	UPROPERTY()
	float ComboResetTime = 5.f;
	
	FVector GetTeamLocation();

	UFUNCTION()
	void TeamDoAttack(bool HasTouch);
	
	UFUNCTION()
	void TeamTakeDamage(int Damage, float StunTime, FVector2D KnockBackVelocity);
	
	UFUNCTION()
	void TeamAirBlock(bool Lock);
	
	UFUNCTION()
	void SwitchEnergy();
	
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
