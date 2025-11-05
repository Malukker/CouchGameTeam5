// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "RobotBattleInputDataMenu.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleInputDataMenu : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveUp;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveDown;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveLeft;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveRight;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionValidate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionCancel;

};
