// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotCharacter.h"
#include "RobotCharacterDown.generated.h"

UCLASS()
class ROBOTBATTLE_API ARobotCharacterDown : public ARobotCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARobotCharacterDown();
	virtual void BeginPlay() override;

protected:
	
	void OnInputJump(const FInputActionValue& InputActionValue);
	
	void OnInputBoost(const FInputActionValue& InputActionValue);
	
	virtual void OnInputAttackDuo(const FInputActionValue& InputActionValue) override;

	virtual void OnInputLeftDash(const FInputActionValue& InputActionValue) override;

	virtual void OnInputRightDash(const FInputActionValue& InputActionValue) override;

	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual ERobotCharacterPositionEnum GetPositionEnum() override;
	
	UPROPERTY(EditAnywhere)
	ERobotCharacterDownChargeID RobotCharacterDownChargeID = ERobotCharacterDownChargeID::None;

	virtual ERobotCharacterDownChargeID GetRobotCharacterDownChargeID() override;
};
