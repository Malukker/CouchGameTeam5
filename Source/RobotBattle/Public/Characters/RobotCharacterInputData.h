// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "RobotCharacterInputData.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotCharacterInputData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveX;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionJump;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionRightDash;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionLeftDash;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionAttack1;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionAttack2;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionAttack3;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionAttackDuo;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionBoost;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionPause;


};
