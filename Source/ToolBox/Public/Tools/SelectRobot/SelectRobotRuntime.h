// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "Match/RobotGameInstance.h"
#include "SelectRobotRuntime.generated.h"


/**
 * 
 */
DECLARE_DELEGATE_TwoParams(FControllerStateChange,EInputDeviceConnectionState,FInputDeviceId);
UCLASS()
class TOOLBOX_API USelectRobotRuntime : public UUserWidget
{
	GENERATED_BODY()

public :
	UPROPERTY(BlueprintReadOnly)
	TArray<int> Controllers;

	FControllerStateChange ControllerStateChange;

	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	UComboBoxString* ComboBox_Controller;
	

	UFUNCTION(BlueprintCallable)
	void DestroyActor();
	

	UFUNCTION(BlueprintCallable)
	void ChangeRobotController(int PlayerIndex, int PlayerController);

	UFUNCTION()
	void UpdateControllerComboBox(EInputDeviceConnectionState NewConnectionState,FPlatformUserId PlateformUserID,FInputDeviceId InputDeviceId);

	



private :
	UPROPERTY()
	URobotGameInstance* GI = nullptr;

	UPROPERTY()
	UInputDeviceSubsystem* Subsystem = nullptr;
	
};
