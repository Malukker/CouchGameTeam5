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

	UPROPERTY(EditAnywhere)
	FName UpperBodySocketName;

protected:

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputJump(const FInputActionValue& InputActionValue);

	void OnInputDash(const FInputActionValue& InputActionValue);

	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) override;

	virtual ERobotCharacterPositionEnum GetPositionEnum() override;
};
