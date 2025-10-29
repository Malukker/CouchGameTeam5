// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MatchManager.generated.h"

class URobotBattleHUD;
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
	TSubclassOf<URobotBattleHUD> UIGameplay;
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<ATeamManager>> Teams;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
