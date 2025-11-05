// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotCharacter.h"
#include "RobotCharacterUp.generated.h"

UCLASS()
class ROBOTBATTLE_API ARobotCharacterUp : public ARobotCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARobotCharacterUp();

protected:
	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) override;
	
	void OnInputAttack1(const FInputActionValue& InputActionValue);
	void OnInputAttack2(const FInputActionValue& InputActionValue);
	void OnInputAttack3(const FInputActionValue& InputActionValue);
	virtual void OnInputAttackDuo(const FInputActionValue& InputActionValue) override;
	virtual void OnInputRightDash(const FInputActionValue& InputActionValue) override;
	virtual void OnInputLeftDash(const FInputActionValue& InputActionValue) override;
	virtual void OnInputMoveX(const FInputActionValue& InputActionValue) override;
	virtual ERobotCharacterPositionEnum GetPositionEnum() override;
};
