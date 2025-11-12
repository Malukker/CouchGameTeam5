// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShakeActorTest.generated.h"

class UShakeCameraTestRuntime;

UCLASS()
class TOOLBOX_API AShakeActorTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShakeActorTest();
	UPROPERTY()
	TSubclassOf<UShakeCameraTestRuntime> ShakeTestWidget;
	virtual void Destroyed() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY()
	UShakeCameraTestRuntime* WidgetTest;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
