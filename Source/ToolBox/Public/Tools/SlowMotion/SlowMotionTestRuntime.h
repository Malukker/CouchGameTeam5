// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlowMotionTestRuntime.generated.h"

/**
 * 
 */
UCLASS()
class TOOLBOX_API USlowMotionTestRuntime : public UUserWidget
{
	GENERATED_BODY()

	public:
	UPROPERTY(BlueprintReadWrite)
	float Duration;

	UPROPERTY(BlueprintReadWrite)
	float Scale;

	UFUNCTION(BlueprintCallable)
	void SetSlowMotion();

	UFUNCTION(BlueprintCallable)
	void SaveSlowMotion();

	UFUNCTION(BlueprintCallable)
	void DestroyActor();


	

	private:
	FTimerHandle TimerHandle;
	virtual void Tick(float DeltaTime);

	
};
