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

protected:

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputJump(const FInputActionValue& InputActionValue);

	void OnInputLeftDash(const FInputActionValue& InputActionValue);
	
	void OnInputRightDash(const FInputActionValue& InputActionValue);

	void 

	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual ERobotCharacterPositionEnum GetPositionEnum() override;
};
