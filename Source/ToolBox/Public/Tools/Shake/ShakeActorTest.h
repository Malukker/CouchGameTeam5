// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShakeActorTest.generated.h"

UCLASS()
class TOOLBOX_API AShakeActorTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShakeActorTest();
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> ShakeTestWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
