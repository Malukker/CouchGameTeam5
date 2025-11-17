// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Match/RobotGameInstance.h"
#include "SelectRobotRuntime.generated.h"

/**
 * 
 */
UCLASS()
class TOOLBOX_API USelectRobotRuntime : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Controllers;

	virtual void NativeConstruct() override;


	UFUNCTION(BlueprintCallable)
	void SetControllers(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId);

	UFUNCTION(BlueprintCallable)
	void DestroyActor();

	UFUNCTION(BlueprintImplementableEvent)
	void OnInputChange(int DeviceID,EInputDeviceConnectionState State);

	UFUNCTION(BlueprintCallable)
	void ChangeRobotController(int PlayerIndex, int PlayerController);

private :
	
};
