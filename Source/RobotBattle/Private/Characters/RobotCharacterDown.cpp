// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacterDown.h"
#include "Characters/RobotCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateID.h"
#include "Characters/RobotCharacterStateMachine.h"


// Sets default values
ARobotCharacterDown::ARobotCharacterDown()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARobotCharacterDown::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ARobotCharacterDown::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputJumpEvent.Broadcast();
}

void ARobotCharacterDown::OnInputRightDash(const FInputActionValue& InputActionValue)
{
	if (!CanDash) return;
	DashDirectionX = 1;
	InputDashEvent.Broadcast();
}

void ARobotCharacterDown::OnInputLeftDash(const FInputActionValue& InputActionValue)
{
	if (!CanDash) return;
	DashDirectionX = -1;
	InputDashEvent.Broadcast();
}

void ARobotCharacterDown::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::BindInputAndActions(EnhancedInputComponent);
	
	if (InputData->InputActionMoveX) {
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ARobotCharacterDown::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ARobotCharacterDown::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ARobotCharacterDown::OnInputMoveX
		);
	}

	if (InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionJump,ETriggerEvent::Started,this,&ARobotCharacterDown::OnInputJump);
	}

	if (InputData->InputActionRightDash)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionRightDash,ETriggerEvent::Started,this,&ARobotCharacterDown::OnInputRightDash);
	}
	
	if (InputData->InputActionLeftDash)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionLeftDash,ETriggerEvent::Started,this,&ARobotCharacterDown::OnInputLeftDash);
	}
}

ERobotCharacterPositionEnum ARobotCharacterDown::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::Down;
}

