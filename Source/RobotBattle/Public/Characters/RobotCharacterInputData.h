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
	TObjectPtr<UInputAction> InputActionMoveXFast;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionJump;
};
