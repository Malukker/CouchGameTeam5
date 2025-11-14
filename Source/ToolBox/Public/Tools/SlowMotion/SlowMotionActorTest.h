// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlowMotionTestRuntime.h"
#include "DynamicMesh/MeshTransforms.h"
#include "GameFramework/Actor.h"
#include "SlowMotionActorTest.generated.h"

DECLARE_DELEGATE_TwoParams(FStartSlowMotion,float,float);
UCLASS()

class TOOLBOX_API ASlowMotionActorTest : public AActor
{
	GENERATED_BODY()

public:
	FStartSlowMotion StartSlowMotion;
	
	ASlowMotionActorTest();
	UPROPERTY()
	TSubclassOf<USlowMotionTestRuntime> ShakeTestWidget;

	UFUNCTION()
	void OnEventStartSlowMotion(float Scale, float Duration);
private:
	virtual void Destroyed() override;
	
	UPROPERTY()
	USlowMotionTestRuntime* WidgetTest;

	FTimerHandle TimerHandle;

	virtual void Tick(float DeltaSeconds) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
