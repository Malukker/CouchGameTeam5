// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/RobotCharacterUp.h"

#include "EnhancedInputComponent.h"
#include "MathUtil.h"
#include "Characters/RobotCharacterInputData.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateID.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Kismet/GameplayStatics.h"

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
	if (InputDataGameplay->InputActionAttack1)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttack1, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack1);
	}

	if (InputDataGameplay->InputActionAttack2)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttack2, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack2);
	}

	if (InputDataGameplay->InputActionAttack3)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttack3, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack3);
	}

#pragma endregion
}

#pragma region Attacks
void ARobotCharacterUp::OnInputAttack1(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return;
	CurrentTypeAttack = EAttackID::Type1;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttack2(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return;
	CurrentTypeAttack = EAttackID::Type2;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttack3(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return;
	CurrentTypeAttack = EAttackID::Type3;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttackDuo(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	AttackDuoManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
}
#pragma endregion

bool ARobotCharacterUp::StartAttackDuo()
{
	if (!CanAttackDuo) return false;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return false;
	CurrentTypeAttack = EAttackID::Ultimate;
	InputAttackEvent.Broadcast();
	return true;
}

ERobotCharacterPositionEnum ARobotCharacterUp::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::Up;
}

void ARobotCharacterUp::OnInputRightDash(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	DashDirectionX = 1;
	if (FMathf::Sign(OrientX) != FMathf::Sign(DashDirectionX))
	{
		InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}

void ARobotCharacterUp::OnInputLeftDash(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	DashDirectionX = -1;
	if (FMathf::Sign(OrientX) != FMathf::Sign(DashDirectionX))
	{
		InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}

void ARobotCharacterUp::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	InputMoveX = InputActionValue.Get<float>();
}