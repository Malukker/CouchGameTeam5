// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/RobotCharacterUp.h"

#include "EnhancedInputComponent.h"
#include "MathUtil.h"
#include "Characters/RobotCharacterInputData.h"
#include "Characters/RobotCharacterPositionEnum.h"

// Sets default values
ARobotCharacterUp::ARobotCharacterUp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawn

void ARobotCharacterUp::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::BindInputAndActions(EnhancedInputComponent);
#pragma region AttackBindings
	if (InputData->InputActionAttack1)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttack1, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack1);
	}

	if (InputData->InputActionAttack2)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttack2, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack2);
	}

	if (InputData->InputActionAttack3)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttack3, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack3);
	}

	if (InputData->InputActionAttackDuo)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttackDuo, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttackDuo);
	}

	if (InputData->InputActionRightDash)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionRightDash,ETriggerEvent::Started,this,&ARobotCharacterUp::OnInputRightDash);
	}
	
	if (InputData->InputActionLeftDash)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionLeftDash,ETriggerEvent::Started,this,&ARobotCharacterUp::OnInputLeftDash);
	}

#pragma endregion
}

#pragma region Attacks
void ARobotCharacterUp::OnInputAttack1(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = EAttackID::Type1;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttack2(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = EAttackID::Type2;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttack3(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = EAttackID::Type3;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttackDuo(const FInputActionValue& InputActionValue)
{
	if (CanAttackDuo == false) return;
	CurrentTypeAttack = EAttackID::Ultimate;
	InputAttackEvent.Broadcast();
	CanAttackDuo = false;
	ChargeManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
}
#pragma endregion

ERobotCharacterPositionEnum ARobotCharacterUp::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::Up;
}

void ARobotCharacterUp::ManageChargeEvent()
{
	CanAttackDuo = true;
}

void ARobotCharacterUp::OnInputRightDash(const FInputActionValue& InputActionValue)
{
	DashDirectionX = 1;
	if (FMathf::Sign(OrientX) != FMathf::Sign(DashDirectionX))
	{
		InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}

void ARobotCharacterUp::OnInputLeftDash(const FInputActionValue& InputActionValue)
{
	DashDirectionX = -1;
	if (FMathf::Sign(OrientX) != FMathf::Sign(DashDirectionX))
	{
		InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}