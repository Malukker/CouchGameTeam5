// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlowMotionTestRuntime.h"
#include "GameFramework/Actor.h"
#include "SlowMotionActorTest.generated.h"

UCLASS()
class TOOLBOX_API ASlowMotionActorTest : public AActor
{
	GENERATED_BODY()

public:
	
	ASlowMotionActorTest();
	UPROPERTY()
	TSubclassOf<USlowMotionTestRuntime> ShakeTestWidget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
