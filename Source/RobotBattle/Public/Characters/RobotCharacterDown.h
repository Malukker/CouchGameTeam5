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
	
	UPROPERTY(EditAnywhere)
	uint8 MaxCharge = 0;

protected:

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputJump(const FInputActionValue& InputActionValue);

	void OnInputLeftDash(const FInputActionValue& InputActionValue);
	
	void OnInputRightDash(const FInputActionValue& InputActionValue);

	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual ERobotCharacterPositionEnum GetPositionEnum() override;

	virtual void ManageChargeEvent() override;
	
	UPROPERTY(VisibleAnywhere)
	uint8 CurrentCharge = 0;

	UFUNCTION()
	virtual void IncrementCurrentCharge();
	
	UPROPERTY(EditAnywhere)
	ERobotCharacterDownChargeID RobotCharacterDownChargeID = ERobotCharacterDownChargeID::None;

	virtual ERobotCharacterDownChargeID GetRobotCharacterDownChargeID() override;
};
