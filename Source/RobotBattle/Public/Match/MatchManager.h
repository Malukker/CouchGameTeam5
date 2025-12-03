// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchManager.generated.h"

class URobotBattleGameplayUI;
class UUserWidget;
class ATeamManager;

UCLASS()
class ROBOTBATTLE_API AMatchManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMatchManager();

	UPROPERTY(EditAnywhere)
	float RoundTime = 60;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URobotBattleGameplayUI> UIGameplayClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIGameOverClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIRoundClass;

	UPROPERTY()
	URobotBattleGameplayUI* UIGameplay;
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ATeamManager>> Teams;
	UPROPERTY()
	TArray<int> TeamsWin;

	UPROPERTY()
	int Round = 0;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoundEvent, int, Round);

	UPROPERTY(BlueprintAssignable, Category="MatchManager")
	FRoundEvent RoundEvent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetFight();
	
	UFUNCTION()
	void EndFightOnTimeOut();

	UFUNCTION()
	void EndFightOnRobotDefeat(int LosingTeam);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
