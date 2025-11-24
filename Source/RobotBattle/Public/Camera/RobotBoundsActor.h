// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotBoundsActor.generated.h"

// class UBoxComponent;

UCLASS()
class ROBOTBATTLE_API ARobotBoundsActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARobotBoundsActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> Box_Right;

	
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UBoxComponent> Box_Left;

	UPROPERTY(EditAnywhere)
	TObjectPtr<class USceneComponent> Root;
	

};
