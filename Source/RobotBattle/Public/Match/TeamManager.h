// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeamManager.generated.h"

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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
