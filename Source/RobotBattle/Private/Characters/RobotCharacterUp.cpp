// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacterUp.h"

#include "EnhancedInputComponent.h"
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
		EnhancedInputComponent->BindAction(InputData->InputActionAttack1,ETriggerEvent::Started,this,&ARobotCharacterUp::OnInputAttack1);
	}
	
	if (InputData->InputActionAttack2)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttack2,ETriggerEvent::Started,this,&ARobotCharacterUp::OnInputAttack2);
	}
	
	if (InputData->InputActionAttack3)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttack3,ETriggerEvent::Started,this,&ARobotCharacterUp::OnInputAttack3);
	}
	
	if (InputData->InputActionAttackDuo)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttackDuo,ETriggerEvent::Started,this,&ARobotCharacterUp::OnInputAttackDuo);
	}

#pragma endregion
}

#pragma region Attacks
void ARobotCharacterUp::OnInputAttack1(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = 0;
	InputAttackEvent.Broadcast(CurrentTypeAttack);
}

void ARobotCharacterUp::OnInputAttack2(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = 1;
	InputAttackEvent.Broadcast(CurrentTypeAttack);
}

void ARobotCharacterUp::OnInputAttack3(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = 2;
	InputAttackEvent.Broadcast(CurrentTypeAttack);
}

void ARobotCharacterUp::OnInputAttackDuo(const FInputActionValue& InputActionValue)
{
	CurrentTypeAttack = 3;
	InputAttackEvent.Broadcast(CurrentTypeAttack);
}
#pragma endregion

ERobotCharacterPositionEnum ARobotCharacterUp::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::Up;
}

